#pragma once
#include <windows.h>
#include <string>

using namespace std;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383


// 数值必须与数组 mingw_Groups 符合，不能超出范围
#define DEVCPP		0
#define CODEBLOCKS	1
#define CLION		2
#define VSCODE		3
#define CFREE		4
#define OTHER_IDE	5

class Reg
{
public:
	wstring GetMingWPath(int identity, bool g_bx64);
	wstring GetVCPath(int iVsVer, bool);
	wchar_t const* GetIncludePath(int iVsVer);
	wchar_t const* GetLibX86Path(int iVcVer);
	wchar_t const* GetLibX64Path(int iVcVer);

	//遍历路劲下所有子项，只返回子项名称
	wchar_t** RegEnumKeys(HKEY defkey, wchar_t const* subkeyname, int* numkeys);

	// 读取某个键值对的值
	wstring RegRead(HKEY defkey, wchar_t const* subkeyname, wchar_t const* key);
};