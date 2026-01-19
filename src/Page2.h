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

#define NOTFOUND 0		// 当前目录没有需要的信息，需要继续查找
#define ERROR_1	-1		// 定义为用户选择了与当前vs版本不符的安装目录
#define ERROR_SYSTEM -2


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
	wstring UninstallHelp();
	wstring Uninstall(int id);
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
	int clVersion(wstring clpath);
	int clVersion_2017(wstring p, int id);
	int AnalysisPath(wstring path, int id, bool repeat = true);
	filesystem::path safe_get_parent(const filesystem::path& filepath);
	void check(EGroups* ep);

	int FindCLion(wstring path, int id, bool repeat = true);
	bool findCLion_exe(wstring);
	void findCLion_mingw(wstring path);
	void check_mingw(EMingWGroups* ep);
	wstring ReadProcessOutput(const wstring& command);
};