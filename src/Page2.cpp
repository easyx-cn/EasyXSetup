#include "page2.h"


EFiles* eFiles[] = {
	new EFiles(L"include\\", L"easyx.h"),
	new EFiles(L"include\\", L"graphics.h"),
	new EFiles(L"lib\\VC6\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC6\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib\\VC2008\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC2008\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib\\VC2008\\x64\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC2008\\x64\\", L"EasyXw.lib"),
	new EFiles(L"lib\\VC2015\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC2015\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib\\VC2015\\x64\\", L"EasyXa.lib"),
	new EFiles(L"lib\\VC2015\\x64\\", L"EasyXw.lib"),
};

Page2::Page2(nk_context* ctx, int w, int h)
{
	_ctx = ctx;
	Width = w;
	Height = h;

	InitIDE();
}


nk_style_button Page2::disableStyle()
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

nk_style_button Page2::enableStyle()
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

nk_user_font* Page2::BoldFont(nk_style* style)
{
	setFont(&boldFnot, &bold_ef, &bold_lfont, style, 22, 900);
	return &boldFnot;
}

void Page2::InitIDE()
{
	eGroups[0] = new EGroups(L"  Visual C++ 6.0", 6, L"", L"", L"", L"", a, b1, NULL);
	eGroups[1] = new EGroups(L"  Visual C++ 2008", 2008, L"", L"", L"", L"", a, b2, b4);
	eGroups[2] = new EGroups(L"  Visual C++ 2010", 2010, L"", L"", L"", L"", a, b2, b4);
	eGroups[3] = new EGroups(L"  Visual C++ 2012", 2012, L"", L"", L"", L"", a, b2, b4);
	eGroups[4] = new EGroups(L"  Visual C++ 2013", 2013, L"", L"", L"", L"", a, b2, b4);
	eGroups[5] = new EGroups(L"  Visual C++ 2015", 2015, L"", L"", L"", L"", a, b3, b5);
	eGroups[6] = new EGroups(L"  Visual C++ 2017", 2017, L"", L"", L"", L"", a, b3, b5);
	eGroups[7] = new EGroups(L"  Visual C++ 2019", 2019, L"", L"", L"", L"", a, b3, b5);
	eGroups[8] = new EGroups(L"  Visual C++ 2022", 2022, L"", L"", L"", L"", a, b3, b5);
	eGroups[9] = new EGroups(L"  Visual C++ 2026", 2026, L"", L"", L"", L"", a, b3, b5);

	wstring path = L"桌面路径 ";
	path += g_pathDesktop();
	VSIDE* d = new VSIDE(L"  EasyX 文档", L"在线文档 https://docs.easyx.cn", path.c_str(), -1, true);
	ide_list.push_back(d);

	bool g_bX64 = false;
	wstring amd = reg.RegRead(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", L"PROCESSOR_ARCHITECTURE");

	if (amd != L"" && wcscmp(amd.c_str(), L"AMD64") == 0)
		g_bX64 = true;

	for (int i = 0; i < VSNUM; i++)
	{
		int v = eGroups[i]->ver;
		wstring name = eGroups[i]->name;
		wstring vcpath = reg.GetVCPath(v, g_bX64);

		if (vcpath == L"")
		{
			VSIDE* item = new VSIDE(name.c_str(), L"", L"", i, false);
			not_exist_list.push_back(item);
			continue;
		}

		eGroups[i]->vcpath = vcpath.c_str();
		eGroups[i]->path_h = reg.GetIncludePath(v);

		if (_waccess((eGroups[i]->vcpath + eGroups[i]->path_h).c_str(), 0) != 0) {
			eGroups[i]->vcpath = L"";
			eGroups[i]->path_h = L"";

			VSIDE* item = new VSIDE(name.c_str(), L"", L"", i, false);
			not_exist_list.push_back(item);
			continue;
		}

		eGroups[i]->path_libx86 = reg.GetLibX86Path(v);
		if (_waccess((eGroups[i]->vcpath + eGroups[i]->path_libx86).c_str(), 0) != 0) {
			eGroups[i]->vcpath = L"";
			eGroups[i]->path_h = L"";
			eGroups[i]->path_libx86 = L"";

			VSIDE* item = new VSIDE(name.c_str(), L"", L"", i, false);
			not_exist_list.push_back(item);
			continue;
		}

		eGroups[i]->path_libx64 = reg.GetLibX64Path(v);
		if (_waccess((eGroups[i]->vcpath + eGroups[i]->path_libx64).c_str(), 0) != 0) {
			eGroups[i]->path_libx64 = L"";
		}

		wstring path1 = L"头文件路径 " + eGroups[i]->vcpath + eGroups[i]->path_h;
		wstring path2 = L"库文件路径 " + eGroups[i]->vcpath + eGroups[i]->path_libx86;

		if (eGroups[i]->path_libx64 != L"") {
			path2 += L" " + eGroups[i]->vcpath + eGroups[i]->path_libx64;
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

void Page2::Draw(int& running, int& current_page)
{
	if (nk_begin(_ctx, "", nk_rect(0, 0, (float)Width, (float)Height), NK_WINDOW_NO_SCROLLBAR))
	{
		nk_fill_rect(&_ctx->current->buffer, nk_rect((float)0, (float)0, (float)Width, (float)45), 0.0f, nk_rgb(250, 250, 250));
		nk_fill_rect(&_ctx->current->buffer, nk_rect((float)0, (float)45, (float)Width, (float)Height), 0.0f, nk_rgb(235, 235, 209));

		nk_layout_row_dynamic(_ctx, 16, 1);
		nk_user_font* f = BoldFont(&_ctx->style);
		nk_text_style(_ctx, u8"  执行安装", NK_TEXT_LEFT, f, nk_vec2(0, 0), nk_rgb(33, 33, 33));
		nk_layout_row_dynamic(_ctx, 1, 1);
		nk_layout_row_dynamic(_ctx, 16, 1);
		nk_label_colored(_ctx, u8"        请针对您所用的开发平台，选择安装或卸载。", NK_TEXT_LEFT, nk_rgb(33, 33, 33));
		nk_layout_row_dynamic(_ctx, 8, 1);
		nk_layout_row_dynamic(_ctx, 16, 1);
		nk_label_colored(_ctx, u8"  在下面中找到您所用的开发平台，点击右侧的“安装”将 EasyX 库安装至", NK_TEXT_LEFT, nk_rgb(33, 33, 33));
		nk_label_colored(_ctx, u8"  该开发平台。或点击右侧的“卸载”将 EasyX 库从该开发平台移除。", NK_TEXT_LEFT, nk_rgb(33, 33, 33));

		float rt[] = { 1.0f };
		nk_style_button btn = enableStyle();
		nk_style_button button = disableStyle();
		nk_layout_row_dynamic(_ctx, 6, 1);
		nk_layout_row(_ctx, NK_DYNAMIC, Height - 160.0f, 1, rt);

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

					nk_layout_space_push(_ctx, nk_rect(Width - 135.0f, 0.0f, 45.0f, 30.0f));
					if (nk_button_label_styled(_ctx, &btn, u8"安装"))
					{
						wstring s;
						if ((*itor)->id == -1)
							s = InstallHelp();
						else
							s = Install((*itor)->id);

						if (s.size() == 0)
							popup_msg = L"安装成功";
						else
							popup_msg = s;

						popup_active = true;
					}

					nk_layout_space_push(_ctx, nk_rect(Width - 85.0f, 0.0f, 45.0f, 30.0f));
					if (nk_button_label_styled(_ctx, &btn, u8"卸载"))
					{
						wstring s;
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
					nk_layout_space_push(_ctx, nk_rect(110.0f, 0.0f, 230.0f, 30.0f));
					nk_label(_ctx, u8"(未检测到)", NK_TEXT_LEFT);

					nk_layout_space_push(_ctx, nk_rect(Width - 135.0f, 0.0f, 45.0f, 30.0f));
					nk_button_label_styled(_ctx, &button, u8"安装");

					nk_layout_space_push(_ctx, nk_rect(Width - 85.0f, 0.0f, 45.0f, 30.0f));
					nk_button_label_styled(_ctx, &button, u8"卸载");
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
				nk_label_colored(_ctx, toU8(popup_msg.c_str()), NK_TEXT_CENTERED, nk_rgb(33, 33, 33));
				nk_layout_row_dynamic(_ctx, 10, 1);
				nk_layout_row_dynamic(_ctx, 25, 1);
				if (nk_button_label_styled(_ctx, &btn, u8"OK")) {
					popup_active = false;
					nk_popup_close(_ctx);
				}
				nk_popup_end(_ctx);
			}
			else popup_active = false;
		}

		nk_layout_row_dynamic(_ctx, 6, 1);
		nk_label_colored(_ctx, u8"-- EasyX Install System —————————————————————————————", NK_TEXT_LEFT, nk_rgb(123, 123, 123));
		nk_layout_row_dynamic(_ctx, 1, 1);

		nk_layout_space_begin(_ctx, NK_STATIC, 25, 2);
		nk_layout_space_push(_ctx, nk_rect(Width - 200.0f, 0, 80, 25));
		if (nk_button_label_styled(_ctx, &btn, u8"< 上一步"))
			current_page = 1;

		nk_layout_space_push(_ctx, nk_rect(Width - 100.0f, 0, 80, 25));
		if (nk_button_label_styled(_ctx, &btn, u8"关闭"))
			running = false;
		nk_layout_space_end(_ctx);
	}
	nk_end(_ctx);
}

const wchar_t* Page2::g_pathDesktop() {
	wchar_t* path = NULL;
	HRESULT result = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path);
	if (result != S_OK)
		return NULL;
	//CoTaskMemFree(path);

	return path;
}

wchar_t* Page2::help_path() {
	const wchar_t* dest = g_pathDesktop();
	wchar_t const* p2 = L"\\EasyX_Help.lnk";

	int len = lstrlenW(dest) + lstrlenW(p2) + 1;
	wchar_t* buf = new wchar_t[len];
	_snwprintf_s(buf, len, len, L"%s%s", dest, p2);
	buf[len - 1] = L'\0';

	return buf;
}

/// <summary>
/// 创建快捷方式
/// </summary>
/// <param name="szStartAppPath"></param>
/// <param name="szDestLnkPath"></param>
/// <returns></returns>
bool Page2::CreateLinkFile(LPCWSTR urlexePath, LPCWSTR szDestLnkPath)
{
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLinkW* pShellLink;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
		if (SUCCEEDED(hr))
		{
			pShellLink->SetPath(urlexePath);
			wstring strTmp = urlexePath;
			size_t nStart = strTmp.find_last_of(L"/\\");
			pShellLink->SetWorkingDirectory(strTmp.substr(0, nStart).c_str());
			IPersistFile* pPersistFile;
			hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
			if (SUCCEEDED(hr))
			{
				hr = pPersistFile->Save(szDestLnkPath, FALSE);
				if (SUCCEEDED(hr))
					return true;
				pPersistFile->Release();
			}
			pShellLink->Release();
		}
		CoUninitialize();
	}
	return false;
}

wstring Page2::deleteFile(wchar_t const* dst)
{
	try {
		WIN32_FIND_DATAW p;
		HANDLE h = FindFirstFileW(dst, &p);
		if (h != INVALID_HANDLE_VALUE) {
			FindClose(h);
			bool result = DeleteFileW(dst);
			if (!result)
				return L"操作失败。";
		}
	}
	catch (exception ex)
	{
		return L"操作异常";
	}
	return L"";
}

wstring Page2::copy_Files(wchar_t const* src, wchar_t const* dst)
{
	try {
		bool result = CopyFileW(src, dst, false);  // 会覆盖源文件
		if (!result)
			return (wstring)L"安装失败";
	}
	catch (exception ex)
	{
		return L"安装异常";
	}
	return L"";
}



void Page2::UninstallOldEasyX(int id) {
	int len_ = eGroups[id]->ver;
	if (eGroups[id]->ver == 2017 || eGroups[id]->ver == 2019) {
		wstring oldpath = eGroups[id]->vcpath;
		if (oldpath.length() > 13 && oldpath.substr(oldpath.length() - 14, 14) == L"\\Auxiliary\\VS\\") {
			oldpath = oldpath.substr(0, oldpath.length() - 13);
			wstring verfile = oldpath + L"Auxiliary\\Build\\Microsoft.VCToolsVersion.default.txt";
			wchar_t const* cver = verfile.c_str();
			if (_waccess(cver, 0) == 0) {  // 如果文件存在
				wifstream fin(cver, ios::in);
				wstring str = L"";
				getline(fin, str);
				fin.close();

				oldpath += L"Tools\\MSVC\\";
				oldpath += str.c_str();
				oldpath += L"\\";

				wstring f;
				if (eGroups[id]->path_h != L"") {
					for (int i = 0; i < 2; i++) {
						f = oldpath + eGroups[id]->path_h + eFiles[eGroups[id]->files_h[i]]->name;
						deleteFile(f.c_str());
					}
				}

				if (eGroups[id]->path_libx86 != L"") {
					for (int i = 0; i < 2; i++) {
						f = oldpath + eGroups[id]->path_libx86 + eFiles[eGroups[id]->files_libx86[i]]->name;
						deleteFile(f.c_str());
					}
				}

				if (eGroups[id]->path_libx64 != L"") {
					for (int i = 0; i < 2; i++) {
						f = oldpath + eGroups[id]->path_libx64 + eFiles[eGroups[id]->files_libx64[i]]->name;
						deleteFile(f.c_str());
					}
				}
			}
		}
	}
}

wstring Page2::Install(int id)
{
	if (eGroups[id]->ver == 2017 || eGroups[id]->ver == 2019) {
		UninstallOldEasyX(id);
	}

	wchar_t exeFullPath[MAX_PATH];
	GetModuleFileNameW(NULL, exeFullPath, MAX_PATH);
	wstring strFullPath = exeFullPath;
	size_t nStart = strFullPath.find_last_of(L"\\");
	wstring strExeName = strFullPath.substr(0, nStart + 1);

	wstring err = L"";
	if (eGroups[id]->files_h != NULL) {
		for (int i = 0; i < 2; i++) {
			int d = eGroups[id]->files_h[i];
			EFiles* dd = eFiles[d];

			wstring src = eFiles[eGroups[id]->files_h[i]]->path + eFiles[eGroups[id]->files_h[i]]->name;
			wstring dst = eGroups[id]->vcpath + eGroups[id]->path_h + eFiles[eGroups[id]->files_h[i]]->name;
			err += copy_Files((strExeName + src).c_str(), dst.c_str());

			if (err != L"")
				return L"安装失败";
		}
	}
	if (eGroups[id]->files_libx86 != NULL) {
		for (int i = 0; i < 2; i++) {
			wstring src = eFiles[eGroups[id]->files_libx86[i]]->path + eFiles[eGroups[id]->files_libx86[i]]->name;
			wstring dst = eGroups[id]->vcpath + eGroups[id]->path_libx86 + eFiles[eGroups[id]->files_libx86[i]]->name;
			err += copy_Files((strExeName + src).c_str(), dst.c_str());
			if (err != L"")
				return L"安装失败";
		}
	}
	if (eGroups[id]->path_libx64 != L"" && eGroups[id]->files_libx64 != NULL) {
		for (int i = 0; i < 2; i++) {
			wstring src = eFiles[eGroups[id]->files_libx64[i]]->path + eFiles[eGroups[id]->files_libx64[i]]->name;
			wstring dst = eGroups[id]->vcpath + eGroups[id]->path_libx64 + eFiles[eGroups[id]->files_libx64[i]]->name;
			err += copy_Files((strExeName + src).c_str(), dst.c_str());
			if (err != L"")
				return L"安装失败";
		}
	}

	return err;
}

wstring Page2::Uninstall(int id) {
	if (eGroups[id]->ver == 2017 || eGroups[id]->ver == 2019) {
		UninstallOldEasyX(id);
	}

	bool result = false;
	wstring err = L"卸载失败";
	wstring f;
	if (eGroups[id]->path_h != L"") {
		for (int i = 0; i < 2; i++) {
			f = eGroups[id]->vcpath + eGroups[id]->path_h + eFiles[eGroups[id]->files_h[i]]->name;
			result = DeleteFileW(f.c_str());
			if (!result)
				return err;
		}
	}

	if (eGroups[id]->path_libx86 != L"") {
		for (int i = 0; i < 2; i++) {
			f = eGroups[id]->vcpath + eGroups[id]->path_libx86 + eFiles[eGroups[id]->files_libx86[i]]->name;
			result = DeleteFileW(f.c_str());
			if (!result)
				return err;
		}
	}

	if (eGroups[id]->path_libx64 != L"") {
		for (int i = 0; i < 2; i++) {
			f = eGroups[id]->vcpath + eGroups[id]->path_libx64 + eFiles[eGroups[id]->files_libx64[i]]->name;
			result = DeleteFileW(f.c_str());
			if (!result)
				return err;
		}
	}

	if (!result)
		return err;
	else
		return L"卸载成功";
}

wstring Page2::UninstallHelp() {
	wstring err = deleteFile(help_path());

	if (err == L"")
		return L"卸载成功";
	else
		return L"卸载失败";
}
wstring Page2::InstallHelp() {
	bool result = CreateLinkFile(L"https://docs.easyx.cn", help_path());
	if (!result)
		return L"EasyX 文档安装失败。";

	return L"";
}