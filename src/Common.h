#pragma once


#include "nuklear_easyx.h"


// 自定义文本样式函数
void nk_text_style(struct nk_context* ctx, const char* str,
	nk_flags alignment, const struct nk_user_font* f, struct nk_vec2 offset, nk_color col);

void setFont(nk_user_font* ufont, EasyXFont* ef, LOGFONT* lf, nk_style* style, int h, int weight = 0, LONG escap = 0, LONG orien = 0);

const char* toU8(const wchar_t* str, int len = -1);