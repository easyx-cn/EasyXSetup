#pragma once

#include <windows.h>
#include <list>
#include <Shlobj.h>
#include <direct.h>
#include <regex>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdlib.h>

#include "Common.h"
#include "egroups.h"
#include "vside.h"
#include "reg.h"

using namespace std;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#define VSNUM 10
#define MINGW_SNUM 5

#define OK			1
#define NOTFOUND	0		// 当前目录没有需要的信息，需要继续查找
#define ERROR_1		-1		// 定义为用户选择了与当前vs版本不符的安装目录
#define ERROR_SYSTEM -2

#define VISUAL_STUDIO	0
#define MINGW			1

#define _SEH	L"_seh"
#define _SJLJ	L"_sjlj"
#define _DWARF	L"_dwarf"

#define _UCRT	L"_ucrt"
#define _MSVCRT	L"_msvcrt"

#define _WIN32	L"_win32"
#define _POSIX	L"_posix"

class Page2
{
	nk_context* _ctx;
	int Width, Height;
	LOGFONT bold_lfont;
	EasyXFont ef, bold_ef;
	nk_user_font boldFnot;
	list<VSIDE*> ide_list, exist_list, not_exist_list;
	wstring popup_msg;

	bool popup_active = false;
	EGroups* eGroups[VSNUM];
	EMingWGroups* mingw_Groups[MINGW_SNUM];

	Reg reg;

	int a[2] = { 0, 1 };
	int b1[2] = { 2, 3 };
	int b2[2] = { 4, 5 };
	int b3[2] = { 8, 9 };
	int b4[2] = { 6, 7 };
	int b5[2] = { 10, 11 };

public:
	Page2(nk_context* ctx, int w, int h);
	~Page2() {}

	void InitIDE();

	void UninstallOldEasyX(int id);
	wstring Install(int id);
	wstring Install_mingw(int id);
	wstring UninstallHelp();
	wstring Uninstall(int id);
	wstring Uninstall_mingw(int id);
	wstring InstallHelp();

	wstring deleteFile(wchar_t const* dst);
	wstring copy_Files(wchar_t const* src, wchar_t const* dst);
	const wchar_t* g_pathDesktop();
	wchar_t* help_path();
	bool CreateLinkFile(LPCWSTR urlexePath, LPCWSTR szDestLnkPath);

	nk_style_button disableStyle();
	nk_style_button enableStyle();
	nk_user_font* BoldFont(nk_style* style);

	wstring OpenFolder();

	void Draw(int& running, int& current_page);

	int toVer(DWORD);
	wstring findFolder(wstring path, const wchar_t* folder);
	wstring findFolder(wstring path, wregex rex);
	int exeVersion(wstring clpath);
	int clVersion_2017(wstring p, VSIDE*);
	int AnalysisPath(wstring path, VSIDE* vec, bool repeat = true);
	filesystem::path safe_get_parent(const filesystem::path& filepath);
	void check(EGroups* ep);

	int FindSDK(wstring path, int id, VSIDE* vec, bool g_bX64);
	bool find_file(wstring, wregex);
	int analysis_mingw(wstring p, int, VSIDE*);
	void findCLion_mingw(wstring path);
	void check_mingw(EMingWGroups* ep);
	bool Support(VSIDE* vec, int type, wstring exception, wstring runtime);
	wstring check_mingw_exception(wstring);	// seh  sjlj  dwarf
	//wstring check_mingw_thread(wstring);		// win32 posix
	wstring check_mingw_runtime(wstring);		// MSVCRT  UCRT
	string ReadProcessOutput(const wstring& command);
};