/**
A patch for flserver to change the account directory path.
By cannon 15 July 2009.
*/
#include "Main.h"

static FARPROC fpOldGetUserDataPath = 0;
std::string set_scDataPath = { "" };

/* functionPrototypes
static void WriteProcMem(void *pAddress, void *pMem, int iSize);
std::string IniGetS(std::string scFile, std::string scApp, std::string scKey, std::string scDefault);
std::string Trim(std::string scIn);
char C HkCb_GetUserDataPath(LPSTR pszPath);
*/

static void WriteProcMem(void *pAddress, void *pMem, int iSize) {
	HANDLE hProc = { OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, GetCurrentProcessId()) };
	DWORD dwOld = { (DWORD)0 };

	BOOL res = { VirtualProtectEx(hProc, pAddress, iSize, PAGE_EXECUTE_READWRITE, &dwOld) };
	res = WriteProcessMemory(hProc, pAddress, pMem, iSize, 0);

	CloseHandle(hProc);
}

std::string IniGetS(std::string scFile, std::string scApp, std::string scKey, std::string scDefault) {
	char szRet[2048];
	memset(szRet, '\0', sizeof(szRet));

	GetPrivateProfileString(scApp.c_str(), scKey.c_str(), scDefault.c_str(), szRet, sizeof(szRet), scFile.c_str());

	return (szRet);
}

std::string Trim(std::string scIn) {
	while (scIn.length() && (scIn[0] == ' ' || scIn[0] == '\n' || scIn[0] == '\r'))
		scIn = scIn.substr(1);

	while (scIn.length() && (scIn[(scIn.length() - 1)] == L' ' || scIn[(scIn.length() - 1)] == '	' || scIn[(scIn.length() - 1)] == '\n' || scIn[(scIn.length() - 1)] == '\r'))
		scIn = scIn.substr(0, (scIn.length() - 1));

	return (scIn);
}

char C HkCb_GetUserDataPath(LPSTR pszPath) {
	strncpy(pszPath, set_scDataPath.c_str(), MAX_PATH);

	return (true);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	HMODULE hModCommon = { GetModuleHandleA("common") };
	DWORD dwJmpOffset = { (DWORD)((char*)HkCb_GetUserDataPath) - (DWORD)hModCommon - 0xA2FA0 - 5 };
	char szCurDir[MAX_PATH], PatchOffsets[(OFFSET_00_SZE + OFFSET_10_SZE)] = { OFFSET_00, OFFSET_10, OFFSET_11, OFFSET_12, OFFSET_13, OFFSET_14 },
		fpPatch[(OFFSET_00_SZE + OFFSET_10_SZE)];
	static bool bPatchInstalled = { false };
	memset(szCurDir, '\0', sizeof(szCurDir));
	memset(fpPatch, '\0', sizeof(fpPatch));

	if ((fdwReason == DLL_PROCESS_ATTACH) && !bPatchInstalled) {
		bPatchInstalled = true;

		GetCurrentDirectory(sizeof(szCurDir), szCurDir);
		std::string scServerCfgFile = std::string(szCurDir) + "\\acctpath_patch.cfg";
		set_scDataPath = Trim(IniGetS(scServerCfgFile, "General", "DataPath", ""));

		if (hModCommon && set_scDataPath.length()) {
			for (register unsigned int i = 0; i < OFFSET_00_SZE; i++)
				fpPatch[i] = PatchOffsets[i];

			WriteProcMem((char*)hModCommon + 0xA2FA0, &fpPatch, 1);
			WriteProcMem((char*)hModCommon + 0xA2FA1, &dwJmpOffset, 4);
		}
	}
	else if ((fdwReason == DLL_PROCESS_DETACH) && bPatchInstalled) {
		bPatchInstalled = false;

		if (hModCommon && set_scDataPath.length()) {
			for (register unsigned int i = 1; i < (OFFSET_00_SZE + OFFSET_10_SZE); i++)
				fpPatch[i] = PatchOffsets[i];

			WriteProcMem((char*)hModCommon + 0xA2FA0, &fpPatch, 5);
		}
	}

	return (true);
}
