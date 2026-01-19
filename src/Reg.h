#pragma once
#include <windows.h>
#include <string>

using namespace std;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383


#define DEVCPP		100
#define CODEBLOCKS	101
#define CLION		102
#define VSCODE		103
#define OTHER_IDE	104

class Reg
{
public:
	wstring GetVCPath(int iVsVer, bool);
	wchar_t const* GetIncludePath(int iVsVer);
	wchar_t const* GetLibX86Path(int iVcVer);
	wchar_t const* GetLibX64Path(int iVcVer);

	//遍历路劲下所有子项，只返回子项名称
	wchar_t** RegEnumKeys(HKEY defkey, wchar_t const* subkeyname, int* numkeys);

	// 读取某个键值对的值
	wstring RegRead(HKEY defkey, wchar_t const* subkeyname, wchar_t const* key);
};