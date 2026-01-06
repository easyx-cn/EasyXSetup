#include "page1.h"


Page1::Page1(nk_context* ctx, int w, int h)
{
	_ctx = ctx;
	Width = w;
	Height = h;
}


nk_style_button Page1::disableStyle()
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

nk_style_button Page1::enableStyle()
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


nk_user_font* Page1::LogoFont(nk_style* style)
{
	setFont(&ufont, &ef, &lfont, style, 80, 0, -900, -900);
	return &ufont;
}


nk_user_font* Page1::BoldFont(nk_style* style)
{
	setFont(&boldFnot, &bold_ef, &bold_lfont, style, 22, 900);
	return &boldFnot;
}

void Page1::Draw(int& running, int& current_page)
{
	_ctx->style.window.background = nk_rgb(77, 77, 77);
	if (nk_begin(_ctx, "", nk_rect(0.0f, 0.0f, (float)Width, (float)Height), NK_WINDOW_NO_SCROLLBAR))
	{
		float rt[] = { 0.3f, 0.7f };
		float w = (float)(Width * rt[0]);
		float bottom = 45;
		nk_layout_row(_ctx, NK_DYNAMIC, Height - bottom, 2, rt);

		if (nk_group_begin(_ctx, "group_p1_logo", NK_WINDOW_NO_SCROLLBAR))
		{
			nk_layout_space_begin(_ctx, NK_STATIC, Height - 100.0f, 2);
			nk_layout_space_push(_ctx, nk_rect(0, 0, w, Height - bottom));
			nk_fill_rect(&_ctx->current->buffer, nk_rect(0, 0, w, Height - bottom), 0.0f, nk_rgb(33, 23, 129));

			nk_user_font* f = LogoFont(&_ctx->style);
			nk_layout_space_push(_ctx, nk_rect(0.0f, 0.0f, Height - bottom, w));
			nk_text_style(_ctx, "EasyX", NK_TEXT_ALIGN_LEFT, f, nk_vec2(145, 30), nk_rgb(170, 170, 170));

			nk_layout_space_end(_ctx);
			nk_group_end(_ctx);
		}

		if (nk_group_begin(_ctx, "group_p1_summary", NK_WINDOW_NO_SCROLLBAR))
		{
			wchar_t buf[] = L"欢迎使用 “EasyX " EASYX_VER "” 安装向导";

			nk_fill_rect(&_ctx->current->buffer, nk_rect(w, 0, (float)(Width * rt[1]), Height - bottom), 0.0f, nk_rgb(245, 245, 239));

			nk_user_font* f = BoldFont(&_ctx->style);
			nk_layout_row_dynamic(_ctx, 5, 1);
			nk_layout_row_dynamic(_ctx, 24, 1);
			nk_text_style(_ctx, toU8(buf), NK_TEXT_CENTERED, f, nk_vec2(0, 0), nk_rgb(33, 33, 33));
			nk_layout_row_dynamic(_ctx, 15, 1);
			nk_layout_row_dynamic(_ctx, 18, 1);
			nk_label_colored(_ctx, u8"  EasyX 库为 C/C++ 提供了简单的绘图接口，可以帮助", NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_label_colored(_ctx, u8"  您快速编写图形程序。", NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_layout_row_dynamic(_ctx, 7, 1);
			nk_layout_row_dynamic(_ctx, 18, 1);
			nk_label_colored(_ctx, u8"  这个向导将帮助您安装 EasyX 库到指定开发环境中。", NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_label_colored(_ctx, u8"  EasyX 目前支持 Visual C++ 6 / 2008 ～ 2026。", NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_layout_row_dynamic(_ctx, 7, 1);
			nk_layout_row_dynamic(_ctx, 18, 1);
			nk_label_colored(_ctx, u8"  如果您希望手动安装 EasyX，请参见帮助文件。", NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_layout_row_dynamic(_ctx, 7, 1);
			nk_layout_row_dynamic(_ctx, 18, 1);
			nk_label_colored(_ctx, u8"  单击 [下一步] 继续。", NK_TEXT_LEFT, nk_rgb(33, 33, 33));
			nk_group_end(_ctx);
		}

		nk_fill_rect(&_ctx->current->buffer, nk_rect(0.0f, 283.0f, (float)Width, 1.0f), 0.0f, nk_rgb(100, 100, 100));
		nk_fill_rect(&_ctx->current->buffer, nk_rect(0.0f, 284.0f, (float)Width, bottom), 0.0f, nk_rgb(235, 235, 209));

		nk_style_button btn = enableStyle();
		nk_layout_space_begin(_ctx, NK_STATIC, 10, 2);
		nk_layout_space_push(_ctx, nk_rect(Width - 200.0f, 7.0f, 80.0f, 25.0f));
		if (nk_button_label_styled(_ctx, &btn, u8"下一步 >"))
			current_page = 2;

		nk_layout_space_push(_ctx, nk_rect(Width - 100.0f, 7.0f, 80.0f, 25.0f));
		if (nk_button_label_styled(_ctx, &btn, u8"取消"))
			running = false;

		nk_layout_space_end(_ctx);
	}
	nk_end(_ctx);
}