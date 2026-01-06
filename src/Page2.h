#pragma once

#include <windows.h>
#include <list>
#include <Shlobj.h>
#include <direct.h>
#include <regex>
#include <tchar.h>
#include <fstream>
#include <list>

#include "Common.h"
#include "egroups.h"
#include "vside.h"
#include "reg.h"
using namespace std;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#define VSNUM 10


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

	void Draw(int& running, int& current_page);
};