

/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#include "nuklear_easyx.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>


char* UnicodeToANSI(const wchar_t* str)
{
    char* pElementText;
    int    iTextLen;
    iTextLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    pElementText = (char*)malloc((iTextLen + 1) * sizeof(char));
    if (pElementText != NULL)
        memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
    WideCharToMultiByte(CP_ACP, 0, str, -1, pElementText, iTextLen, NULL, NULL);
    return pElementText;
}

wchar_t* ANSIToUnicode(const char* str, int len)
{
    int  unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str, len, NULL, 0);
    wchar_t* pUnicode = (wchar_t*)malloc((unicodeLen + 1) * sizeof(wchar_t));
    if (pUnicode == NULL)
        return NULL;

    memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, len, (LPWSTR)pUnicode, unicodeLen);
    return  pUnicode;
}

char* UTF8ToASCII(const char* str, int len)
{
    int  unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str, len, NULL, 0);
    wchar_t* pUnicode;
    pUnicode = new wchar_t[unicodeLen + 1];
    memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, len, (LPWSTR)pUnicode, unicodeLen);

    char* pElementText;
    int    iTextLen;
    iTextLen = WideCharToMultiByte(CP_ACP, 0, pUnicode, -1, NULL, 0, NULL, NULL);
    pElementText = new char[iTextLen + 1];
    memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
    WideCharToMultiByte(CP_ACP, 0, pUnicode, -1, pElementText, iTextLen, NULL, NULL);
    return  pElementText;
}


// 转为 TCHAR* 类型
const TCHAR* toTCHAR(const char* src)
{
#ifdef UNICODE
    int n = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    wchar_t* wp = (wchar_t*)malloc((n + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, src, -1, wp, n);
    return wp;
#else
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, src, -1, nullptr, 0);
    if (wideSize == 0) {
        return "";
    }

    wchar_t* wp = (wchar_t*)malloc((wideSize + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, src, -1, wp, wideSize);
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wp, -1, NULL, 0, NULL, NULL);
    if (utf8Size == 0)
        return _T("");

    char* p = (char*)malloc((utf8Size + 1) * sizeof(char));
    WideCharToMultiByte(CP_UTF8, 0, wp, -1, p, utf8Size, nullptr, nullptr);

    return p;
#endif
}

void _settextstyle(int h, int w, const char* fontname)
{
    LOGFONT f;
    gettextstyle(&f);
    f.lfQuality = PROOF_QUALITY;
    f.lfOutPrecision = OUT_TT_ONLY_PRECIS;
    f.lfWidth = w;
    f.lfHeight = h;
    _tcscpy(f.lfFaceName, toTCHAR(fontname));

    settextstyle(&f);
}

void _outtextxy(int x, int y, const char* text)
{
#ifdef _UNICODE
    wchar_t* str = ANSIToUnicode(text);
    outtextxy(x, y, str);
    free(str);
#else
    outtextxy(x, y, UTF8ToASCII(text));
#endif
}

int _textwidth(const char* text, int len)
{
    int w = 0;
#ifdef _UNICODE
    wchar_t* str = ANSIToUnicode(text, len);
    w = textwidth(str);
    free(str);
#else
    w = textwidth(UTF8ToASCII(text, len));
#endif

    return w;
}




static void
nk_gdi_clipboard_paste(nk_handle usr, struct nk_text_edit* edit)
{
    (void)usr;
    if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(NULL))
    {
        HGLOBAL mem = GetClipboardData(CF_UNICODETEXT);
        if (mem)
        {
            SIZE_T size = GlobalSize(mem) - 1;
            if (size)
            {
                LPCWSTR wstr = (LPCWSTR)GlobalLock(mem);
                if (wstr)
                {
                    int utf8size = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)(size / sizeof(wchar_t)), NULL, 0, NULL, NULL);
                    if (utf8size)
                    {
                        char* utf8 = (char*)malloc(utf8size);
                        if (utf8)
                        {
                            WideCharToMultiByte(CP_UTF8, 0, wstr, (int)(size / sizeof(wchar_t)), utf8, utf8size, NULL, NULL);
                            nk_textedit_paste(edit, utf8, utf8size);
                            free(utf8);
                        }
                    }
                    GlobalUnlock(mem);
                }
            }
        }
        CloseClipboard();
    }
}

static void
nk_gdi_clipboard_copy(nk_handle usr, const char* text, int len)
{
    if (OpenClipboard(NULL))
    {
        int wsize = MultiByteToWideChar(CP_UTF8, 0, text, len, NULL, 0);
        if (wsize)
        {
            HGLOBAL mem = (HGLOBAL)GlobalAlloc(GMEM_MOVEABLE, (wsize + 1) * sizeof(wchar_t));
            if (mem)
            {
                wchar_t* wstr = (wchar_t*)GlobalLock(mem);
                if (wstr)
                {
                    MultiByteToWideChar(CP_UTF8, 0, text, len, wstr, wsize);
                    wstr[wsize] = 0;
                    GlobalUnlock(mem);

                    SetClipboardData(CF_UNICODETEXT, mem);
                }
            }
        }
        CloseClipboard();
    }
}

static void
nk_create_image(struct nk_image* image, const char* frame_buffer, const int width, const int height)
{
    if (image && frame_buffer && (width > 0) && (height > 0))
    {
        const unsigned char* src = (const unsigned char*)frame_buffer;
        INT row = ((width * 3 + 3) & ~3);
        LPBYTE lpBuf, pb = NULL;
        BITMAPINFO bi = { 0 };
        HBITMAP hbm;
        int v, i;

        image->w = width;
        image->h = height;
        image->region[0] = 0;
        image->region[1] = 0;
        image->region[2] = width;
        image->region[3] = height;

        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = width;
        bi.bmiHeader.biHeight = height;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 24;
        bi.bmiHeader.biCompression = BI_RGB;
        bi.bmiHeader.biSizeImage = row * height;

        hbm = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (void**)&lpBuf, NULL, 0);

        pb = lpBuf + row * height;
        for (v = 0; v < height; v++)
        {
            pb -= row;
            for (i = 0; i < row; i += 3)
            {
                pb[i + 0] = src[0];
                pb[i + 1] = src[1];
                pb[i + 2] = src[2];
                src += 3;
            }
        }
        SetDIBits(NULL, hbm, 0, height, lpBuf, &bi, DIB_RGB_COLORS);
        image->handle.ptr = hbm;
    }
}

static void
nk_easyx_delete_image(struct nk_image* image)
{
    if (image && image->handle.id != 0)
        memset(image, 0, sizeof(struct nk_image));
}

static void
nk_easyx_draw_image(short x, short y, unsigned short w, unsigned short h,
    struct nk_image img, struct nk_color col)
{
    IMAGE* image = (IMAGE*)img.handle.ptr;
    if (!image)
        return;

    putimage(x, y, w, h, image, 0, 0);
}

static COLORREF
convert_color(struct nk_color c)
{
    return c.r | (c.g << 8) | (c.b << 16);
}

static void
nk_easyx_scissor(HDC dc, float x, float y, float w, float h)
{
    setcliprgn(NULL);
    HRGN rgn = CreateRectRgn((int)x, (int)y, (int)(x + w + 1), (int)(y + h + 1));
    setcliprgn(rgn);
    DeleteObject(rgn);
}

static void
nk_easyx_stroke_line(HDC dc, short x0, short y0, short x1,
    short y1, unsigned int line_thickness, struct nk_color col)
{
    COLORREF c = getlinecolor();
    LINESTYLE l;
    getlinestyle(&l);

    COLORREF color = convert_color(col);
    setlinestyle(PS_SOLID, line_thickness);
    setlinecolor(color);
    line(x0, y0, x1, y1);

    setlinecolor(c);
    setlinestyle(&l);
}

static void
nk_easyx_stroke_rect(HDC dc, short x, short y, unsigned short w,
    unsigned short h, unsigned short r, unsigned short line_thickness, struct nk_color col)
{
    COLORREF c = getlinecolor();
    LINESTYLE l;
    getlinestyle(&l);

    COLORREF color = convert_color(col);
    setlinestyle(PS_SOLID, line_thickness);
    setlinecolor(color);

    if (r == 0)
        rectangle(x, y, x + w, y + h);
    else
        roundrect(x, y, x + w, y + h, r, r);

    setlinecolor(c);
    setlinestyle(&l);
}

static void
nk_easyx_fill_rect(HDC dc, short x, short y, unsigned short w,
    unsigned short h, unsigned short r, struct nk_color col)
{
    COLORREF c = getfillcolor();
    COLORREF lc = getlinecolor();

    COLORREF color = convert_color(col);
    setfillcolor(color);
    setlinecolor(color);

    if (r == 0) {
        fillrectangle(x, y, x + w, y + h);
    }
    else {
        fillroundrect(x, y, x + w, y + h, r, r);
    }

    setfillcolor(c);
    setlinecolor(lc);
}

static void
nk_gdi_set_vertexColor(PTRIVERTEX tri, struct nk_color col)
{
    tri->Red = col.r << 8;
    tri->Green = col.g << 8;
    tri->Blue = col.b << 8;
    tri->Alpha = 0xff << 8;
}

static void
nk_gdi_rect_multi_color(HDC dc, short x, short y, unsigned short w,
    unsigned short h, struct nk_color left, struct nk_color top,
    struct nk_color right, struct nk_color bottom)
{
    BLENDFUNCTION alphaFunction;
    GRADIENT_TRIANGLE gTri[2];
    TRIVERTEX vt[4];
    alphaFunction.BlendOp = AC_SRC_OVER;
    alphaFunction.BlendFlags = 0;
    alphaFunction.SourceConstantAlpha = 0;
    alphaFunction.AlphaFormat = AC_SRC_ALPHA;

    /* TODO: This Case Needs Repair.*/
    /* Top Left Corner */
    vt[0].x = x;
    vt[0].y = y;
    nk_gdi_set_vertexColor(&vt[0], left);
    /* Top Right Corner */
    vt[1].x = x + w;
    vt[1].y = y;
    nk_gdi_set_vertexColor(&vt[1], top);
    /* Bottom Left Corner */
    vt[2].x = x;
    vt[2].y = y + h;
    nk_gdi_set_vertexColor(&vt[2], right);

    /* Bottom Right Corner */
    vt[3].x = x + w;
    vt[3].y = y + h;
    nk_gdi_set_vertexColor(&vt[3], bottom);

    gTri[0].Vertex1 = 0;
    gTri[0].Vertex2 = 1;
    gTri[0].Vertex3 = 2;
    gTri[1].Vertex1 = 2;
    gTri[1].Vertex2 = 1;
    gTri[1].Vertex3 = 3;
    GdiGradientFill(dc, vt, 4, gTri, 2, GRADIENT_FILL_TRIANGLE);
    AlphaBlend(easyx.window_dc, x, y, x + w, y + h, dc, x, y, x + w, y + h, alphaFunction);
}

static BOOL
SetPoint(POINT* p, LONG x, LONG y)
{
    if (!p)
        return FALSE;
    p->x = x;
    p->y = y;
    return TRUE;
}


static void
nk_easyx_fill_triangle(HDC dc, short x0, short y0, short x1,
    short y1, short x2, short y2, struct nk_color col)
{
    COLORREF c = getfillcolor();
    COLORREF lc = getlinecolor();

    COLORREF color = convert_color(col);
    setfillcolor(color);
    setlinecolor(color);
    POINT points[3];

    SetPoint(&points[0], x0, y0);
    SetPoint(&points[1], x1, y1);
    SetPoint(&points[2], x2, y2);
    fillpolygon(points, 3);

    setfillcolor(c);
    setlinecolor(lc);
}

static void
nk_easyx_stroke_triangle(HDC dc, short x0, short y0, short x1,
    short y1, short x2, short y2, unsigned short line_thickness, struct nk_color col)
{
    COLORREF c = getlinecolor();

    COLORREF color = convert_color(col);
    setlinecolor(color);
    POINT points[4];

    SetPoint(&points[0], x0, y0);
    SetPoint(&points[1], x1, y1);
    SetPoint(&points[2], x2, y2);
    SetPoint(&points[3], x0, y0);
    polygon(points, 4);

    setlinecolor(c);
}

static void
nk_easyx_fill_polygon(HDC dc, const struct nk_vec2i* pnts, int count, struct nk_color col)
{
    COLORREF c = getfillcolor();
    COLORREF lc = getlinecolor();

    int i = 0;
#define MAX_POINTS 64
    POINT points[MAX_POINTS];
    COLORREF color = convert_color(col);
    setfillcolor(color);
    setlinecolor(color);
    for (i = 0; i < count && i < MAX_POINTS; ++i) {
        points[i].x = pnts[i].x;
        points[i].y = pnts[i].y;
    }
    fillpolygon(points, i);
#undef MAX_POINTS

    setfillcolor(c);
    setlinecolor(lc);
}

static void
nk_easyx_stroke_polygon(HDC dc, const struct nk_vec2i* pnts, int count,
    unsigned short line_thickness, struct nk_color col)
{
    COLORREF c = getlinecolor();
    LINESTYLE l;
    getlinestyle(&l);

    COLORREF color = convert_color(col);
    setlinecolor(color);
    setlinestyle(PS_SOLID, line_thickness);
#define MAX_POINTS 64
    int i = 0;
    POINT points[MAX_POINTS];
    for (i = 0; i < count && i < MAX_POINTS; ++i) {
        points[i].x = pnts[i].x;
        points[i].y = pnts[i].y;
    }
    polygon(points, i);
#undef MAX_POINTS

    setlinecolor(c);
    setlinestyle(&l);
}

static void
nk_easyx_stroke_polyline(HDC dc, const struct nk_vec2i* pnts,
    int count, unsigned short line_thickness, struct nk_color col)
{
    COLORREF c = getlinecolor();
    LINESTYLE l;
    getlinestyle(&l);

    COLORREF color = convert_color(col);
    setlinecolor(color);
    setlinestyle(PS_SOLID, line_thickness);

    if (count > 0) {
        int i;
        moveto(pnts[0].x, pnts[0].y);
        for (i = 1; i < count; ++i)
            lineto(pnts[i].x, pnts[i].y);
    }

    setlinecolor(c);
    setlinestyle(&l);
}

static void
nk_easyx_stroke_arc(HDC dc, short cx, short cy, unsigned short r, float amin, float adelta, unsigned short line_thickness, struct nk_color col)
{
    COLORREF c = getlinecolor();
    LINESTYLE l;
    getlinestyle(&l);

    COLORREF color = convert_color(col);
    setlinecolor(color);
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_GEOMETRIC, line_thickness);

    arc(cx - r, cy - r, cx + r, cy + r, amin, adelta);

    setlinecolor(c);
    setlinestyle(&l);
}

static void
nk_easyx_fill_arc(HDC dc, short cx, short cy, unsigned short r, float amin, float adelta, struct nk_color col)
{
    COLORREF c = getfillcolor();
    COLORREF lc = getlinecolor();

    COLORREF color = convert_color(col);
    setfillcolor(color);
    setlinecolor(color);
    fillpie(cx - r, cy - r, cx + r, cy + r, amin, adelta);

    setfillcolor(c);
    setlinecolor(lc);
}

static void
nk_easyx_fill_circle(HDC dc, short x, short y, unsigned short w,
    unsigned short h, struct nk_color col)
{
    COLORREF c = getfillcolor();
    COLORREF lc = getlinecolor();

    COLORREF color = convert_color(col);
    setfillcolor(color);
    setlinecolor(color);
    fillellipse(x, y, x + w, y + h);

    setfillcolor(c);
    setlinecolor(lc);
}

static void
nk_easyx_stroke_circle(HDC dc, short x, short y, unsigned short w,
    unsigned short h, unsigned short line_thickness, struct nk_color col)
{
    COLORREF c = getlinecolor();
    LINESTYLE l;
    getlinestyle(&l);

    COLORREF color = convert_color(col);
    setlinecolor(color);
    setlinestyle(PS_SOLID, line_thickness);
    ellipse(x, y, x + w, y + h);

    setlinecolor(c);
    setlinestyle(&l);
}

static void
nk_easyx_stroke_curve(HDC dc, struct nk_vec2i p1,
    struct nk_vec2i p2, struct nk_vec2i p3, struct nk_vec2i p4,
    unsigned short line_thickness, struct nk_color col)
{
    COLORREF c = getlinecolor();

    COLORREF color = convert_color(col);
    setlinecolor(color);
    POINT p[4];

    SetPoint(&p[0], p1.x, p1.y);
    SetPoint(&p[1], p2.x, p2.y);
    SetPoint(&p[2], p3.x, p3.y);
    SetPoint(&p[3], p4.x, p4.y);

    if (line_thickness != 1)
        setlinestyle(PS_SOLID, line_thickness);

    polybezier(p, 4);
    setlinecolor(c);
}

static void
nk_easyx_draw_text(HDC dc, short x, short y, unsigned short w, unsigned short h,
    const char* text, int len, EasyXFont* font, struct nk_color cbg, struct nk_color cfg)
{
    COLORREF c = gettextcolor();
    COLORREF bc = getbkcolor();
    int m = getbkmode();
    LOGFONT f;
    gettextstyle(&f);

    settextstyle(font->handle);
    setbkcolor(convert_color(cbg));
    settextcolor(convert_color(cfg));
    setbkmode(TRANSPARENT);

    _outtextxy(x, y, text);

    settextstyle(&f);
    settextcolor(c);
    setbkcolor(bc);
    setbkmode(m);
}

static void
nk_easyx_clear(struct nk_color col)
{
    COLORREF color = convert_color(col);
    setbkcolor(color);
    cleardevice();
}

static void
nk_gdi_blit(HDC dc)
{
    BitBlt(dc, 0, 0, easyx.width, easyx.height, easyx.window_dc, 0, 0, SRCCOPY);
}

static float
nk_easyxfont_get_text_width(nk_handle handle, float height, const char* text, int len)
{
    EasyXFont* font = (EasyXFont*)handle.ptr;
    if (!font || !text)
        return 0;

    LOGFONT f;
    gettextstyle(&f);

    settextstyle(font->handle);
    float w = (float)_textwidth(text, len);

    settextstyle(&f);
    return w;
}

NK_API EasyXFont*
nk_easyxfont_create(const char* name, int height, int width)
{
    _settextstyle(height, width, name);

    EasyXFont* font = (EasyXFont*)calloc(1, sizeof(EasyXFont));
    if (!font)
        return NULL;

    LOGFONT* f = (LOGFONT*)calloc(1, sizeof(LOGFONT));
    gettextstyle(f);
    font->handle = f;

    //font->handle->lfHeight = textheight(_T("T"));
    //font->height = textheight(_T("T"));
    return font;
}

NK_API struct nk_context*
nk_easyx_init(EasyXFont* f, HDC window_dc, unsigned int width, unsigned int height)
{
    struct nk_user_font* font = &f->nk;
    font->userdata = nk_handle_ptr(f);
    font->height = (float)f->handle->lfHeight;
    font->width = nk_easyxfont_get_text_width;

    easyx.window_dc = window_dc;
    easyx.memory_dc = CreateCompatibleDC(window_dc);
    easyx.width = width;
    easyx.height = height;

    nk_init_default(&easyx.ctx, font);
    easyx.ctx.clip.copy = nk_gdi_clipboard_copy;
    easyx.ctx.clip.paste = nk_gdi_clipboard_paste;
    return &easyx.ctx;
}

void
nk_easyxfont_del(EasyXFont* font)
{
    if (!font) return;
    free(font);
}

NK_API void
nk_easyx_set_font(EasyXFont* f)
{
    struct nk_user_font* font = &f->nk;
    font->userdata = nk_handle_ptr(f);
    font->height = (float)f->handle->lfHeight;
    font->width = nk_easyxfont_get_text_width;
    nk_style_set_font(&easyx.ctx, font);
}

NK_API int
nk_easyx_handle_event(HWND wnd, ExMessage* msg)
{
    WPARAM wparam = msg->wParam; LPARAM lparam = msg->lParam;
    switch (msg->message)
    {
        /* case WM_SIZE:
         {
             unsigned width = LOWORD(lparam);
             unsigned height = HIWORD(lparam);
             if (width != easyx.width || height != easyx.height)
             {
                 easyx.width = width;
                 easyx.height = height;
             }
             break;
         }*/

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    {
        //int down = !((lparam >> 31) & 1);
        int down = msg->message == WM_SYSKEYDOWN || msg->message == WM_KEYDOWN;
        // int ctrl = GetKeyState(VK_CONTROL) & (1 << 15);
        int ctrl = msg->ctrl;

        switch (msg->vkcode)
        {
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            nk_input_key(&easyx.ctx, NK_KEY_SHIFT, down);
            return 1;

        case VK_DELETE:
            nk_input_key(&easyx.ctx, NK_KEY_DEL, down);
            return 1;

        case VK_RETURN:
            nk_input_key(&easyx.ctx, NK_KEY_ENTER, down);
            return 1;

        case VK_TAB:
            nk_input_key(&easyx.ctx, NK_KEY_TAB, down);
            return 1;

        case VK_LEFT:
            if (ctrl)
                nk_input_key(&easyx.ctx, NK_KEY_TEXT_WORD_LEFT, down);
            else
                nk_input_key(&easyx.ctx, NK_KEY_LEFT, down);
            return 1;

        case VK_RIGHT:
            if (ctrl)
                nk_input_key(&easyx.ctx, NK_KEY_TEXT_WORD_RIGHT, down);
            else
                nk_input_key(&easyx.ctx, NK_KEY_RIGHT, down);
            return 1;

        case VK_BACK:
            nk_input_key(&easyx.ctx, NK_KEY_BACKSPACE, down);
            return 1;

        case VK_HOME:
            nk_input_key(&easyx.ctx, NK_KEY_TEXT_START, down);
            nk_input_key(&easyx.ctx, NK_KEY_SCROLL_START, down);
            return 1;

        case VK_END:
            nk_input_key(&easyx.ctx, NK_KEY_TEXT_END, down);
            nk_input_key(&easyx.ctx, NK_KEY_SCROLL_END, down);
            return 1;

        case VK_NEXT:
            nk_input_key(&easyx.ctx, NK_KEY_SCROLL_DOWN, down);
            return 1;

        case VK_PRIOR:
            nk_input_key(&easyx.ctx, NK_KEY_SCROLL_UP, down);
            return 1;

        case 'A':
            if (ctrl) {
                nk_input_key(&easyx.ctx, NK_KEY_TEXT_SELECT_ALL, down);
                return 1;
            }
            break;

        case 'C':
            if (ctrl) {
                nk_input_key(&easyx.ctx, NK_KEY_COPY, down);
                return 1;
            }
            break;

        case 'V':
            if (ctrl) {
                nk_input_key(&easyx.ctx, NK_KEY_PASTE, down);
                return 1;
            }
            break;

        case 'X':
            if (ctrl) {
                nk_input_key(&easyx.ctx, NK_KEY_CUT, down);
                return 1;
            }
            break;

        case 'Z':
            if (ctrl) {
                nk_input_key(&easyx.ctx, NK_KEY_TEXT_UNDO, down);
                return 1;
            }
            break;

        case 'R':
            if (ctrl) {
                nk_input_key(&easyx.ctx, NK_KEY_TEXT_REDO, down);
                return 1;
            }
            break;
        }
        return 0;
    }

    case WM_CHAR:
        if (wparam >= 32)
        {
            nk_input_unicode(&easyx.ctx, (nk_rune)msg->ch);
            return 1;
        }
        break;

    case WM_LBUTTONDOWN:
        nk_input_button(&easyx.ctx, NK_BUTTON_LEFT, msg->x, msg->y, 1);
        setcapture();
        return 1;

    case WM_LBUTTONUP:
        nk_input_button(&easyx.ctx, NK_BUTTON_DOUBLE, msg->x, msg->y, 0);
        nk_input_button(&easyx.ctx, NK_BUTTON_LEFT, msg->x, msg->y, 0);
        releasecapture();
        return 1;

    case WM_RBUTTONDOWN:
        nk_input_button(&easyx.ctx, NK_BUTTON_RIGHT, msg->x, msg->y, 1);
        setcapture();
        return 1;

    case WM_RBUTTONUP:
        nk_input_button(&easyx.ctx, NK_BUTTON_RIGHT, msg->x, msg->y, 0);
        ReleaseCapture();
        return 1;

    case WM_MBUTTONDOWN:
        nk_input_button(&easyx.ctx, NK_BUTTON_MIDDLE, msg->x, msg->y, 1);
        setcapture();
        return 1;

    case WM_MBUTTONUP:
        nk_input_button(&easyx.ctx, NK_BUTTON_MIDDLE, msg->x, msg->y, 0);
        releasecapture();
        return 1;

    case WM_MOUSEWHEEL:
        nk_input_scroll(&easyx.ctx, nk_vec2(0, (float)(short)msg->wheel / WHEEL_DELTA));
        return 1;

    case WM_MOUSEMOVE:
        nk_input_motion(&easyx.ctx, msg->x, msg->y);
        return 1;

    case WM_LBUTTONDBLCLK:
        nk_input_button(&easyx.ctx, NK_BUTTON_DOUBLE, msg->x, msg->y, 1);
        return 1;
    }

    return 0;
}

NK_API void
nk_easyx_shutdown(void)
{
    nk_free(&easyx.ctx);
}

NK_API void
nk_easyx_render(struct nk_color clear)
{
    const struct nk_command* cmd;
    nk_easyx_clear(clear);
    HDC memory_dc = easyx.window_dc;

    nk_foreach(cmd, &easyx.ctx)
    {
        switch (cmd->type) {
        case NK_COMMAND_NOP: break;
        case NK_COMMAND_SCISSOR: {
            const struct nk_command_scissor* s = (const struct nk_command_scissor*)cmd;
            nk_easyx_scissor(memory_dc, s->x, s->y, s->w, s->h);
        } break;
        case NK_COMMAND_LINE: {
            const struct nk_command_line* l = (const struct nk_command_line*)cmd;
            nk_easyx_stroke_line(memory_dc, l->begin.x, l->begin.y, l->end.x,
                l->end.y, l->line_thickness, l->color);
        } break;
        case NK_COMMAND_RECT: {
            const struct nk_command_rect* r = (const struct nk_command_rect*)cmd;
            nk_easyx_stroke_rect(memory_dc, r->x, r->y, r->w, r->h,
                (unsigned short)r->rounding, r->line_thickness, r->color);
        } break;
        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled* r = (const struct nk_command_rect_filled*)cmd;
            nk_easyx_fill_rect(memory_dc, r->x, r->y, r->w, r->h,
                (unsigned short)r->rounding, r->color);
        } break;
        case NK_COMMAND_CIRCLE: {
            const struct nk_command_circle* c = (const struct nk_command_circle*)cmd;
            nk_easyx_stroke_circle(memory_dc, c->x, c->y, c->w, c->h, c->line_thickness, c->color);
        } break;
        case NK_COMMAND_CIRCLE_FILLED: {
            const struct nk_command_circle_filled* c = (const struct nk_command_circle_filled*)cmd;
            nk_easyx_fill_circle(memory_dc, c->x, c->y, c->w, c->h, c->color);
        } break;
        case NK_COMMAND_ARC: {
            const struct nk_command_arc* q = (const struct nk_command_arc*)cmd;
            nk_easyx_stroke_arc(memory_dc, q->cx, q->cy, q->r, q->a[0], q->a[1], q->line_thickness, q->color);
        } break;
        case NK_COMMAND_ARC_FILLED: {
            const struct nk_command_arc_filled* q = (const struct nk_command_arc_filled*)cmd;
            nk_easyx_fill_arc(memory_dc, q->cx, q->cy, q->r, q->a[0], q->a[1], q->color);
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle* t = (const struct nk_command_triangle*)cmd;
            nk_easyx_stroke_triangle(memory_dc, t->a.x, t->a.y, t->b.x, t->b.y,
                t->c.x, t->c.y, t->line_thickness, t->color);
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled* t = (const struct nk_command_triangle_filled*)cmd;
            nk_easyx_fill_triangle(memory_dc, t->a.x, t->a.y, t->b.x, t->b.y,
                t->c.x, t->c.y, t->color);
        } break;
        case NK_COMMAND_POLYGON: {
            const struct nk_command_polygon* p = (const struct nk_command_polygon*)cmd;
            nk_easyx_stroke_polygon(memory_dc, p->points, p->point_count, p->line_thickness, p->color);
        } break;
        case NK_COMMAND_POLYGON_FILLED: {
            const struct nk_command_polygon_filled* p = (const struct nk_command_polygon_filled*)cmd;
            nk_easyx_fill_polygon(memory_dc, p->points, p->point_count, p->color);
        } break;
        case NK_COMMAND_POLYLINE: {
            const struct nk_command_polyline* p = (const struct nk_command_polyline*)cmd;
            nk_easyx_stroke_polyline(memory_dc, p->points, p->point_count, p->line_thickness, p->color);
        } break;
        case NK_COMMAND_TEXT: {
            const struct nk_command_text* t = (const struct nk_command_text*)cmd;
            nk_easyx_draw_text(memory_dc, t->x, t->y, t->w, t->h,
                (const char*)t->string, t->length,
                (EasyXFont*)t->font->userdata.ptr,
                t->background, t->foreground);
        } break;
        case NK_COMMAND_CURVE: {
            const struct nk_command_curve* q = (const struct nk_command_curve*)cmd;
            nk_easyx_stroke_curve(memory_dc, q->begin, q->ctrl[0], q->ctrl[1],
                q->end, q->line_thickness, q->color);
        } break;
        case NK_COMMAND_RECT_MULTI_COLOR: {
            const struct nk_command_rect_multi_color* r = (const struct nk_command_rect_multi_color*)cmd;
            nk_gdi_rect_multi_color(memory_dc, r->x, r->y, r->w, r->h, r->left, r->top, r->right, r->bottom);
        } break;
        case NK_COMMAND_IMAGE: {
            const struct nk_command_image* i = (const struct nk_command_image*)cmd;
            nk_easyx_draw_image(i->x, i->y, i->w, i->h, i->img, i->col);
        } break;
        case NK_COMMAND_CUSTOM:
        default: break;
        }
    }
    nk_clear(&easyx.ctx);
}
