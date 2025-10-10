#pragma once

/* nuklear - 1.32.0 - public domain */
#define _CRT_SECURE_NO_WARNINGS			// 移除 _s 安全函数警告
#define WIN32_LEAN_AND_MEAN

#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_FIXED_TYPES			// 这个宏会引入 <stdint.h> 以帮助 nuklear 使用正确的数据类型

// nuklear 的设计目标是适配所有平台，以上几个宏是控制 nuklear 特性和行为的
// 不同平台基本数据类型大小不一样，指针长度不一样
// 有些平台(例如嵌入式)不提供 IO 功能，或者使用者希望 nuklear context 使用固定分配的内存等各种特殊需求
// 在 PC 平台上定义以上几个宏几乎是标准动作


#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_EASYX_IMPLEMENTATION

#include <windows.h>
#include "lib/nuklear/nuklear.h"
#include "lib/nuklear/nuklear_easyx.h"
#include "Reg.h"

#include <string>
#include <map>
using namespace std;


class MyWindow
{
	nk_context* _ctx;
	int Width, Height;

private:
	nk_style_button disableStyle();
	nk_style_button enableStyle();
	void P1();
	void P2();

public:
	MyWindow(nk_context*, int, int);
	~MyWindow();

	void Draw();
};



class VSIDE
{
public:
	int id;			// 对应 eGroups 下标
	wstring name;
	wstring path_1;
	wstring path_2;
	bool checked;		// 是否勾选安装
	bool installed;		// 是否安装成功
	string msg;
	bool exist;

	VSIDE(wstring n, wstring p1, wstring p2, int i, bool e) {
		name = n;
		path_1 = p1;
		path_2 = p2;
		id = i;
		checked = false;
		installed = false;
		exist = e;
	}
	~VSIDE() {}
};





class EFiles
{
public:
	wstring path;
	wstring name;

	EFiles(wstring p, wstring n) { path = p, name = n; }
	~EFiles() {}
};

EFiles* eFiles[] = {
	new EFiles(L"include\\", L"easyx.h"),
	new EFiles(L"include\\", L"graphics.h"),
	new EFiles(L"lib\\VC6\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC6\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib\\VC2008\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC2008\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib\\VC2008\\x64\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC2008\\x64\\", L"EasyXw.lib"),
	new EFiles(L"lib\\VC2015\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC2015\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib\\VC2015\\x64\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC2015\\x64\\", L"EasyXw.lib"),
};

class EGroups
{
public:
	wstring name;
	int ver;
	wstring vcpath;
	wstring path_h;
	wstring path_libx86;
	wstring path_libx64;
	int* files_h;
	int* files_libx86;
	int* files_libx64;

	EGroups(wstring n, int v, wstring vp, wstring p_h, wstring px86, wstring px64, int fh[2], int* fx86, int* fx64)
	{
		name = n;
		ver = v;
		vcpath = vp;
		path_h = p_h;
		path_libx86 = px86;
		path_libx64 = px64;
		files_h = fh;
		files_libx86 = fx86;
		files_libx64 = fx64;	// 对应全局变量 eFiles 的下标，决定使用哪两个 lib 文件
	}
	~EGroups() {}
};


void InitIDE();
nk_user_font* logoFont(nk_style* style);
nk_user_font* boldFont(nk_style* style);
void setFont(nk_user_font* ufont, EasyXFont* ef, LOGFONT* lf, nk_style* style, int h, int weight = 0, LONG escap = 0, LONG orien = 0);
void nk_text_style(struct nk_context* ctx, const char* str, nk_flags alignment, const struct nk_user_font* f, struct nk_vec2 offset, nk_color col);

wstring Install(int id);
wstring UninstallHelp();
wstring Uninstall(int id);
wstring InstallHelp();
