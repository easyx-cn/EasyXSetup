#pragma once
/*
 * Nuklear - 1.32.0 - public domain
 * no warrenty implied; use at your own risk.
 * authored from 2015-2016 by Micha Mettke
 */
 /*
  * ==============================================================
  *
  *                              API
  *
  * ===============================================================
  */
#ifndef NK_EASYX_H_
#define NK_EASYX_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define NK_EASYX_IMPLEMENTATION

#pragma warning(disable:4996)				// nuklear ��Ҫʹ���°汾 VS Ĭ�Ͻ�ֹ�� vsprintf �Ⱦɰ� C ����


#include <wingdi.h>
#include <graphics.h>
#pragma comment(lib,"Msimg32.lib")

#include "nuklearx.h"







#define WIN32_LEAN_AND_MEAN
#include <wingdi.h>
#include <graphics.h>
#pragma comment(lib,"Msimg32.lib")


struct EasyXFont {
    struct nk_user_font nk;
    //int height;
    LOGFONT* handle;
};

static struct {
    HDC window_dc, memory_dc;
    unsigned int width;
    unsigned int height;
    struct nk_context ctx;
} easyx;

NK_API struct nk_context* nk_easyx_init(EasyXFont* font, HDC window_dc, unsigned int width, unsigned int height);
NK_API int nk_easyx_handle_event(HWND hwnd, ExMessage* msg);
NK_API void nk_easyx_render(struct nk_color clear);
NK_API void nk_easyx_shutdown(void);

/* font */
NK_API EasyXFont* nk_easyxfont_create(const char* name, int h, int w = 0);
NK_API void nk_easyxfont_del(EasyXFont* font);
NK_API void nk_easyx_set_font(EasyXFont* font);


char* UnicodeToANSI(const wchar_t* str);
wchar_t* ANSIToUnicode(const char* str, int len = -1);
char* UTF8ToASCII(const char* str, int len = -1);
const TCHAR* toTCHAR(const char* src);
void _settextstyle(int h, int w, const char* fontname);
void _outtextxy(int x, int y, const char* text);
int _textwidth(const char* text, int len = -1);
void
nk_gdi_clipboard_paste(nk_handle usr, struct nk_text_edit* edit);
void
nk_gdi_clipboard_copy(nk_handle usr, const char* text, int len);
void
nk_create_image(struct nk_image* image, const char* frame_buffer, const int width, const int height);
void
nk_easyx_delete_image(struct nk_image* image);
void
nk_easyx_draw_image(short x, short y, unsigned short w, unsigned short h,
    struct nk_image img, struct nk_color col);
COLORREF
convert_color(struct nk_color c);
void
nk_easyx_scissor(HDC dc, float x, float y, float w, float h);
void
nk_easyx_stroke_line(HDC dc, short x0, short y0, short x1,
    short y1, unsigned int line_thickness, struct nk_color col);
void
nk_easyx_stroke_rect(HDC dc, short x, short y, unsigned short w,
    unsigned short h, unsigned short r, unsigned short line_thickness, struct nk_color col);
void
nk_easyx_fill_rect(HDC dc, short x, short y, unsigned short w,
    unsigned short h, unsigned short r, struct nk_color col);
void
nk_gdi_set_vertexColor(PTRIVERTEX tri, struct nk_color col);
void
nk_gdi_rect_multi_color(HDC dc, short x, short y, unsigned short w,
    unsigned short h, struct nk_color left, struct nk_color top,
    struct nk_color right, struct nk_color bottom);
BOOL
SetPoint(POINT* p, LONG x, LONG y);
void
nk_easyx_fill_triangle(HDC dc, short x0, short y0, short x1,
    short y1, short x2, short y2, struct nk_color col);
void
nk_easyx_stroke_triangle(HDC dc, short x0, short y0, short x1,
    short y1, short x2, short y2, unsigned short line_thickness, struct nk_color col);
void
nk_easyx_fill_polygon(HDC dc, const struct nk_vec2i* pnts, int count, struct nk_color col);
void
nk_easyx_stroke_polygon(HDC dc, const struct nk_vec2i* pnts, int count,
    unsigned short line_thickness, struct nk_color col);
void
nk_easyx_stroke_polyline(HDC dc, const struct nk_vec2i* pnts,
    int count, unsigned short line_thickness, struct nk_color col);
void
nk_easyx_stroke_arc(HDC dc, short cx, short cy, unsigned short r, float amin, float adelta, unsigned short line_thickness, struct nk_color col);
void
nk_easyx_fill_arc(HDC dc, short cx, short cy, unsigned short r, float amin, float adelta, struct nk_color col);
void
nk_easyx_fill_circle(HDC dc, short x, short y, unsigned short w,
    unsigned short h, struct nk_color col);
void
nk_easyx_stroke_circle(HDC dc, short x, short y, unsigned short w,
    unsigned short h, unsigned short line_thickness, struct nk_color col);
void
nk_easyx_stroke_curve(HDC dc, struct nk_vec2i p1,
    struct nk_vec2i p2, struct nk_vec2i p3, struct nk_vec2i p4,
    unsigned short line_thickness, struct nk_color col);
void
nk_easyx_draw_text(HDC dc, short x, short y, unsigned short w, unsigned short h,
    const char* text, int len, EasyXFont* font, struct nk_color cbg, struct nk_color cfg);
void
nk_easyx_clear(struct nk_color col);
void
nk_gdi_blit(HDC dc);
float
nk_easyxfont_get_text_width(nk_handle handle, float height, const char* text, int len);
NK_API EasyXFont*
nk_easyxfont_create(const char* name, int height, int width);
NK_API struct nk_context*
nk_easyx_init(EasyXFont* f, HDC window_dc, unsigned int width, unsigned int height);
void
nk_easyxfont_del(EasyXFont* font);
NK_API void
nk_easyx_set_font(EasyXFont* f);
NK_API int
nk_easyx_handle_event(HWND wnd, ExMessage* msg);
NK_API void
nk_easyx_shutdown(void);
NK_API void
nk_easyx_render(struct nk_color clear);
#endif
