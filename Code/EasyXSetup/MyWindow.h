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

#pragma warning(disable:4996)				// nuklear 需要使用新版本 VS 默认禁止的 vsprintf 等旧版 C 函数
#pragma execution_character_set("utf-8")	// VS 需要设定执行字符集为 utf8 格式，否则非英文字符会乱码


#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_EASYX_IMPLEMENTATION

#include <windows.h>
#include "Reg.h"
#include "../nuklear.h"
#include "../nuklear_easyx.h"

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
	tstring name;
	tstring path_1;
	tstring path_2;
	bool checked;		// 是否勾选安装
	bool installed;		// 是否安装成功
	string msg;
	bool exist;

	VSIDE(tstring n, tstring p1, tstring p2, int i, bool e) {
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
	string path;
	string name;

	EFiles(string p, string n) { path = p, name = n; }
	~EFiles() {}
};

EFiles* eFiles[] = {
	new EFiles("include\\", "easyx.h"),
	new EFiles("include\\", "graphics.h"),
	new EFiles("lib\\VC6\\x86\\", "EasyXa.lib"),
	new EFiles("lib\\VC6\\x86\\", "EasyXw.lib"),
	new EFiles("lib\\VC2008\\x86\\", "EasyXa.lib"),
	new EFiles("lib\\VC2008\\x86\\", "EasyXw.lib"),
	new EFiles("lib\\VC2008\\x64\\", "EasyXa.lib"),
	new EFiles("lib\\VC2008\\x64\\", "EasyXw.lib"),
	new EFiles("lib\\VC2015\\x86\\", "EasyXa.lib"),
	new EFiles("lib\\VC2015\\x86\\", "EasyXw.lib"),
	new EFiles("lib\\VC2015\\x64\\", "EasyXa.lib"),
	new EFiles("lib\\VC2015\\x64\\", "EasyXw.lib"),
};

class EGroups
{
public:
	tstring name;
	int ver;
	tstring vcpath;
	tstring path_h;
	tstring path_libx86;
	tstring path_libx64;
	int* files_h;
	int* files_libx86;
	int* files_libx64;

	EGroups(tstring n, int v, tstring vp, tstring p_h, tstring px86, tstring px64, int fh[2], int* fx86, int* fx64)
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

tstring Install(int id);
tstring UninstallHelp();
tstring Uninstall(int id);
tstring InstallHelp();
