#include "MyWindow.h"
#include <process.h>
#include <Shlobj.h>
#include <io.h>
#include <fstream>
#include <direct.h>
#include <regex>
#include <conio.h>
#include <list>
#include "easyxver.h"
using namespace std;

int current_page = 1;
int running = 1;
int fontsize = 17;
list<VSIDE*> ide_list, exist_list, not_exist_list;

LOGFONT lfont, bold_lfont;
struct EasyXFont ef, bold_ef;
struct nk_user_font ufont, boldFnot;

#define WIDTH 490
#define HEIGHT 328
#define VSNUM 10

EasyXFont* font;
bool popup_active = false;
string window_title;
tstring popup_msg;
int a[2] = { 0, 1 };
int b1[2] = { 2, 3 };
int b2[2] = { 4, 5 };
int b3[2] = { 8, 9 };
int b4[2] = { 6, 7 };
int b5[2] = { 10, 11 };

EGroups* eGroups[VSNUM] = {
	new EGroups(_T("  Visual C++ 6.0"),6, _T(""),_T(""),_T(""),_T(""), a, b1, NULL),
	new EGroups(_T("  Visual C++ 2008"),2008, _T(""),_T(""),_T(""),_T(""), a,b2,b4),
	new EGroups(_T("  Visual C++ 2010"),2010, _T(""),_T(""),_T(""),_T(""), a,b2,b4),
	new EGroups(_T("  Visual C++ 2012"),2012, _T(""),_T(""),_T(""),_T(""), a,b2,b4),
	new EGroups(_T("  Visual C++ 2013"),2013, _T(""),_T(""),_T(""),_T(""), a,b2,b4),
	new EGroups(_T("  Visual C++ 2015"),2015, _T(""),_T(""),_T(""),_T(""), a,b3,b5),
	new EGroups(_T("  Visual C++ 2017"),2017, _T(""),_T(""),_T(""),_T(""), a,b3,b5),
	new EGroups(_T("  Visual C++ 2019"),2019, _T(""),_T(""),_T(""),_T(""), a,b3,b5),
	new EGroups(_T("  Visual C++ 2022"),2022, _T(""),_T(""),_T(""),_T(""), a,b3,b5),
	new EGroups(_T("  Visual C++ 2026"),2026, _T(""),_T(""),_T(""),_T(""), a,b3,b5)
};

// 主函数
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	initgraph(WIDTH, HEIGHT);

	TCHAR buf[100] = { '\0' };
	_stprintf(buf, _T("%s %s %s"), _T("EasyX"), EASYX_VER, _T("安装向导"));
	window_title = tochar(buf);

	struct nk_context* ctx;
	HWND wnd;
	HDC dc;
	int needs_refresh = 1;
	wnd = GetHWnd();
	dc = GetDC(wnd);
	SetWindowTextW(wnd, towchar(buf));

	font = nk_easyxfont_create("Segoe UI", fontsize);
	ctx = nk_easyx_init(font, dc, WIDTH, HEIGHT);
	MyWindow window(ctx, WIDTH, HEIGHT);

	InitIDE();

	BeginBatchDraw();
	while (running)
	{
		ExMessage msg;
		nk_input_begin(ctx);

		while (peekmessage(&msg)) {
			if (msg.message == WM_QUIT)
				running = 0;

			nk_easyx_handle_event(wnd, &msg);
			needs_refresh = 1;
		}
		nk_input_end(ctx);

		window.Draw();

		nk_easyx_render(nk_rgb(99, 99, 99));
		FlushBatchDraw();

		Sleep(10);
	}

	EndBatchDraw();
	nk_easyxfont_del(font);
	ReleaseDC(wnd, dc);

	closegraph();
	return 0;
}


MyWindow::MyWindow(nk_context* ctx, int w, int h)
{
	_ctx = ctx;
	Width = w;
	Height = h;
}

MyWindow::~MyWindow()
{
}


void InitIDE()
{
	tstring path = _T("桌面路径 ");
	path += g_pathDesktop();
	VSIDE* d = new VSIDE(_T("  EasyX 文档"), _T("在线文档 https://docs.easyx.cn"), path.c_str(), -1, true);
	ide_list.push_back(d);

	bool g_bX64 = false;
	tstring amd = RegRead(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), _T("PROCESSOR_ARCHITECTURE"));

	if (amd != _T("") && _tcscmp(amd.c_str(), _T("AMD64")) == 0)
		g_bX64 = true;

	for (int i = 0; i < VSNUM; i++)
	{
		int v = eGroups[i]->ver;
		tstring name = eGroups[i]->name;
		tstring vcpath = GetVCPath(v, g_bX64);

		if (vcpath == _T(""))
		{
			VSIDE* item = new VSIDE(name.c_str(), _T(""), _T(""), i, false);
			not_exist_list.push_back(item);
			continue;
		}

		eGroups[i]->vcpath = vcpath.c_str();
		eGroups[i]->path_h = GetIncludePath(v);

		if (_taccess((eGroups[i]->vcpath + eGroups[i]->path_h).c_str(), 0) != 0) {
			eGroups[i]->vcpath = _T("");
			eGroups[i]->path_h = _T("");

			VSIDE* item = new VSIDE(name.c_str(), _T(""), _T(""), i, false);
			not_exist_list.push_back(item);
			continue;
		}

		eGroups[i]->path_libx86 = GetLibX86Path(v);
		if (_taccess((eGroups[i]->vcpath + eGroups[i]->path_libx86).c_str(), 0) != 0) {
			eGroups[i]->vcpath = _T("");
			eGroups[i]->path_h = _T("");
			eGroups[i]->path_libx86 = _T("");

			VSIDE* item = new VSIDE(name.c_str(), _T(""), _T(""), i, false);
			not_exist_list.push_back(item);
			continue;
		}

		eGroups[i]->path_libx64 = GetLibX64Path(v);
		if (_taccess((eGroups[i]->vcpath + eGroups[i]->path_libx64).c_str(), 0) != 0) {
			eGroups[i]->path_libx64 = _T("");
		}

		tstring path1 = _T("头文件路径 ") + eGroups[i]->vcpath + eGroups[i]->path_h;
		tstring path2 = _T("库文件路径 ") + eGroups[i]->vcpath + eGroups[i]->path_libx86;

		if (eGroups[i]->path_libx64 != _T("")) {
			path2 += _T(" ") + eGroups[i]->vcpath + eGroups[i]->path_libx64;
		}
		VSIDE* item = new VSIDE(name.c_str(), path1.c_str(), path2.c_str(), i, true);
		exist_list.push_back(item);
	}

	for (list<VSIDE*>::iterator itor = exist_list.begin(); itor != exist_list.end(); itor++)
	{
		ide_list.push_back(*itor);
	}

	for (list<VSIDE*>::iterator itor = not_exist_list.begin(); itor != not_exist_list.end(); itor++)
	{
		ide_list.push_back(*itor);
	}
}


void MyWindow::Draw()
{
	// 取消内边距 padding
	nk_style_window* win = &_ctx->style.window;
	win->group_padding.x = 0;
	win->group_padding.y = 0;
	win->padding.x = 0;
	win->padding.y = 0;

	// 设置滚动条样式
	_ctx->style.scrollv.normal.data.color = nk_rgb(222, 222, 222);
	_ctx->style.scrollv.hover.data.color = nk_rgb(222, 222, 222);
	_ctx->style.scrollv.active.data.color = nk_rgb(222, 222, 222);
	_ctx->style.scrollv.cursor_normal.data.color = nk_rgb(166, 166, 166);
	_ctx->style.scrollv.cursor_hover.data.color = nk_rgb(110, 100, 110);
	_ctx->style.scrollv.cursor_active.data.color = nk_rgb(110, 100, 110);

	switch (current_page)
	{
	case 1:
		P1();
		break;
	case 2:
		P2();
		break;
	}
}

void MyWindow::P1()
{
	_ctx->style.window.background = nk_rgb(77, 77, 77);
	if (nk_begin(_ctx, "", nk_rect(0, 0, WIDTH, HEIGHT), NK_WINDOW_NO_SCROLLBAR))
	{
		float rt[] = { 0.3f, 0.7f };
		float w = (float)(WIDTH * rt[0]);
		float bottom = 45;
		nk_layout_row(_ctx, NK_DYNAMIC, HEIGHT - bottom, 2, rt);

		if (nk_group_begin(_ctx, "group_p1_logo", NK_WINDOW_NO_SCROLLBAR))
		{
			nk_layout_space_begin(_ctx, NK_STATIC, HEIGHT - 100, 2);
			nk_layout_space_push(_ctx, nk_rect(0, 0, w, HEIGHT - bottom));
			nk_fill_rect(&_ctx->current->buffer, nk_rect(0, 0, w, HEIGHT - bottom), 0.0f, nk_rgb(33, 23, 129));

			nk_user_font* f = logoFont(&_ctx->style);
			nk_layout_space_push(_ctx, nk_rect(0, 0, HEIGHT - bottom, w));
			nk_text_style(_ctx, "EasyX", NK_TEXT_ALIGN_LEFT, f, nk_vec2(133, 30), nk_rgb(170, 170, 170));

			nk_layout_space_end(_ctx);
			nk_group_end(_ctx);
		}

		if (nk_group_begin(_ctx, "group_p1_summary", NK_WINDOW_NO_SCROLLBAR))
		{
			char buf[100] = { '\0' };
			sprintf(buf, tochar(_T("欢迎使用 “EasyX %s” 安装向导")), tochar(EASYX_VER));

			nk_fill_rect(&_ctx->current->buffer, nk_rect(w, 0, (float)(WIDTH * rt[1]), HEIGHT - bottom), 0.0f, nk_rgb(245, 245, 239));

			nk_user_font* f = boldFont(&_ctx->style);
			nk_layout_row_dynamic(_ctx, 5, 1);
			nk_layout_row_dynamic(_ctx, 24, 1);
			nk_text_style(_ctx, buf, NK_TEXT_CENTERED, f, nk_vec2(0, 0), nk_rgb(33, 33, 33));
			nk_layout_row_dynamic(_ctx, 15, 1);
			nk_layout_row_dynamic(_ctx, 18, 1);
			nk_label_colored(_ctx, tochar(_T("  EasyX 库为 C/C++ 提供了简单的绘图接口，可以帮助")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_label_colored(_ctx, tochar(_T("  您快速编写图形程序。")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_layout_row_dynamic(_ctx, 7, 1);
			nk_layout_row_dynamic(_ctx, 18, 1);
			nk_label_colored(_ctx, tochar(_T("  这个向导将帮助您安装 EasyX 库到指定开发环境中。")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_label_colored(_ctx, tochar(_T("  EasyX 目前支持 Visual C++ 6 / 2008 ～ 2022。")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_layout_row_dynamic(_ctx, 7, 1);
			nk_layout_row_dynamic(_ctx, 18, 1);
			nk_label_colored(_ctx, tochar(_T("  如果您希望手动安装 EasyX，请参见帮助文件。")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_layout_row_dynamic(_ctx, 7, 1);
			nk_layout_row_dynamic(_ctx, 18, 1);
			nk_label_colored(_ctx, tochar(_T("  单击 [下一步] 继续。")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_group_end(_ctx);
		}

		nk_fill_rect(&_ctx->current->buffer, nk_rect(0, 283, WIDTH, 1), 0.0f, nk_rgb(100, 100, 100));
		nk_fill_rect(&_ctx->current->buffer, nk_rect(0, 284, WIDTH, bottom), 0.0f, nk_rgb(235, 235, 209));

		nk_style_button btn = enableStyle();
		nk_layout_space_begin(_ctx, NK_STATIC, 10, 2);
		nk_layout_space_push(_ctx, nk_rect(WIDTH - 200, 7, 80, 25));
		if (nk_button_label_styled(_ctx, &btn, tochar(_T("下一步 >"))))
			current_page = 2;

		nk_layout_space_push(_ctx, nk_rect(WIDTH - 100, 7, 80, 25));
		if (nk_button_label_styled(_ctx, &btn, tochar(_T("关闭"))))
			running = false;

		nk_layout_space_end(_ctx);
	}
	nk_end(_ctx);
}


void MyWindow::P2()
{
	if (nk_begin(_ctx, "", nk_rect(0, 0, (float)Width, (float)Height), NK_WINDOW_NO_SCROLLBAR))
	{
		nk_fill_rect(&_ctx->current->buffer, nk_rect(0, 0, WIDTH, 45), 0.0f, nk_rgb(250, 250, 250));
		nk_fill_rect(&_ctx->current->buffer, nk_rect(0, 45, WIDTH, HEIGHT), 0.0f, nk_rgb(235, 235, 209));

		nk_layout_row_dynamic(_ctx, 16, 1);
		nk_user_font* f = boldFont(&_ctx->style);
		nk_text_style(_ctx, tochar(_T("  执行安装")), NK_TEXT_LEFT, f, nk_vec2(0, 0), nk_rgb(33, 33, 33));
		nk_layout_row_dynamic(_ctx, 1, 1);
		nk_layout_row_dynamic(_ctx, 16, 1);
		nk_label_colored(_ctx, tochar(_T("        请针对您所用的开发平台，选择安装或卸载。")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
		nk_layout_row_dynamic(_ctx, 8, 1);
		nk_layout_row_dynamic(_ctx, 16, 1);
		nk_label_colored(_ctx, tochar(_T("  在下面中找到您所用的开发平台，点击右侧的“安装”将 EasyX 库安装至")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
		nk_label_colored(_ctx, tochar(_T("  该开发平台。或点击右侧的“卸载”将 EasyX 库从该开发平台移除。")), NK_TEXT_LEFT, nk_rgb(33, 33, 33));

		float rt[] = { 1.0f };
		nk_style_button btn = enableStyle();
		nk_style_button button = disableStyle();
		nk_layout_row_dynamic(_ctx, 6, 1);
		nk_layout_row(_ctx, NK_DYNAMIC, HEIGHT - 160, 1, rt);

		_ctx->style.window.fixed_background.data.color = nk_rgb(235, 235, 209);

		if (nk_group_begin(_ctx, "group_p2_list", 0)) {		// group_p2_list 不能与其它 nk_group_begin 相同，否则会串滚
			int i = 0, h = 26;
			int cur_height = 10;

			int s_h = textheight(_T("天"));

			for (list<VSIDE*>::iterator itor = ide_list.begin(); itor != ide_list.end(); itor++)
			{
				nk_layout_space_begin(_ctx, NK_STATIC, 35, 5);
				nk_layout_space_push(_ctx, nk_rect(0, 0, 110, 30));
				nk_label_colored(_ctx, toU8((*itor)->name.c_str()), NK_TEXT_LEFT, nk_rgb(33, 33, 33));

				if ((*itor)->exist == true)
				{
					nk_layout_space_push(_ctx, nk_rect(110, 0, 230, 13));
					nk_label_colored(_ctx, toU8((*itor)->path_1.c_str()), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
					struct nk_rect bounds = nk_widget_bounds(_ctx);

					// 鼠标悬浮 tooltip
					if (!popup_active && nk_input_is_mouse_hovering_rect(&_ctx->input, bounds)) {
						nk_tooltip_begin(_ctx, 300.0f);
						const char* str = toU8((*itor)->path_1.c_str());
						int len = strlen(str);
						int s_w = textwidth(toTCHAR(str));
						float line = s_w / 300.0f + 1;
						nk_layout_row_static(_ctx, line * s_h, 300, 1);
						nk_label_colored_wrap(_ctx, str, nk_rgb(233, 233, 233));
						nk_tooltip_end(_ctx);
					}

					nk_layout_space_push(_ctx, nk_rect(110, 16, 230, 13));
					nk_label_colored(_ctx, toU8((*itor)->path_2.c_str()), NK_TEXT_LEFT, nk_rgb(33, 33, 33));
					bounds = nk_widget_bounds(_ctx);

					// 鼠标悬浮 tooltip
					if (!popup_active && nk_input_is_mouse_hovering_rect(&_ctx->input, bounds))
					{
						nk_tooltip_begin(_ctx, 300.0f);
						const char* str = toU8((*itor)->path_2.c_str());
						int len = strlen(str);
						int s_w = textwidth(toTCHAR(str));
						float line = s_w / 300.0f + 1;
						nk_layout_row_static(_ctx, line * s_h, 300, 1);
						nk_label_colored_wrap(_ctx, str, nk_rgb(233, 233, 233));
						nk_tooltip_end(_ctx);
					}

					nk_layout_space_push(_ctx, nk_rect(WIDTH - 135, 0, 45, 30));
					if (nk_button_label_styled(_ctx, &btn, tochar(_T("安装"))))
					{
						tstring s;
						if ((*itor)->id == -1)
							s = InstallHelp();
						else
							s = Install((*itor)->id);

						if (s.size() == 0)
							popup_msg = _T("安装成功");
						else
							popup_msg = s;

						popup_active = true;
					}

					nk_layout_space_push(_ctx, nk_rect(WIDTH - 85, 0, 45, 30));
					if (nk_button_label_styled(_ctx, &btn, tochar(_T("卸载"))))
					{
						tstring s;
						if ((*itor)->id == -1)
							s = UninstallHelp();
						else
							s = Uninstall((*itor)->id);

						popup_active = true;
						popup_msg = s;
					}
				}
				else	// 灰色按钮
				{
					nk_layout_space_push(_ctx, nk_rect(110, 0, 230, 30));
					nk_label(_ctx, tochar(_T("(未检测到)")), NK_TEXT_LEFT);

					nk_layout_space_push(_ctx, nk_rect(WIDTH - 135, 0, 45, 30));
					nk_button_label_styled(_ctx, &button, tochar(_T("安装")));

					nk_layout_space_push(_ctx, nk_rect(WIDTH - 85, 0, 45, 30));
					nk_button_label_styled(_ctx, &button, tochar(_T("卸载")));
				}

				i++;
				nk_layout_space_end(_ctx);
			}
			nk_group_end(_ctx);
		}

		// 绘制 tooltip
		if (popup_active)
		{
			static struct nk_rect s = { 120, 100, 220, 80 };
			if (nk_popup_begin(_ctx, NK_POPUP_STATIC, "", NK_WINDOW_NO_SCROLLBAR, s))
			{
				nk_layout_row_dynamic(_ctx, 5, 1);
				nk_layout_row_dynamic(_ctx, 16, 1);
				nk_label_colored(_ctx, tochar(popup_msg.c_str()), NK_TEXT_CENTERED, nk_rgb(33, 33, 33));
				nk_layout_row_dynamic(_ctx, 10, 1);
				nk_layout_row_dynamic(_ctx, 25, 1);
				if (nk_button_label_styled(_ctx, &btn, "OK")) {
					popup_active = false;
					nk_popup_close(_ctx);
				}
				nk_popup_end(_ctx);
			}
			else popup_active = false;
		}

		nk_layout_row_dynamic(_ctx, 6, 1);
		nk_label_colored(_ctx, tochar(_T("EasyX Install System v5.0 —————————————————————————————")), NK_TEXT_LEFT, nk_rgb(123, 123, 123));
		nk_layout_row_dynamic(_ctx, 1, 1);

		nk_layout_space_begin(_ctx, NK_STATIC, 25, 2);
		nk_layout_space_push(_ctx, nk_rect(WIDTH - 200, 0, 80, 25));
		if (nk_button_label_styled(_ctx, &btn, tochar(_T("< 上一步"))))
			current_page = 1;

		nk_layout_space_push(_ctx, nk_rect(WIDTH - 100, 0, 80, 25));
		if (nk_button_label_styled(_ctx, &btn, tochar(_T("取消"))))
			running = false;
		nk_layout_space_end(_ctx);
	}
	nk_end(_ctx);
}

nk_style_button MyWindow::disableStyle()
{
	nk_style_button button = _ctx->style.button;
	button.normal = nk_style_item_color(nk_rgb(222, 222, 222));
	button.hover = nk_style_item_color(nk_rgb(222, 222, 222));
	button.active = nk_style_item_color(nk_rgb(222, 222, 222));
	button.text_normal = nk_rgb(166, 166, 166);
	button.text_active = nk_rgb(166, 166, 166);
	button.text_hover = nk_rgb(166, 166, 166);
	button.text_alignment = NK_TEXT_CENTERED;

	return button;
}

nk_style_button MyWindow::enableStyle()
{
	nk_style_button button = _ctx->style.button;
	button.normal = nk_style_item_color(nk_rgb(222, 222, 222));
	button.hover = nk_style_item_color(nk_rgb(255, 255, 255));
	button.active = nk_style_item_color(nk_rgb(255, 255, 255));
	button.text_normal = nk_rgb(66, 66, 66);
	button.text_active = nk_rgb(88, 88, 88);
	button.text_hover = nk_rgb(88, 88, 88);

	return button;
}

// 自定义文本样式函数
void nk_text_style(struct nk_context* ctx, const char* str,
	nk_flags alignment, const struct nk_user_font* f, struct nk_vec2 offset, nk_color col)
{
	struct nk_window* win;
	const struct nk_style* style;

	struct nk_vec2 item_padding;
	struct nk_rect bounds;
	struct nk_text text;

	NK_ASSERT(ctx);
	NK_ASSERT(ctx->current);
	NK_ASSERT(ctx->current->layout);
	if (!ctx || !ctx->current || !ctx->current->layout) return;

	win = ctx->current;
	style = &ctx->style;
	nk_panel_alloc_space(&bounds, ctx);
	item_padding = style->text.padding;

	bounds.x += offset.x;
	bounds.y += offset.y;

	text.padding.x = item_padding.x;
	text.padding.y = item_padding.y;
	text.background = style->window.background;	// 已经设置 透明模式，字体背景无效
	text.text = col;
	nk_widget_text(&win->buffer, bounds, str, nk_strlen(str), &text, alignment, f);
}

void setFont(nk_user_font* ufont, EasyXFont* ef, LOGFONT* lf, nk_style* style, int h, int weight, LONG escap, LONG orien)
{
	ufont->height = style->font->height;
	ufont->width = style->font->width;

	lf->lfHeight = h;
	lf->lfWidth = 0;
	lf->lfEscapement = escap;
	lf->lfOrientation = orien;
	lf->lfWeight = weight;
	lf->lfItalic = 0;
	lf->lfUnderline = 0;
	lf->lfStrikeOut = 0;
	lf->lfCharSet = 1;
	lf->lfOutPrecision = OUT_TT_ONLY_PRECIS;
	lf->lfClipPrecision = 0;
	lf->lfQuality = PROOF_QUALITY;
	lf->lfPitchAndFamily = 0;
	_tcscpy(lf->lfFaceName, _T("Segoe UI"));

	ef->handle = lf;
	ufont->userdata.ptr = (void*)ef;
}

nk_user_font* logoFont(nk_style* style)
{
	setFont(&ufont, &ef, &lfont, style, 80, 0, -900, -900);
	return &ufont;
}

nk_user_font* boldFont(nk_style* style)
{
	setFont(&boldFnot, &bold_ef, &bold_lfont, style, 22, 900);
	return &boldFnot;
}









void UninstallOldEasyX(int id) {
	int len_ = eGroups[id]->ver;
	if (eGroups[id]->ver == 2017 || eGroups[id]->ver == 2019) {
		string oldpath = tochar(eGroups[id]->vcpath.c_str());
		if (oldpath.length() > 13 && oldpath.substr(oldpath.length() - 14, 14) == "\\Auxiliary\\VS\\") {
			oldpath = oldpath.substr(0, oldpath.length() - 13);
			string verfile = oldpath + "Auxiliary\\Build\\Microsoft.VCToolsVersion.default.txt";
			char const* cver = verfile.c_str();
			if (_access(cver, 0) == 0) {  // 如果文件存在
				ifstream fin(cver, ios::in);
				string str = "";
				getline(fin, str);
				fin.close();

				oldpath += "Tools\\MSVC\\" + str + "\\";

				string f;
				if (eGroups[id]->path_h != _T("")) {
					for (int i = 0; i < 2; i++) {
						f = oldpath + tochar(eGroups[id]->path_h.c_str()) + tochar(eFiles[eGroups[id]->files_h[i]]->name.c_str());
						deleteFile(toTCHAR(f.c_str()));
					}
				}

				if (eGroups[id]->path_libx86 != _T("")) {
					for (int i = 0; i < 2; i++) {
						f = oldpath + tochar(eGroups[id]->path_libx86.c_str()) + tochar(eFiles[eGroups[id]->files_libx86[i]]->name.c_str());
						deleteFile(toTCHAR(f.c_str()));
					}
				}

				if (eGroups[id]->path_libx64 != _T("")) {
					for (int i = 0; i < 2; i++) {
						f = oldpath + tochar(eGroups[id]->path_libx64.c_str()) + tochar(eFiles[eGroups[id]->files_libx64[i]]->name.c_str());
						deleteFile(toTCHAR(f.c_str()));
					}
				}
			}
		}
	}
}

tstring Install(int id)
{
	if (eGroups[id]->ver == 2017 || eGroups[id]->ver == 2019) {
		UninstallOldEasyX(id);
	}

	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	string strFullPath = tochar(exeFullPath);
	size_t nStart = strFullPath.find_last_of("\\");
	string strExeName = strFullPath.substr(0, nStart + 1);

	tstring err = _T("");
	string e = "";
	if (eGroups[id]->files_h != NULL) {
		for (int i = 0; i < 2; i++) {
			string src = tochar(eFiles[eGroups[id]->files_h[i]]->path.c_str()) + eFiles[eGroups[id]->files_h[i]]->name;
			string dst = tochar(eGroups[id]->vcpath.c_str()) + e + tochar(eGroups[id]->path_h.c_str()) + eFiles[eGroups[id]->files_h[i]]->name;
			err += copy_Files(toTCHAR((strExeName + src).c_str()), toTCHAR(dst.c_str()));

			if (err != _T(""))
				return _T("安装失败");
		}
	}
	if (eGroups[id]->files_libx86 != NULL) {
		for (int i = 0; i < 2; i++) {
			string src = tochar(eFiles[eGroups[id]->files_libx86[i]]->path.c_str()) + eFiles[eGroups[id]->files_libx86[i]]->name;
			string dst = tochar(eGroups[id]->vcpath.c_str()) + e + tochar(eGroups[id]->path_libx86.c_str()) + eFiles[eGroups[id]->files_libx86[i]]->name;
			err += copy_Files(toTCHAR((strExeName + src).c_str()), toTCHAR(dst.c_str()));
			if (err != _T(""))
				return _T("安装失败");
		}
	}
	if (eGroups[id]->path_libx64 != _T("") && eGroups[id]->files_libx64 != NULL) {
		for (int i = 0; i < 2; i++) {
			string src = tochar(eFiles[eGroups[id]->files_libx64[i]]->path.c_str()) + eFiles[eGroups[id]->files_libx64[i]]->name;
			string dst = tochar(eGroups[id]->vcpath.c_str()) + e + tochar(eGroups[id]->path_libx64.c_str()) + eFiles[eGroups[id]->files_libx64[i]]->name;
			err += copy_Files(toTCHAR((strExeName + src).c_str()), toTCHAR(dst.c_str()));
			if (err != _T(""))
				return _T("安装失败");
		}
	}

	return err;
}

tstring Uninstall(int id) {
	if (eGroups[id]->ver == 2017 || eGroups[id]->ver == 2019) {
		UninstallOldEasyX(id);
	}

	bool result = false;
	tstring err = _T("卸载失败");
	string f, e = "";
	if (eGroups[id]->path_h != _T("")) {
		for (int i = 0; i < 2; i++) {
			f = tochar(eGroups[id]->vcpath.c_str())  + e + tochar(eGroups[id]->path_h.c_str()) + eFiles[eGroups[id]->files_h[i]]->name;
			result = DeleteFile(toTCHAR(f.c_str()));
			if (!result)
				return err;
		}
	}

	if (eGroups[id]->path_libx86 != _T("")) {
		for (int i = 0; i < 2; i++) {
			f = tochar(eGroups[id]->vcpath.c_str()) + e + tochar(eGroups[id]->path_libx86.c_str()) + eFiles[eGroups[id]->files_libx86[i]]->name;
			result = DeleteFile(toTCHAR(f.c_str()));
			if (!result)
				return err;
		}
	}

	if (eGroups[id]->path_libx64 != _T("")) {
		for (int i = 0; i < 2; i++) {
			f = tochar(eGroups[id]->vcpath.c_str()) + e + tochar(eGroups[id]->path_libx64.c_str()) + eFiles[eGroups[id]->files_libx64[i]]->name;
			result = DeleteFile(toTCHAR(f.c_str()));
			if (!result)
				return err;
		}
	}

	if (!result)
		return err;
	else
		return _T("卸载成功");
}

tstring UninstallHelp() {
	tstring err = deleteFile(help_path());

	if (err == _T(""))
		return _T("卸载成功");
	else
		return _T("卸载失败");
}
tstring InstallHelp() {
	bool result = CreateLinkFile(TEXT("https://docs.easyx.cn"), help_path());
	if (!result)
		return _T("EasyX 文档安装失败。");

	return _T("");
}