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

#pragma warning(disable:4996)				// nuklear 需要使用新版本 VS 默认禁止的 vsprintf 等旧版 C 函数

#include "lib/nuklear/nuklear.h"
#include "lib/nuklear/nuklear_easyx.h"


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

void setFont(nk_user_font* ufont, EasyXFont* ef, LOGFONT* lf, nk_style* style, int h, int weight = 0, LONG escap = 0, LONG orien = 0)
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

const char* toU8(const wchar_t* str, int len = -1)
{
	char* pElementText;
	int    iTextLen;
	iTextLen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	pElementText = (char*)malloc((iTextLen + 1) * sizeof(char));
	if (pElementText == NULL)
		return "";

	memset(pElementText, 0, (iTextLen + 1) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, str, -1, pElementText, iTextLen, NULL, NULL);

	return pElementText;
}