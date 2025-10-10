#include "Reg.h"
#include <Shlobj.h>
#include <direct.h>
#include <regex>
#include <tchar.h>


using namespace std;


TCHAR** RegEnumKeys(HKEY defkey, TCHAR const* subkeyname, int* numkeys)
{
	TCHAR** list = NULL;
	HKEY hKey;
	if (RegOpenKeyEx(defkey, subkeyname, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return list;

	TCHAR    achKey[MAX_KEY_LENGTH];			// buffer for subkey name
	DWORD    cbNameLength;						// size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");		// buffer for class name 
	DWORD    cchClassName = MAX_PATH;			// size of class string 
	DWORD    cSubKeys = 0;						// number of subkeys 
	DWORD    cbMaxSubKey;						// longest subkey size 
	DWORD    cchMaxClass;						// longest class string 
	DWORD    cValues;							// number of values for key 
	DWORD    cchMaxValue;						// longest value name 
	DWORD    cbMaxValueData;					// longest value data 
	DWORD    cbSecurityDescriptor;				// size of security descriptor 
	FILETIME ftLastWriteTime;					// last write time 

	DWORD i, retCode;

	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	if (cSubKeys)
	{
		*numkeys = cSubKeys;
		list = new TCHAR * [cSubKeys];
		for (i = 0; i < cSubKeys; i++)
		{
			cbNameLength = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i, achKey, &cbNameLength, NULL, NULL, NULL, &ftLastWriteTime);

			if (cSubKeys == i + 1)
			{
				int i = 0;
			}
			if (retCode == ERROR_SUCCESS)
			{
				list[i] = new TCHAR[cbNameLength + 1];
				_tcscpy_s(list[i], cbNameLength + 1, achKey);
			}
		}

		int s[2] = { 2, 4 };
	}

	RegCloseKey(hKey);        // 程序结束前要关闭已经打开的 hKEY。
	return list;
}

/// <summary>
/// subkeyname 不以 \\ 开头、结尾
/// </summary>
/// <param name="defkey"></param>
/// <param name="subkeyname"></param>
tstring RegRead(HKEY defkey, TCHAR const* subkeyname, TCHAR const* key)
{
	if (key == NULL)
		return _T("");

	TCHAR* result = NULL;
	HKEY hKey;
	DWORD dataType;
	TCHAR data[1024] = { 0 };
	DWORD dataSize = sizeof(data);

	if (defkey == HKEY_CURRENT_USER)
	{
		int keyNum;
		TCHAR** aNames = RegEnumKeys(HKEY_USERS, NULL, &keyNum);

		for (int i = 0; i < keyNum; i++)
		{
			int len = lstrlen(aNames[i]);
			if (len < 2)
				continue;

			TCHAR const* s1 = TEXT("S-");
			TCHAR* sub_1 = new TCHAR[3];
			_tcsncpy_s(sub_1, 3, aNames[i], 2);
			sub_1[2] = '\0';

			if (_tcscmp(sub_1, s1) != 0)
				continue;

			if (len > 8) {
				TCHAR const* s2 = TEXT("_Classes");
				TCHAR* sub_2 = new TCHAR[9];
				_tcsncpy_s(sub_2, 9, aNames[i] + len - 8, 8);   // 截取后面 8 个字符
				sub_2[8] = '\0';
				if (_tcscmp(sub_2, s2) == 0)
					continue;
			}

			int buf_len = len + 1 + lstrlen(subkeyname) + 1;
			TCHAR* buf = new TCHAR[buf_len];
			_sntprintf_s(buf, buf_len, buf_len, _T("%s%s%s"), aNames[i], _T("\\"), subkeyname);
			buf[buf_len - 1] = '\0';

			int re = RegOpenKeyEx(HKEY_USERS, buf, NULL, KEY_READ, &hKey);
			delete buf;
			if (re != ERROR_SUCCESS)
				continue;

			if (RegQueryValueEx(hKey, key, NULL, &dataType, (LPBYTE)data, &dataSize) != ERROR_SUCCESS)
			{
				RegCloseKey(hKey);
				return _T("");
			}

			RegCloseKey(hKey);
		}

		tstring str = data;
		return str;
	}

	if (RegOpenKeyEx(defkey, subkeyname, NULL, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("");
	}

	if (RegQueryValueEx(hKey, key, NULL, &dataType, (LPBYTE)data, &dataSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("");
	}

	RegCloseKey(hKey);

	tstring str = data;
	return str;
}

tstring GetVCPath(int iVsVer, bool g_bX64)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	TCHAR const* path = NULL;
	TCHAR const* subkeyname = NULL;
	TCHAR const* key = NULL;

	switch (iVsVer)
	{
	case 6:
		hKey = HKEY_CURRENT_USER;
		subkeyname = _T("Software\\Microsoft\\DevStudio\\6.0\\Build System\\Components\\Platforms\\Win32 (x86)\\Directories");
		key = _T("Library Dirs");
		break;

	case 2008:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = _T("Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7");
		else
			subkeyname = _T("Software\\Microsoft\\VisualStudio\\SxS\\VC7");
		key = _T("9.0");
		break;

	case 2010:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = _T("Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7");
		else
			subkeyname = _T("Software\\Microsoft\\VisualStudio\\SxS\\VC7");
		key = _T("10.0");
		break;

	case 2012:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = _T("Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7");
		else
			subkeyname = _T("Software\\Microsoft\\VisualStudio\\SxS\\VC7");
		key = _T("11.0");
		break;

	case 2013:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = _T("Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7");
		else
			subkeyname = _T("Software\\Microsoft\\VisualStudio\\SxS\\VC7");
		key = _T("12.0");
		break;

	case 2015:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = _T("Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7");
		else
			subkeyname = _T("Software\\Microsoft\\VisualStudio\\SxS\\VC7");
		key = _T("14.0");
		break;

	case 2017:
	{
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = _T("Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
		else
			subkeyname = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

		int numKeys;
		int len = lstrlen(subkeyname);
		TCHAR** aNames = RegEnumKeys(hKey, subkeyname, &numKeys);
		for (int i = 0; i < numKeys; i++)
		{
			int buf_len = len + 1 + lstrlen(aNames[i]) + 1;
			TCHAR* buf = new TCHAR[buf_len];
			_sntprintf_s(buf, buf_len, buf_len, _T("%s%s%s"), subkeyname, _T("\\"), aNames[i]);
			buf[buf_len - 1] = '\0';

			tstring rtn = RegRead(hKey, buf, _T("DisplayName"));
			if (rtn != _T(""))
			{
				regex rex("^Visual Studio (Community|Professional|Enterprise) 2017(\\([^\\)]+\\))?$", regex_constants::icase);  //   \\( 代替原来的 \(      匹配括号
				const char* str = tochar(rtn.c_str());
				string s = str;

				bool result = regex_search(s, rex);
				if (result)
				{
					subkeyname = buf;
					key = _T("InstallLocation");
					break;
				}
			}
		}

		break;
	}

	case 2019:
	{
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = _T("Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
		else
			subkeyname = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

		int numKeys;
		int len = lstrlen(subkeyname);
		TCHAR** aNames = RegEnumKeys(hKey, subkeyname, &numKeys);
		for (int i = 0; i < numKeys; i++)
		{
			int buf_len = len + 1 + lstrlen(aNames[i]) + 1;
			TCHAR* buf = new TCHAR[buf_len];
			_sntprintf_s(buf, buf_len, buf_len, _T("%s%s%s"), subkeyname, _T("\\"), aNames[i]);
			buf[buf_len - 1] = '\0';

			tstring rtn = RegRead(hKey, buf, _T("DisplayName"));
			if (rtn != _T("")) {
				const char* str = tochar(rtn.c_str());

				regex rex("^Visual Studio (Community|Professional|Enterprise) 2019( \\([^\\)]+\\))?$", regex_constants::icase);  //   \\( 代替原来的 \(      匹配括号
				string s = str;
				bool result = regex_search(s, rex);
				if (result)
				{
					subkeyname = buf;
					key = _T("InstallLocation");
					break;
				}
			}
		}
		break;
	}

	case 2022:
	{
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = _T("Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
		else
			subkeyname = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

		int numKeys;
		int len = lstrlen(subkeyname);
		TCHAR** aNames = RegEnumKeys(hKey, subkeyname, &numKeys);
		for (int i = 0; i < numKeys; i++)
		{
			int buf_len = len + 1 + lstrlen(aNames[i]) + 1;
			TCHAR* buf = new TCHAR[buf_len];
			_sntprintf_s(buf, buf_len, buf_len, _T("%s%s%s"), subkeyname, _T("\\"), aNames[i]);
			buf[buf_len - 1] = '\0';

			tstring rtn = RegRead(hKey, buf, _T("DisplayName"));
			if (rtn != _T("")) {
				const char* str = tochar(rtn.c_str());
				regex rex("^Visual Studio (Community|Professional|Enterprise) 2022( \\([^\\)]+\\))?$", regex_constants::icase);  //   \\( 代替原来的 \(      匹配括号
				string s = str;
				bool result = regex_search(s, rex);
				if (result)
				{
					subkeyname = buf;
					key = _T("InstallLocation");
					break;
				}
			}
		}
		break;
	}
	}

	tstring localPath = RegRead(hKey, subkeyname, key);
	switch (iVsVer)
	{
	case 6:
		if (localPath != _T("")) {
			string str = tochar(localPath.c_str());
			regex rex("[^;]+\\\\(?=vc98\\\\mfc\\\\lib$|vc98\\\\mfc\\\\lib;)", regex_constants::icase);
			smatch what;
			bool result = regex_search(str, what, rex);
			if (result)
			{
				size_t len = strlen(what[0].str().c_str()) + strlen("VC98\\") + 1;
				TCHAR* buf = new TCHAR[len];
				_sntprintf_s(buf, len, len, _T("%s%s"), toTCHAR(what[0].str().c_str()), _T("VC98\\"));
				localPath = buf;
			}
		}
		break;
	case 2017:
	case 2019:
	case 2022:
		if (localPath != _T("")) {
			int len = lstrlen(localPath.c_str());
			if (localPath[len - 1] != '\\')
				localPath = spliceString(localPath.c_str(), TEXT("\\VC\\Auxiliary\\VS\\"));
			else
				localPath = spliceString(localPath.c_str(), TEXT("VC\\Auxiliary\\VS\\"));
		}
		break;
	default: break;
	}

	if (localPath != _T("")) {
		DWORD dir = GetFileAttributes(localPath.c_str());
		if (dir != FILE_ATTRIBUTE_DIRECTORY)
			localPath = _T("");
	}

	if (localPath != _T("")) {
		int len = lstrlen(localPath.c_str());
		if (localPath[len - 1] != '\\')
			localPath = spliceString(localPath.c_str(), _TEXT("\\"));
	}

	return localPath;
}

TCHAR const* GetIncludePath(int iVsVer) {
	if (iVsVer == 6)
		return TEXT("Include\\");

	return TEXT("include\\");
}

TCHAR const* GetLibX86Path(int iVcVer) {
	switch (iVcVer) {
	case 6:     return _T("Lib\\");
	case 2008:
	case 2010:
	case 2012:
	case 2013:
	case 2015: return _T("lib\\");
	case 2017:
	case 2019:
	case 2022: return _T("lib\\x86\\");
	}

	return _T("");
}

TCHAR const* GetLibX64Path(int iVcVer) {
	switch (iVcVer) {
	case 6:     return NULL;
	case 2008:
	case 2010:
	case 2012:
	case 2013:
	case 2015: return _T("lib\\amd64\\");
	case 2017:
	case 2019:
	case 2022: return _T("lib\\x64\\");
	}
	return _T("");
}


const char* g_pathDesktop() {
	wchar_t* path = NULL;
	HRESULT result = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path);
	if (result != S_OK)
		return NULL;
	//CoTaskMemFree(path);

	return tochar(path);
}

TCHAR* help_path() {
	const char* dest = g_pathDesktop();
	TCHAR const* p2 = TEXT("\\EasyX_Help.lnk");

	int len = lstrlen(toTCHAR(dest)) + lstrlen(p2) + 1;
	TCHAR* buf = new TCHAR[len];
	_sntprintf_s(buf, len, len, _T("%s%s"), toTCHAR(dest), p2);
	buf[len - 1] = '\0';

	return buf;
}

/// <summary>
/// 创建快捷方式
/// </summary>
/// <param name="szStartAppPath"></param>
/// <param name="szDestLnkPath"></param>
/// <returns></returns>
bool CreateLinkFile(LPCTSTR urlexePath, LPCTSTR szDestLnkPath)
{
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink* pShellLink;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
		if (SUCCEEDED(hr))
		{
			pShellLink->SetPath(urlexePath);
			tstring strTmp = urlexePath;
			size_t nStart = strTmp.find_last_of(_T("/\\"));
			pShellLink->SetWorkingDirectory(strTmp.substr(0, nStart).c_str());
			IPersistFile* pPersistFile;
			hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
			if (SUCCEEDED(hr))
			{
				hr = pPersistFile->Save(towchar(szDestLnkPath), FALSE);
				if (SUCCEEDED(hr))
					return true;
				pPersistFile->Release();
			}
			pShellLink->Release();
		}
		CoUninitialize();
	}
	return false;
}

tstring deleteFile(TCHAR const* dst)
{
	try {
		WIN32_FIND_DATA p;
		HANDLE h = FindFirstFile(dst, &p);
		if (h != INVALID_HANDLE_VALUE) {
			FindClose(h);
			bool result = DeleteFile(dst);
			if (!result)
				return _T("操作失败。");
		}
	}
	catch (exception ex)
	{
		return (tstring)toTCHAR(ex.what());
	}
	return _T("");
}


tstring copy_Files(TCHAR const* src, TCHAR const* dst)
{
	try {
		bool result = CopyFile(src, dst, false);  // 会覆盖源文件
		if (!result)
			return (tstring)toTCHAR(u8"安装失败");
	}
	catch (exception ex)
	{
		return (tstring)toTCHAR(ex.what());
	}
	return _T("");
}













const TCHAR* spliceString(TCHAR const* s1, TCHAR const* s2)
{
	string p1 = tochar(s1);
	string p2 = tochar(s2);
	string restul = p1 + p2;

	return toTCHAR(restul.c_str());
}


// 转为 TCHAR* 类型
const TCHAR* toTCHAR(const char* src)
{
#ifdef UNICODE
	int n = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	wchar_t* wp = (wchar_t*)malloc((n + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, src, -1, wp, n);
	return wp;
#else
	int wideSize = MultiByteToWideChar(CP_UTF8, 0, src, -1, nullptr, 0);
	if (wideSize == 0) {
		return "";
	}

	wchar_t* wp = (wchar_t*)malloc((wideSize + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, src, -1, wp, wideSize);
	int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wp, -1, NULL, 0, NULL, NULL);
	if (utf8Size == 0)
		return _T("");

	char* p = (char*)malloc((utf8Size + 1) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, wp, -1, p, utf8Size, nullptr, nullptr);

	return p;
#endif
}

const char* tochar(const char* src)
{
	if (src == NULL)
		return "";

	return src;
}

const char* tochar(const wchar_t* src)
{
	if (src == NULL)
		return "";

	int Msize = WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, 0, NULL, NULL);
	char* p = (char*)malloc((Msize + 1) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, src, -1, p, Msize, NULL, NULL);
	return p;
}

const wchar_t* towchar(const TCHAR* src)
{
#ifdef UNICODE
	return src;
#else
	int n = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	wchar_t* wp = (wchar_t*)malloc((n + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, src, -1, wp, n);
	return wp;
#endif
}

const char* toU8(const TCHAR* str, int len)
{
#ifdef UNICODE
	char* pElementText;
	int    iTextLen;
	iTextLen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	pElementText = (char*)malloc((iTextLen + 1) * sizeof(char));
	if (pElementText == NULL)
		return NULL;

	memset(pElementText, 0, (iTextLen + 1) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, str, -1, pElementText, iTextLen, NULL, NULL);

	return pElementText;
#else
	return str;
#endif
}