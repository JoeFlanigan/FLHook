#include <windows.h>
#include <string>
#include <map>

#pragma comment(lib, "../plugins/flhookplugin_sdk/libs/FLCoreCommon.lib")

#define              EXPORT __declspec(dllexport)
#define              IMPORT __declspec(dllimport)
#define    OBJECT_DATA_SIZE 2048
#define WIN32_LEAN_AND_MEAN

using namespace std;

/* functionPrototypes
extern "C" EXPORT wstring* CreateWString(wchar_t *wszStr);
extern "C" EXPORT void FreeWString(wstring *wscStr);	
extern "C" EXPORT string* CreateString(char *szStr);	
extern "C" EXPORT void FreeString(string *scStr);
extern "C" EXPORT const char *GetCString(string *scStr);
extern "C" EXPORT const wchar_t *GetWCString(wstring *wscStr);
extern "C" EXPORT void WStringAssign(wstring *wscStr, const wchar_t *wszCStr);
extern "C" EXPORT void WStringAppend(wstring *wscStr, const wchar_t *wszCStr);
extern "C" EXPORT wstring *CPlayerAccount_GetServerSignature(CPlayerAccount* cpa, const char* sig);
*/

extern "C" EXPORT wstring *CreateWString(wchar_t *wszStr) {
	wstring *wscStr = new wstring;
	*wscStr = wszStr;

	return (wscStr);
}

extern "C" EXPORT void FreeWString(wstring *wscStr) {
	delete wscStr;
}

extern "C" EXPORT string *CreateString(char *szStr) {
	string *scStr = new string;
	*scStr = szStr;

	return (scStr);
}

extern "C" EXPORT void FreeString(string *scStr) {
	delete scStr;
}

extern "C" EXPORT const char *GetCString(string *scStr) {
	return (scStr->c_str());
}

extern "C" EXPORT const wchar_t *GetWCString(wstring *wscStr) {
	return (wscStr->c_str());
}

extern "C" EXPORT void WStringAssign(wstring *wscStr, const wchar_t *wszCStr) {
	wscStr->assign(wszCStr);
}

extern "C" EXPORT void WStringAppend(wstring *wscStr, const wchar_t *wszCStr) {
	wscStr->append(wszCStr);
}

class IMPORT CPlayerAccount {
public:
	CPlayerAccount(class CPlayerAccount const &);
	CPlayerAccount(void);
	virtual ~CPlayerAccount(void);
	class CPlayerAccount & operator=(class CPlayerAccount const &);
	void GenerateAccount(char const *);
	static bool  GenerateTextKey(char *);
	wstring GetAccountName(void);
	char const * GetAccountNameChar(void);
	wstring GetAccountNameSig(void);
	char const * GetAccountNameSigChar(void);
	wstring GetServerSignature(char const *);
	static bool  GetTextKey(char *);
	bool LoadAccount(void);
	bool LoadAccountFromStrings(char const *, char const *);
	static void  SetAcctIndex(unsigned long);
	static bool  SetTextKey(char *);
	bool VerifyAccount(void);

protected:
	void TossHashes(void);
	static unsigned long  g_dwAccountIndex;

public:
	unsigned char data[OBJECT_DATA_SIZE];
};

extern "C" EXPORT wstring *CPlayerAccount_GetServerSignature(CPlayerAccount* cpa, const char* sig) {
	wstring ws = { cpa->GetServerSignature(sig) };

	wstring* ws2 = new wstring;
	ws2->assign(ws);

	return (ws2);
}
