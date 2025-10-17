#pragma once

/* nuklear - 1.32.0 - public domain */
#define _CRT_SECURE_NO_WARNINGS			// �Ƴ� _s ��ȫ��������
#define WIN32_LEAN_AND_MEAN

#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_FIXED_TYPES			// ���������� <stdint.h> �԰��� nuklear ʹ����ȷ����������

// nuklear �����Ŀ������������ƽ̨�����ϼ������ǿ��� nuklear ���Ժ���Ϊ��
// ��ͬƽ̨�����������ʹ�С��һ����ָ�볤�Ȳ�һ��
// ��Щƽ̨(����Ƕ��ʽ)���ṩ IO ���ܣ�����ʹ����ϣ�� nuklear context ʹ�ù̶�������ڴ�ȸ�����������
// �� PC ƽ̨�϶������ϼ����꼸���Ǳ�׼����

#pragma warning(disable:4996)				// nuklear ��Ҫʹ���°汾 VS Ĭ�Ͻ�ֹ�� vsprintf �Ⱦɰ� C ����
#pragma execution_character_set("utf-8")	// VS ��Ҫ�趨ִ���ַ���Ϊ utf8 ��ʽ�������Ӣ���ַ�������


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
	int id;			// ��Ӧ eGroups �±�
	TCHAR const* name;
	TCHAR const* path_1;
	TCHAR const* path_2;
	bool checked;		// �Ƿ�ѡ��װ
	bool installed;		// �Ƿ�װ�ɹ�
	string msg;
	bool exist;

	VSIDE(TCHAR const* n, TCHAR const* p1, TCHAR const* p2, int i, bool e) {
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
	string name;
	int ver;
	string vcpath;
	string path_h;
	string path_libx86;
	string path_libx64;
	int* files_h;
	int* files_libx86;
	int* files_libx64;

	EGroups(string n, int v, string vp, string p_h, string px86, string px64, int fh[2], int* fx86, int* fx64)
	{
		name = n;
		ver = v;
		vcpath = vp;
		path_h = p_h;
		path_libx86 = px86;
		path_libx64 = px64;
		files_h = fh;
		files_libx86 = fx86;
		files_libx64 = fx64;
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
