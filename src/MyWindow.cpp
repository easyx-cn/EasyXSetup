#include "MyWindow.h"
#include <process.h>
#include <Shlobj.h>
#include <io.h>
#include <fstream>
#include <direct.h>
#include <regex>
#include <conio.h>
#include <list>
using namespace std;

#define WIDTH 490
#define HEIGHT 328

int current_page = 1;
int running = 1;
int fontsize = 17;

EasyXFont* font;

// 主函数
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	initgraph(WIDTH, HEIGHT);

	wchar_t buf[] = L"EasyX " EASYX_VER L" 安装向导";

	struct nk_context* ctx;
	HWND wnd;
	HDC dc;
	int needs_refresh = 1;
	wnd = GetHWnd();
	dc = GetDC(wnd);
	SetWindowTextW(wnd, buf);

	font = nk_easyxfont_create("Segoe UI", fontsize);
	ctx = nk_easyx_init(font, dc, WIDTH, HEIGHT);
	MyWindow window(ctx, WIDTH, HEIGHT);

	bool init = false;

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

		// 先显示一次页面，避免初始化遍历文件需要时间，导致页面未及时显示出来
		if (!init)
		{
			init = true;
			window.Init();
		}
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

	p1 = new Page1(ctx, w, h);
	p2 = new Page2(ctx, w, h);
}

MyWindow::~MyWindow()
{
}

void MyWindow::Init()
{
	p2->InitIDE();
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
		p1->Draw(running, current_page);
		break;
	case 2:
		p2->Draw(running, current_page);
		break;
	}
}