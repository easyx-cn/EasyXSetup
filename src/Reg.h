#pragma once
#include <windows.h>
#include <string>

using namespace std;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#ifdef _UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif



wstring GetVCPath(int iVsVer, bool);
wchar_t const* GetIncludePath(int iVsVer);
wchar_t const* GetLibX86Path(int iVcVer);
wchar_t const* GetLibX64Path(int iVcVer);


//遍历路劲下所有子项，只返回子项名称
wchar_t** RegEnumKeys(HKEY defkey, wchar_t const* subkeyname, int* numkeys);

// 读取某个键值对的值
wstring RegRead(HKEY defkey, wchar_t const* subkeyname, wchar_t const* key);

wstring deleteFile(wchar_t const* dst);

const char* toU8(const wchar_t* str, int len = -1);

wstring copy_Files(wchar_t const* src, wchar_t const* dst);
const wchar_t* g_pathDesktop();
wchar_t* help_path();
bool CreateLinkFile(LPCWSTR urlexePath, LPCWSTR szDestLnkPath);