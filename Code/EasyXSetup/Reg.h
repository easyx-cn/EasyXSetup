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



tstring GetVCPath(int iVsVer, bool);
TCHAR const* GetIncludePath(int iVsVer);
TCHAR const* GetLibX86Path(int iVcVer);
TCHAR const* GetLibX64Path(int iVcVer);


//����·�����������ֻ������������
TCHAR** RegEnumKeys(HKEY defkey, TCHAR const* subkeyname, int* numkeys);

// ��ȡĳ����ֵ�Ե�ֵ
tstring RegRead(HKEY defkey, TCHAR const* subkeyname, TCHAR const* key);

tstring deleteFile(TCHAR const* dst);

// ���ݿ��ֽڡ����ֽڣ���������һ��
const TCHAR* toTCHAR(const char* src);
const char* toU8(const TCHAR* str, int len = -1);
const char* tochar(const char* src);
const char* tochar(const wchar_t* src);
const wchar_t* towchar(const TCHAR* src);

const TCHAR* spliceString(TCHAR const* s1, TCHAR const* s2);

tstring copy_Files(TCHAR const* src, TCHAR const* dst);
const char* g_pathDesktop();
TCHAR* help_path();
bool CreateLinkFile(LPCTSTR urlexePath, LPCTSTR szDestLnkPath);