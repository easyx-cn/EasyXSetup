#pragma once

#include "common.h"
#include "easyxver.h"

class Page1
{
	nk_context* _ctx;
	int Width, Height;
	LOGFONT lfont, bold_lfont;
	EasyXFont ef, bold_ef;
	nk_user_font ufont, boldFnot;

public:
	Page1(nk_context* ctx, int w, int h);
	~Page1() {}

	nk_style_button disableStyle();
	nk_style_button enableStyle();
	nk_user_font* LogoFont(nk_style* style);
	nk_user_font* BoldFont(nk_style* style);

	void Draw(int& running, int& current_page);
};