#include "Reg.h"
#include <Shlobj.h>
#include <direct.h>
#include <regex>
#include <tchar.h>


using namespace std;


wchar_t** Reg::RegEnumKeys(HKEY defkey, wchar_t const* subkeyname, int* numkeys)
{
	wchar_t** list = NULL;
	HKEY hKey;
	if (RegOpenKeyExW(defkey, subkeyname, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return list;

	wchar_t  achKey[MAX_KEY_LENGTH];			// buffer for subkey name
	DWORD    cbNameLength;						// size of name string 
	wchar_t  achClass[MAX_PATH] = L"";		// buffer for class name 
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

	retCode = RegQueryInfoKeyW(
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
		list = new wchar_t* [cSubKeys];
		for (i = 0; i < cSubKeys; i++)
		{
			cbNameLength = MAX_KEY_LENGTH;
			retCode = RegEnumKeyExW(hKey, i, achKey, &cbNameLength, NULL, NULL, NULL, &ftLastWriteTime);

			if (cSubKeys == i + 1)
			{
				int i = 0;
			}
			if (retCode == ERROR_SUCCESS)
			{
				list[i] = new wchar_t[cbNameLength + 1];
				wcscpy_s(list[i], cbNameLength + 1, achKey);
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
wstring Reg::RegRead(HKEY defkey, wchar_t const* subkeyname, wchar_t const* key)
{
	if (key == NULL)
		return L"";

	wchar_t* result = NULL;
	HKEY hKey;
	DWORD dataType;
	wchar_t data[1024] = { 0 };
	DWORD dataSize = sizeof(data);

	if (defkey == HKEY_CURRENT_USER)
	{
		int keyNum;
		wchar_t** aNames = RegEnumKeys(HKEY_USERS, NULL, &keyNum);

		for (int i = 0; i < keyNum; i++)
		{
			int len = lstrlenW(aNames[i]);
			if (len < 2)
				continue;

			wchar_t const* s1 = L"S-";
			wchar_t* sub_1 = new wchar_t[3];
			wcsncpy_s(sub_1, 3, aNames[i], 2);
			sub_1[2] = L'\0';

			if (wcscmp(sub_1, s1) != 0)
				continue;

			if (len > 8) {
				wchar_t const* s2 = L"_Classes";
				wchar_t* sub_2 = new wchar_t[9];
				wcsncpy_s(sub_2, 9, aNames[i] + len - 8, 8);   // 截取后面 8 个字符
				sub_2[8] = L'\0';
				if (wcscmp(sub_2, s2) == 0)
					continue;
			}

			int buf_len = len + 1 + lstrlenW(subkeyname) + 1;
			wchar_t* buf = new wchar_t[buf_len];
			_snwprintf_s(buf, buf_len, buf_len, L"%s%s%s", aNames[i], L"\\", subkeyname);
			buf[buf_len - 1] = L'\0';

			int re = RegOpenKeyExW(HKEY_USERS, buf, NULL, KEY_READ, &hKey);
			delete buf;
			if (re != ERROR_SUCCESS)
				continue;

			if (RegQueryValueExW(hKey, key, NULL, &dataType, (LPBYTE)data, &dataSize) != ERROR_SUCCESS)
			{
				RegCloseKey(hKey);
				return L"";
			}

			RegCloseKey(hKey);
		}

		wstring str = data;
		return str;
	}

	if (RegOpenKeyExW(defkey, subkeyname, NULL, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"";
	}

	if (RegQueryValueExW(hKey, key, NULL, &dataType, (LPBYTE)data, &dataSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"";
	}

	RegCloseKey(hKey);

	wstring str = data;
	return str;
}

wstring Reg::GetVCPath(int iVsVer, bool g_bX64)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	wchar_t const* path = NULL;
	wchar_t const* subkeyname = NULL;
	wchar_t const* key = NULL;

	switch (iVsVer)
	{
	case 6:
		hKey = HKEY_CURRENT_USER;
		subkeyname = L"Software\\Microsoft\\DevStudio\\6.0\\Build System\\Components\\Platforms\\Win32 (x86)\\Directories";
		key = L"Library Dirs";
		break;

	case 2008:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7";
		else
			subkeyname = L"Software\\Microsoft\\VisualStudio\\SxS\\VC7";
		key = L"9.0";
		break;

	case 2010:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7";
		else
			subkeyname = L"Software\\Microsoft\\VisualStudio\\SxS\\VC7";
		key = L"10.0";
		break;

	case 2012:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7";
		else
			subkeyname = L"Software\\Microsoft\\VisualStudio\\SxS\\VC7";
		key = L"11.0";
		break;

	case 2013:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7";
		else
			subkeyname = L"Software\\Microsoft\\VisualStudio\\SxS\\VC7";
		key = L"12.0";
		break;

	case 2015:
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VC7";
		else
			subkeyname = L"Software\\Microsoft\\VisualStudio\\SxS\\VC7";
		key = L"14.0";
		break;

	case 2017:
	{
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
		else
			subkeyname = L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

		int numKeys;
		int len = lstrlenW(subkeyname);
		wchar_t** aNames = RegEnumKeys(hKey, subkeyname, &numKeys);
		for (int i = 0; i < numKeys; i++)
		{
			int buf_len = len + 1 + lstrlenW(aNames[i]) + 1;
			wchar_t* buf = new wchar_t[buf_len];
			_snwprintf_s(buf, buf_len, buf_len, L"%s%s%s", subkeyname, L"\\", aNames[i]);
			buf[buf_len - 1] = L'\0';

			wstring rtn = RegRead(hKey, buf, L"DisplayName");
			if (rtn != L"")
			{
				wregex rex(L"^Visual Studio (Community|Professional|Enterprise) 2017(\\([^\\)]+\\))?$", regex_constants::icase);  //   \\( 代替原来的 \(      匹配括号
				bool result = regex_search(rtn, rex);
				if (result)
				{
					subkeyname = buf;
					key = L"InstallLocation";
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
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
		else
			subkeyname = L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

		int numKeys;
		int len = lstrlenW(subkeyname);
		wchar_t** aNames = RegEnumKeys(hKey, subkeyname, &numKeys);
		for (int i = 0; i < numKeys; i++)
		{
			int buf_len = len + 1 + lstrlenW(aNames[i]) + 1;
			wchar_t* buf = new wchar_t[buf_len];
			_snwprintf_s(buf, buf_len, buf_len, L"%s%s%s", subkeyname, L"\\", aNames[i]);
			buf[buf_len - 1] = L'\0';

			wstring rtn = RegRead(hKey, buf, L"DisplayName");
			if (rtn != L"") {
				wregex rex(L"^Visual Studio (Community|Professional|Enterprise) 2019( \\([^\\)]+\\))?$", regex_constants::icase);  //   \\( 代替原来的 \(      匹配括号
				bool result = regex_search(rtn, rex);
				if (result)
				{
					subkeyname = buf;
					key = L"InstallLocation";
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
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
		else
			subkeyname = L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

		int numKeys;
		int len = lstrlenW(subkeyname);
		wchar_t** aNames = RegEnumKeys(hKey, subkeyname, &numKeys);
		for (int i = 0; i < numKeys; i++)
		{
			int buf_len = len + 1 + lstrlenW(aNames[i]) + 1;
			wchar_t* buf = new wchar_t[buf_len];
			_snwprintf_s(buf, buf_len, buf_len, L"%s%s%s", subkeyname, L"\\", aNames[i]);
			buf[buf_len - 1] = L'\0';

			wstring rtn = RegRead(hKey, buf, L"DisplayName");
			if (rtn != L"") {
				wregex rex(L"^Visual Studio (Community|Professional|Enterprise) 2022( \\([^\\)]+\\))?$", regex_constants::icase);  //   \\( 代替原来的 \(      匹配括号
				bool result = regex_search(rtn, rex);
				if (result)
				{
					subkeyname = buf;
					key = L"InstallLocation";
					break;
				}
			}
		}
		break;
	}

	case 2026:
	{
		hKey = HKEY_LOCAL_MACHINE;
		if (g_bX64)
			subkeyname = L"Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
		else
			subkeyname = L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

		int numKeys;
		int len = lstrlenW(subkeyname);
		wchar_t** aNames = RegEnumKeys(hKey, subkeyname, &numKeys);
		for (int i = 0; i < numKeys; i++)
		{
			int buf_len = len + 1 + lstrlenW(aNames[i]) + 1;
			wchar_t* buf = new wchar_t[buf_len];
			_snwprintf_s(buf, buf_len, buf_len, L"%s%s%s", subkeyname, L"\\", aNames[i]);
			buf[buf_len - 1] = L'\0';

			wstring rtn = RegRead(hKey, buf, L"DisplayName");
			if (rtn != L"") {
				wregex rex(L"^Visual Studio (Community|Professional|Enterprise) 2026( \\([^\\)]+\\))?$", regex_constants::icase);  //   \\( 代替原来的 \(      匹配括号
				bool result = regex_search(rtn, rex);
				if (result)
				{
					subkeyname = buf;
					key = L"InstallLocation";
					break;
				}
			}
		}
		break;
	}
	}

	wstring localPath = RegRead(hKey, subkeyname, key);
	switch (iVsVer)
	{
	case 6:
		if (localPath != L"") {
			wregex rex(L"[^;]+\\\\(?=vc98\\\\mfc\\\\lib$|vc98\\\\mfc\\\\lib;)", regex_constants::icase);
			wsmatch what;
			bool result = regex_search(localPath, what, rex);
			if (result)
			{
				size_t len = lstrlenW(what[0].str().c_str()) + lstrlenW(L"VC98\\") + 1;
				wchar_t* buf = new wchar_t[len];
				_snwprintf_s(buf, len, len, L"%s%s", what[0].str().c_str(), L"VC98\\");
				localPath = buf;
			}
		}
		break;
	case 2017:
	case 2019:
	case 2022:
	case 2026:
		if (localPath != L"") {
			int len = lstrlenW(localPath.c_str());
			if (localPath[len - 1] != L'\\')
				localPath = localPath + L"\\VC\\Auxiliary\\VS\\";
			else
				localPath = localPath + L"VC\\Auxiliary\\VS\\";
		}
		break;
	default: break;
	}

	if (localPath != L"") {
		DWORD dir = GetFileAttributesW(localPath.c_str());
		if (dir != FILE_ATTRIBUTE_DIRECTORY)
			localPath = L"";
	}

	if (localPath != L"") {
		int len = lstrlenW(localPath.c_str());
		if (localPath[len - 1] != L'\\')
			localPath = localPath + L"\\";
	}

	return localPath;
}

wchar_t const* Reg::GetIncludePath(int iVsVer) {
	if (iVsVer == 6)
		return L"Include\\";

	return L"include\\";
}

wchar_t const* Reg::GetLibX86Path(int iVcVer) {
	switch (iVcVer) {
	case 6:     return L"Lib\\";
	case 2008:
	case 2010:
	case 2012:
	case 2013:
	case 2015: return L"lib\\";
	case 2017:
	case 2019:
	case 2022:
	case 2026: return L"lib\\x86\\";
	}

	return L"";
}

wchar_t const* Reg::GetLibX64Path(int iVcVer) {
	switch (iVcVer) {
	case 6:     return L"";
	case 2008:
	case 2010:
	case 2012:
	case 2013:
	case 2015: return L"lib\\amd64\\";
	case 2017:
	case 2019:
	case 2022:
	case 2026: return L"lib\\x64\\";
	}
	return L"";
}