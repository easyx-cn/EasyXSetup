#include "page2.h"

//#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")		// winapi GetFileVersionInfo 需要

EFiles* eFiles[] = {
	new EFiles(L"include\\", L"easyx.h"),
	new EFiles(L"include\\", L"graphics.h"),
	new EFiles(L"lib_vs\\VC6\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib_vs\\VC6\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib_vs\\VC2008\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib_vs\\VC2008\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib_vs\\VC2008\\x64\\", L"EasyXa.lib"),
	new EFiles(L"lib_vs\\VC2008\\x64\\", L"EasyXw.lib"),
	new EFiles(L"lib_vs\\VC2015\\x86\\", L"EasyXa.lib"),
	new EFiles(L"lib_vs\\VC2015\\x86\\", L"EasyXw.lib"),
	new EFiles(L"lib_vs\\VC2015\\x64\\", L"EasyXa.lib"),
	new EFiles(L"lib_vs\\VC2015\\x64\\", L"EasyXw.lib")
};


Page2::Page2(nk_context* ctx, int w, int h)
{
	_ctx = ctx;
	Width = w;
	Height = h;
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

	mingw_Groups[DEVCPP] = new EMingWGroups(L"  DevCpp", DEVCPP);
	mingw_Groups[CODEBLOCKS] = new EMingWGroups(L"  CodeBlocks", CODEBLOCKS);
	mingw_Groups[CLION] = new EMingWGroups(L"  CLion", CLION);
	mingw_Groups[VSCODE] = new EMingWGroups(L"  VSCode", VSCODE);
	mingw_Groups[CFREE] = new EMingWGroups(L"  C-Free", CFREE);
	mingw_Groups[OTHER_IDE] = new EMingWGroups(L"  其它 Mingw", OTHER_IDE);

	wstring path = L"桌面路径 ";
	path += g_pathDesktop();
	VSIDE* d = new VSIDE(L"  EasyX 文档", L"在线文档 https://docs.easyx.cn", path.c_str(), -1, true, VISUAL_STUDIO);
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
			VSIDE* item = new VSIDE(name.c_str(), L"", L"", i, false, VISUAL_STUDIO);
			not_exist_list.push_back(item);
			continue;
		}

		eGroups[i]->vcpath = vcpath.c_str();
		eGroups[i]->path_h = L"include\\";

		if (_waccess((eGroups[i]->vcpath + eGroups[i]->path_h).c_str(), 0) != 0) {
			eGroups[i]->vcpath = L"";
			eGroups[i]->path_h = L"";

			VSIDE* item = new VSIDE(name.c_str(), L"", L"", i, false, VISUAL_STUDIO);
			not_exist_list.push_back(item);
			continue;
		}

		eGroups[i]->path_libx86 = reg.GetLibX86Path(v);
		if (_waccess((eGroups[i]->vcpath + eGroups[i]->path_libx86).c_str(), 0) != 0) {
			eGroups[i]->vcpath = L"";
			eGroups[i]->path_h = L"";
			eGroups[i]->path_libx86 = L"";

			VSIDE* item = new VSIDE(name.c_str(), L"", L"", i, false, VISUAL_STUDIO);
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
		VSIDE* item = new VSIDE(name.c_str(), path1.c_str(), path2.c_str(), i, true, VISUAL_STUDIO);
		exist_list.push_back(item);
	}

	FindSDK(L"", DEVCPP, NULL, g_bX64);
	FindSDK(L"", CLION, NULL, g_bX64);
	FindSDK(L"", CODEBLOCKS, NULL, g_bX64);
	FindSDK(L"", CFREE, NULL, g_bX64);

	for (list<VSIDE*>::iterator itor = exist_list.begin(); itor != exist_list.end(); itor++)
	{
		ide_list.push_back(*itor);
	}

	for (list<VSIDE*>::iterator itor = not_exist_list.begin(); itor != not_exist_list.end(); itor++)
	{
		ide_list.push_back(*itor);
	}

	VSIDE* item = new VSIDE(mingw_Groups[OTHER_IDE]->name.c_str(), L"", L"", OTHER_IDE, false, MINGW);
	ide_list.push_back(item);
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

		// 鼠标悬停检测限制在控件内部！
		struct nk_rect bounds = nk_widget_bounds(_ctx);		// 获取的是后面紧跟的第一个控件坐标
		bool in_rect = nk_input_is_mouse_hovering_rect(&_ctx->input, bounds);
		if (nk_group_begin(_ctx, "group_p2_list", 0)) {		// group_p2_list 不能与其它 nk_group_begin 相同，否则会串滚

			int i = 0, h = 26;
			int cur_height = 10;

			int s_h = textheight(_T("天"));
			float tip_w = 340.0f;

			for (list<VSIDE*>::iterator itor = ide_list.begin(); itor != ide_list.end(); itor++)
			{
				nk_layout_space_begin(_ctx, NK_STATIC, 35, 5);
				nk_layout_space_push(_ctx, nk_rect(0, 0, 110, 30));
				nk_label_colored(_ctx, toU8((*itor)->name.c_str()), NK_TEXT_LEFT, nk_rgb(33, 33, 33));

				if ((*itor)->exist == true)
				{
					nk_layout_space_push(_ctx, nk_rect(110, 0, 230, 13));
					bounds = nk_widget_bounds(_ctx);
					nk_label_colored(_ctx, toU8((*itor)->path_1.c_str()), NK_TEXT_LEFT, nk_rgb(33, 33, 33));

					// 鼠标悬浮 tooltip
					if (in_rect && !popup_active && nk_input_is_mouse_hovering_rect(&_ctx->input, bounds)) {
						nk_tooltip_begin(_ctx, tip_w);
						const char* str = toU8((*itor)->path_1.c_str());
						int len = strlen(str);
						int s_w = textwidth((*itor)->path_1.c_str());
						float line = s_w / tip_w + 1;
						nk_layout_row_static(_ctx, (float)line * s_h, (int)tip_w - 10, 1);
						nk_label_colored_wrap(_ctx, str, nk_rgb(233, 233, 233));
						nk_tooltip_end(_ctx);
					}

					nk_layout_space_push(_ctx, nk_rect(110, 16, 230, 13));
					bounds = nk_widget_bounds(_ctx);
					nk_label_colored(_ctx, toU8((*itor)->path_2.c_str()), NK_TEXT_LEFT, nk_rgb(33, 33, 33));

					// 鼠标悬浮 tooltip
					if (in_rect && !popup_active && nk_input_is_mouse_hovering_rect(&_ctx->input, bounds))
					{
						nk_tooltip_begin(_ctx, tip_w);
						const char* str = toU8((*itor)->path_2.c_str());
						int len = strlen(str);
						int s_w = textwidth((*itor)->path_2.c_str());
						double line = s_w / tip_w + 1.5;
						nk_layout_row_static(_ctx, (float)(line * s_h), (int)tip_w - 10, 1);
						nk_label_colored_wrap(_ctx, str, nk_rgb(233, 233, 233));
						nk_tooltip_end(_ctx);
					}

					nk_layout_space_push(_ctx, nk_rect(Width - 135.0f, 0.0f, 45.0f, 30.0f));
					if (nk_button_label_styled(_ctx, &btn, u8"安装"))
					{
						wstring s;
						if ((*itor)->id == -1)
							s = InstallHelp();
						else if ((*itor)->type == VISUAL_STUDIO)
							s = Install((*itor)->id);
						else
							s = Install_mingw((*itor)->id);

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
						else if ((*itor)->type == VISUAL_STUDIO)
							s = Uninstall((*itor)->id);
						else
							s = Uninstall_mingw((*itor)->id);

						popup_active = true;
						popup_msg = s;
					}
				}
				else	// 灰色按钮
				{
					nk_layout_space_push(_ctx, nk_rect(110.0f, 0.0f, 230.0f, 30.0f));
					nk_label(_ctx, toU8((*itor)->msg.c_str()), NK_TEXT_LEFT);

					nk_layout_space_push(_ctx, nk_rect(Width - 135.0f, 0.0f, 95.0f, 30.0f));
					if (nk_button_label_styled(_ctx, &btn, u8"选择安装目录"))
					{
						wstring path = OpenFolder();
						if (!path.empty())
						{
							if ((*itor)->type == VISUAL_STUDIO)
							{
								int ver = AnalysisPath(path, *itor);
								if (ver == NOTFOUND)
								{
									popup_active = true;
									popup_msg = L"未检测到 VS";
								}
								else if (ver == ERROR_1)
								{
									popup_active = true;
									popup_msg = L"版本错误，请重新选择对应版本";
								}
								else
								{
									(*itor)->exist = true;
								}
							}
							else
							{
								int ver = FindSDK(path, (*itor)->id, *itor, false);
								if (ver == NOTFOUND)
								{
									popup_active = true;
									popup_msg = L"未检测到编译器";
								}
								else if (ver == ERROR_1)
								{
									popup_active = true;
									popup_msg = (*itor)->msg;
								}
								else if (ver == NOSURE)
								{
									(*itor)->exist = true;
									popup_active = true;
									popup_msg = (*itor)->msg;
								}
								else
								{
									(*itor)->exist = true;
								}
							}
						}
					}
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

wstring Page2::Install_mingw(int id)
{
	wchar_t exeFullPath[MAX_PATH];
	GetModuleFileNameW(NULL, exeFullPath, MAX_PATH);
	wstring strFullPath = exeFullPath;
	size_t nStart = strFullPath.find_last_of(L"\\");
	wstring strExeName = strFullPath.substr(0, nStart + 1);

	//EFiles* mingw_eFiles[] = {
	//	new EFiles(L"include\\", L"easyx.h"),
	//	new EFiles(L"include\\", L"graphics.h"),
	//	new EFiles(L"lib32\\libeasyx.a", L""),
	//	new EFiles(L"lib32\\libeasyxw.a", L""),
	//	new EFiles(L"lib64\\libeasyx.a", L""),
	//	new EFiles(L"lib64\\libeasyxw.a", L""),
	//	new EFiles(L"lib-for-devcpp_5.4.0\\", L"libeasyx.a")
	//};

	EMingWGroups* ep = mingw_Groups[id];

	wstring src_lib32, src_libw32, src_lib64, src_libw64;
	wstring dest_lib32 = ep->mingw_path + ep->path_lib32 + L"libeasyx.a";
	wstring dest_libw32 = ep->mingw_path + ep->path_lib32 + L"libeasyxw.a";

	wstring dest_lib64 = ep->mingw_path + ep->path_lib64 + L"libeasyx.a";
	wstring dest_libw64 = ep->mingw_path + ep->path_lib64 + L"libeasyxw.a";

	src_lib32 = strExeName + L"lib_mingw\\lib32\\libeasyx.a";
	src_libw32 = strExeName + L"lib_mingw\\lib32\\libeasyxw.a";
	src_lib64 = strExeName + L"lib_mingw\\lib64\\libeasyx.a";
	src_libw64 = strExeName + L"lib_mingw\\lib64\\libeasyxw.a";
	wstring src_devcpp_54_x86 = strExeName + L"lib_mingw\\lib-for-devcpp_5.4.0\\libeasyx.a";

	wstring err = L"";
	if (ep->mingw_path != L"")
	{
		wstring src = strExeName + L"include\\easyx.h";
		wstring dst = ep->mingw_path + ep->path_h + L"easyx.h";
		err += copy_Files(src.c_str(), dst.c_str());
		
		src = strExeName + L"include\\graphics.h";
		dst = ep->mingw_path + ep->path_h + L"graphics.h";
		err += copy_Files(src.c_str(), dst.c_str());

		if (err != L"")
			return L"安装失败_inlcude";
	}

	if (ep->path_lib32 != L"" && ep->use_devcpp_5_4_x86 == false)
	{
		wstring dst = ep->mingw_path + ep->path_lib32 + L"libeasyx.a";
		err += copy_Files(src_lib32.c_str(), dst.c_str());

		dst = ep->mingw_path + ep->path_lib32 + L"libeasyxw.a";
		err += copy_Files(src_libw32.c_str(), dst.c_str());
		if (err != L"")
			return L"安装失败_lib32";
	}
	if (ep->path_lib32 != L"" && ep->use_devcpp_5_4_x86 == true)
	{
		wstring dst = ep->mingw_path + ep->path_lib32 + L"libeasyx.a";
		err += copy_Files(src_devcpp_54_x86.c_str(), dst.c_str());
		if (err != L"")
			return L"安装失败_lib32";
	}

	if (ep->path_lib64 != L"")
	{
		wstring dst = ep->mingw_path + ep->path_lib64 + L"libeasyx.a";
		err += copy_Files(src_lib64.c_str(), dst.c_str());

		dst = ep->mingw_path + ep->path_lib64 + L"libeasyxw.a";
		err += copy_Files(src_libw64.c_str(), dst.c_str());
		if (err != L"")
			return L"安装失败_lib64";
	}

	return err;
}

wstring Page2::Uninstall_mingw(int id)
{
	bool result = false;
	wstring err = L"卸载失败";
	wstring f;
	if (mingw_Groups[id]->mingw_path != L"")
	{
		f = mingw_Groups[id]->mingw_path + mingw_Groups[id]->path_h + L"easyx.h";
		result = DeleteFileW(f.c_str());
		if (!result)
			return err;

		f = mingw_Groups[id]->mingw_path + mingw_Groups[id]->path_h + L"graphics.h";
		result = DeleteFileW(f.c_str());
		if (!result)
			return err;
	}

	if (mingw_Groups[id]->path_lib32 != L"")
	{
		f = mingw_Groups[id]->mingw_path + mingw_Groups[id]->path_lib32 + L"libeasyx.a";
		result = DeleteFileW(f.c_str());
		if (!result)
			return err;

		f = mingw_Groups[id]->mingw_path + mingw_Groups[id]->path_lib32 + L"libeasyxw.a";
		result = DeleteFileW(f.c_str());
		if (!result)
			return err;
	}

	if (mingw_Groups[id]->path_lib64 != L"")
	{
		f = mingw_Groups[id]->mingw_path + mingw_Groups[id]->path_lib64 + L"libeasyx.a";
		result = DeleteFileW(f.c_str());
		if (!result)
			return err;

		f = mingw_Groups[id]->mingw_path + mingw_Groups[id]->path_lib64 + L"libeasyxw.a";
		result = DeleteFileW(f.c_str());
		if (!result)
			return err;
	}

	if (!result)
		return err;
	else
		return L"卸载成功";
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

wstring Page2::OpenFolder()
{
	Sleep(100);		// 必须等待鼠标弹起消息进入 nuklear，否则下次点击无效
	wchar_t szPathName[MAX_PATH];
	BROWSEINFO bInfo = { 0 };
	bInfo.hwndOwner = GetForegroundWindow();
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_UAHINT;
	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);
	if (lpDlist != NULL) {
		SHGetPathFromIDListW(lpDlist, szPathName);
		return szPathName;
	}

	return L"";
}

/// <summary>
/// 返回上层目录
/// </summary>
/// <param name="filepath"></param>
/// <returns></returns>
filesystem::path Page2::safe_get_parent(const filesystem::path& filepath) {
	if (filepath.empty() || filepath == filepath.root_path()) {
		return filepath;  // 已经是根目录，返回自身
	}

	filesystem::path parent = filepath.parent_path();
	return parent.empty() ? filepath.root_path() : parent;
}

int Page2::toVer(DWORD cl_ver)
{
	int h = cl_ver / 100;
	int l = cl_ver % 100;
	switch (h)
	{
	case 12:
		return 6;
	case 15:
		return 2008;
	case 16:
		return 2010;
	case 17:
		return 2012;
	case 18:
		return 2013;
	case 19:
		{
			if (l < 10)
				return 2015;
			else if (l >= 10 && l <= 19)
				return 2017;
			else if (l >= 20 && l <= 29)
				return 2019;
			else if (l >= 30 && l <= 49)
				return 2022;
			else if (l >= 50/* && l <= 59??*/)
				return 2026;
			else
				return NOTFOUND;
		}
	case NOTFOUND:
		return NOTFOUND;
	default:
		return cl_ver;
	}
}

void Page2::check(EGroups* ep)
{
	if (_waccess((ep->vcpath + ep->path_h).c_str(), 0) != 0) {
		ep->vcpath = L"";
		ep->path_h = L"";
	}

	if (_waccess((ep->vcpath + ep->path_libx86).c_str(), 0) != 0) {
		ep->vcpath = L"";
		ep->path_h = L"";
		ep->path_libx86 = L"";
	}

	if (_waccess((ep->vcpath + ep->path_libx64).c_str(), 0) != 0) {
		ep->path_libx64 = L"";
	}
}

/// <summary>
/// * 当前目录下存在 vc 文件夹，cl.exe 就在该路径内部
/// * 当前目录下存在 vc 文件夹，但不是 cl.exe 的路径，没有找到cl.exe 继续查找父目录
/// * 当前目录下是 Community|Professional|Enterprise 往下层目录检测 VC 文件夹是否存在
/// * 当前目录下是 2017\2019\2022\18 文件夹，每次执行上一步流程三次，检测 Community|Professional|Enterprise 类型
/// vc6		12.xx
/// 2008	15.xx
/// 2010	16.xx
/// 2012	17.xx
/// 2013	18.xx
/// 2015	19.00
/// 2017	19.10-19.19
/// 2019	19.20-19.29
/// 2022	19.30-19.49
/// 2026	19.50-19.??
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
int Page2::AnalysisPath(wstring path, VSIDE* vec, bool repeat)
{
	EGroups* ep = eGroups[vec->id];
	wstring p = path;
	if (path[lstrlenW(path.c_str()) - 1] != L'\\')
		p += L"\\";

	bool next = false;
	DWORD cl_ver = NOTFOUND;
	wstring ver_c = findFolder(p, L"VC");
	if (ver_c != L"")
	{
		// 检测 2008-2015 的 cl.exe 路径
		cl_ver = exeVersion(ver_c + L"bin\\cl.exe");
		if (cl_ver != NOTFOUND)
		{
			if (cl_ver != ep->ver)	
				return ERROR_1;	// 必须报错，不能再继续循环，否则会无限循环

			ep->path_libx86 = L"lib\\";
			ep->path_libx64 = L"lib\\amd64\\";
			ep->vcpath = ver_c; 
			ep->path_h = L"include\\";
			vec->path_1 = L"头文件路径 " + ver_c + ep->path_h;
			vec->path_2 = L"库文件路径 " + ver_c + ep->path_libx86 + L" " + ver_c + ep->path_libx64;
			check(ep);
		}
		else
			cl_ver = clVersion_2017(ver_c, vec); // >= 2017 的版本

		if (cl_ver != NOTFOUND)
			return cl_ver;

		next = repeat;
	}
	else
	{
		// 检测 VC6.0
		ver_c = findFolder(p, L"VC98");
		if (ver_c != L"")
		{
			cl_ver = exeVersion(ver_c + L"bin\\cl.exe");
			if (cl_ver != NOTFOUND)
			{
				EGroups* ep = eGroups[vec->id];
				if (cl_ver != ep->ver)
					return ERROR_1;

				ep->path_libx86 = L"lib\\";
				ep->path_libx64 = L"";
				ep->vcpath = ver_c;
				ep->path_h = L"include\\";
				vec->path_1 = L"头文件路径 " + ver_c + ep->path_h;
				vec->path_2 = L"库文件路径 " + ver_c + ep->path_libx86;
				check(ep);
				return cl_ver;
			}
		}
		cl_ver = clVersion_2017(p + L"community\\vc\\", vec);
		if (cl_ver != NOTFOUND)	// 如果版本不对也需要返回，结束递归，否则会一直循环
			return cl_ver;

		cl_ver = clVersion_2017(p + L"professional\\vc\\", vec);
		if (cl_ver != NOTFOUND)
			return cl_ver;

		cl_ver = clVersion_2017(p + L"enterprise\\vc\\", vec);
		if (cl_ver != NOTFOUND)
			return cl_ver;


		/**************
		* bug: 如果当前文件夹下同时存在多个版本（2017、2019、2022...），可能会检测到其它版本
		*/
		// F:\\Program Files\\Microsoft Visual Studio 路劲后是 2017\2019\2022\2026 的情况
		// 进入下一层目录，递归调用检测 community | professional | enterprise
		/*ver_c = findFolder(p, L"2017");	不检测版本文件夹，避免同时存在多个版本时，
		if (ver_c != L"")
			return AnalysisPath(p + L"2017", id, false);

		ver_c = findFolder(p, L"2019");
		if (ver_c != L"")
			return AnalysisPath(p + L"2019", id, false);

		ver_c = findFolder(p, L"2022");
		if (ver_c != L"")
			return AnalysisPath(p + L"2022", id, false);

		ver_c = findFolder(p, L"18");
		if (ver_c != L"")
			return AnalysisPath(p + L"18", id, false);*/

		// 默认 repeat = true，当使用 AnalysisPath 继续检测子目录时，不能进行循环，否则会进入死循环
		next = repeat;
	}

	if (next)
	{
		filesystem::path pp = path.c_str();
		filesystem::path pr = safe_get_parent(pp);

		if (_wcsicmp(pp.root_path().c_str(), pr.c_str()) != 0)
			return AnalysisPath(pr, vec);
	}

	return NOTFOUND;
}

int Page2::clVersion_2017(wstring p, VSIDE* vec)
{
	wstring type = L"";
	DWORD cl_ver = NOTFOUND;
	wstring s_c = p + L"Tools\\MSVC\\";
	wregex rex(L"^\\d+\\.\\d+\\.\\d+$", regex_constants::icase);    // 18.xx.xxxxx

	wstring ver_c = findFolder(s_c, rex);
	if (ver_c != L"")
	{
		cl_ver = exeVersion(ver_c + L"bin\\Hostx86\\x86\\cl.exe");
		if (cl_ver != NOTFOUND)
		{
			EGroups* ep = eGroups[vec->id];
			if (cl_ver != ep->ver)
				return ERROR_1;

			ep->path_libx86 = L"lib\\x86\\";
			ep->path_libx64 = L"lib\\x64\\";
			ep->vcpath = p + L"Auxiliary\\VS\\";
			ep->path_h = L"include\\";
			vec->path_1 = L"头文件路径 " + ep->vcpath + ep->path_h;
			vec->path_2 = L"库文件路径 " + ep->vcpath + ep->path_libx86 + L" " + ver_c + ep->path_libx64;
			check(ep);
			return cl_ver;
		}
	}

	return NOTFOUND;
}

/// <summary>
/// 读取文件属性获取版本号
/// </summary>
/// <param name="clpath"></param>
/// <returns></returns>
int Page2::exeVersion(wstring clpath)
{
	DWORD dummy;
	DWORD size = GetFileVersionInfoSize(clpath.c_str(), &dummy);
	std::vector<BYTE> versionBuffer(size);
	if (!GetFileVersionInfo(clpath.c_str(), 0, size, versionBuffer.data())) {
		return NOTFOUND;
	}

	VS_FIXEDFILEINFO* fileInfo = nullptr;
	UINT len = 0;
	if (!VerQueryValue(versionBuffer.data(), L"\\", reinterpret_cast<LPVOID*>(&fileInfo), &len)) {
		return NOTFOUND;
	}

	// 构建版本字符串
	DWORD major = HIWORD(fileInfo->dwFileVersionMS);
	DWORD minor = LOWORD(fileInfo->dwFileVersionMS);
	/*DWORD build = HIWORD(fileInfo->dwFileVersionLS);
	DWORD revision = LOWORD(fileInfo->dwFileVersionLS);*/

	std::wstring version = std::to_wstring(major) + L"." +
		std::to_wstring(minor)/* +L"." +
		std::to_wstring(build) + L"." +
		std::to_wstring(revision)*/;

	return toVer(major * 100 + minor);
}


/// <summary>
/// 检测某个格式路径是否存在
/// 如果存在返回绝对路径，末尾加 \\
/// </summary>
/// <param name="path"></param>
/// <param name="rex"></param>
/// <returns></returns>
wstring Page2::findFolder(wstring path, wregex rex)
{
	wstring searchPath = path + L"*";;
	WIN32_FIND_DATAW findData;
	HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return L"";

	wstring result = L"";
	do {
		if (wcscmp(findData.cFileName, L".") == 0 ||
			wcscmp(findData.cFileName, L"..") == 0) {
			continue;
		}

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wsmatch ms;
			wstring fc = findData.cFileName;
			bool r = regex_search(fc, ms, rex);
			if (r)
			{
				result = path + fc + L"\\";
				break;
			}
		}
	} while (FindNextFileW(hFind, &findData) != 0);
	FindClose(hFind);

	return result;
}

wstring Page2::findFolder(wstring path, const wchar_t* folder)
{
	wstring searchPath = path + L"*";;
	WIN32_FIND_DATAW findData;
	HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return L"";

	wstring result = L"";
	do {
		// 跳过 "." 和 ".."
		if (wcscmp(findData.cFileName, L".") == 0 ||
			wcscmp(findData.cFileName, L"..") == 0) {
			continue;
		}

		// 检查是否是 folder 目录
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_wcsicmp(findData.cFileName, folder) == 0)
			{
				result = path + folder + L"\\";
				break;
			}
		}
	} while (FindNextFileW(hFind, &findData) != 0);
	FindClose(hFind);

	return result;
}

/// <summary>
/// libgcc_s_seh-1.dll
/// libgcc_s_sjlj-1.dll
/// libgcc_s_dw2-1.dll
/// libgcc_s_seh_64-1.dll
/// 有可能检测不到这些文件，还需要从 gcc -v 检测！！！
/// </summary>
/// <param name="mingw_path"></param>
/// <returns></returns>
wstring Page2::check_mingw_exception(wstring bin_path, wstring cmdOut)
{
	wregex rex_seh(L"^libgcc_s_seh.*?-1\\.dll$", regex_constants::icase);
	wregex rex_sjlj(L"^libgcc_s_sjlj.*?-1\\.dll$", regex_constants::icase);
	wregex rex_dwarf(L"^libgcc_s_dw2.*?-1\\.dll$", regex_constants::icase);

	if (find_file(bin_path, rex_seh) != L"")
		return _SEH;

	if (find_file(bin_path, rex_sjlj) != L"")
		return _SJLJ;

	if (find_file(bin_path, rex_dwarf) != L"")
		return _DWARF;

	wsmatch ms;
	wregex rex(L"sjlj", regex_constants::icase);
	bool r = regex_search(cmdOut, ms, rex);
	if (r)
		return _SJLJ;

	wregex rex2(L"seh", regex_constants::icase);
	r = regex_search(cmdOut, ms, rex2);
	if (r)
		return _SEH;

	wregex rex3(L"dwarf", regex_constants::icase);
	r = regex_search(cmdOut, ms, rex3);
	if (r)
		return _DWARF;

	return L"unkonw-ex";
}

/// <summary>
/// win32、posix
/// </summary>
/// <param name="mingw_path"></param>
/// <returns></returns>
wstring Page2::check_mingw_thread(wstring cmdOut)
{
	wsmatch ms;
	wregex rex(L"Thread model.\\s*?posix", regex_constants::icase);
	bool r = regex_search(cmdOut, ms, rex);
	if (r)
		return _T_POSIX;
	else
	{
		wregex rex2(L"Thread model.\\s*?win32", regex_constants::icase);
		r = regex_search(cmdOut, ms, rex2);
		if (r)
			return _T_POSIX;
		else
			return _T_MCF;
	}
}

/// <summary>
/// mingw64-...-rt-..  \lib\ 同时存在 ucrt、msvcrt 也能支持 easyx
/// mingw64-...-ucrt-.. \lib\ 也可能同时存在两者，但是不支持 easyx，改为检测 gcc -v 输出结果
/// </summary>
/// <param name="mingw_path"></param>
/// <returns></returns>
wstring Page2::check_mingw_runtime(wstring cmdOut)
{
	wsmatch ms;
	wregex rex(L"ucrt", regex_constants::icase);
	bool r = regex_search(cmdOut, ms, rex);
	return r ? _LIBUCRT : _LIBMSVCRT;
}

void Page2::check_mingw(EMingWGroups* ep)
{
	if (_waccess((ep->mingw_path + ep->path_h).c_str(), 0) != 0) {
		//ep->mingw_path = L"";
		ep->path_h = L"";
	}

	if (_waccess((ep->mingw_path + ep->path_lib64).c_str(), 0) != 0) {
		//ep->mingw_path = L"";
		ep->path_lib64 = L"";
	}

	if (_waccess((ep->mingw_path + ep->path_lib32).c_str(), 0) != 0) {
		//	ep->mingw_path = L"";
		ep->path_lib32 = L"";
	}

	if (ep->path_lib32 != L"" && ep->path_lib64 != L"")
		ep->both_32_64 = true;
}

int Page2::FindSDK(wstring path, int identity, VSIDE* vec, bool g_bX64)
{
	// 首次运行初始化时，进入该逻辑
	if (path == L"")
	{
		path = reg.GetMingWPath(identity, g_bX64);
		int result = analysis_mingw(path, identity, vec);

		// 必须在递归循环 FindSDK() 外添加 not_exist_list
		if (result == NOTFOUND)
		{
			VSIDE* item = new VSIDE(mingw_Groups[identity]->name.c_str(), L"", L"", identity, false, MINGW);
			not_exist_list.push_back(item);
		}
		return result;
	}


	// 如果是 debcpp 5.4.0 32位版本，需要使用特定的 lib
	// 查找当前目录是否有 devcpp.exe
	// 如果没有，则返回上一层继续查找，不进入子目录
	bool repeat = false;
	bool use_devcpp_5_4 = false;
	if (identity == DEVCPP || identity == OTHER_IDE)
	{
		wregex rex(L"^.*?devcpp.*?\\.exe$", regex_constants::icase);
		wstring file  = find_file(path, rex);
		if (file != L"")
		{
			int ver = exeVersion(file);
			if (ver == NOTFOUND)
				return NOTFOUND;

			int a = ver / 100;
			int b = ver % 100;
			if (a == 5 && b == 4)
				use_devcpp_5_4 = true;
		}
	}
	// c-free 也需要使用特定版本
	if (identity == CFREE)
		use_devcpp_5_4 = true;

	int re = analysis_mingw(path, identity, vec, use_devcpp_5_4);
	if (re == OK || re == ERROR_1)
		return re;

	filesystem::path pp = path.c_str();
	filesystem::path pr = safe_get_parent(pp);

	if (_wcsicmp(pp.root_path().c_str(), pr.c_str()) != 0)
		return FindSDK(pr, identity, vec, g_bX64);

	return NOTFOUND;
}


/// <summary>
/// clion:  注册表路径 + /bin/mingw../bin/gcc.exe
/// codeblocks: 注册表路径 + /mingw../bin/gcc.exe
/// devcpp: 注册表路径 + /mingw64/bin/gcc.exe
/// </summary>
/// <param name="p"></param>
/// <param name="id"></param>
/// <param name="vec"></param>
/// <returns></returns>
int Page2::analysis_mingw(wstring p, int id, VSIDE* vec, bool is_dev)
{
	if (p == L"")
		return NOTFOUND;

	if (p[lstrlenW(p.c_str()) - 1] != L'\\')
		p += L"\\";

	EMingWGroups* ep = mingw_Groups[id];

	//// 如果当前目录 p 下有 \bin\gcc.exe，继续后面操作
	//// 如果没有，返回上层目录继续查找
	//wregex rex_gcc(L"^gcc.exe$", regex_constants::icase);
	//bool result = find_file(p + L"bin\\", rex_gcc);
	//if (!result)
	//	return NOTFOUND;

	// 是否存在 \\**mingw** 文件夹
	wregex rex(L"^.*?mingw.*?$", regex_constants::icase);
	wstring mingw_folder = findFolder(p, rex);

	// 如果没有发现 mingwxxx 文件夹，反而发现 bin，则进入查找它
	// 因为 Clion 中的 mingw 是在 bin 内
	wstring bin_folder = findFolder(p, L"bin");
	if (mingw_folder == L"" && bin_folder != L"")
		mingw_folder = findFolder(bin_folder, rex);

	if (mingw_folder != L"")
	{
		wstring x86_64_path = findFolder(mingw_folder, L"x86_64-w64-mingw32");		// 内部可能有 lib/lib32 同时支持 32/64 位
		wstring i686_path = findFolder(mingw_folder, L"i686-w64-mingw32");		// 也有可能存在 lib/lib64 两个
		wstring mingw32_path = findFolder(mingw_folder, L"mingw32");
		wstring mingw_path = L"";
		int type;

		wstring path_1, path_2;
		if (x86_64_path != L"")
		{
			ep->path_lib64 = L"lib\\";
			if(id != CODEBLOCKS)			// 没有32位
				ep->path_lib32 = L"lib32\\";

			path_1 = L"头文件路径 " + x86_64_path + ep->path_h;
			path_2 = L"库文件路径";
			if (ep->path_lib32 != L"")
				path_2 += L" " + x86_64_path + ep->path_lib32;
			if (ep->path_lib64 != L"")
				path_2 += L" " + x86_64_path + ep->path_lib64;

			type = 64;
			mingw_path = x86_64_path;
		}
		else if (i686_path != L"")
		{
			ep->path_lib64 = L"lib64\\";
			if (id != CODEBLOCKS)			// 没有32位
				ep->path_lib32 = L"lib\\";

			path_1 = L"头文件路径 " + i686_path + ep->path_h;
			path_2 = L"库文件路径";
			if (ep->path_lib32 != L"")
				path_2 += L" " + i686_path + ep->path_lib32;
			if (ep->path_lib64 != L"")
				path_2 += L" " + i686_path + ep->path_lib64;

			type = 32;
			mingw_path = i686_path;
		}
		else if (mingw32_path != L"")
		{
			ep->use_devcpp_5_4_x86 = is_dev;		// 只有 32 位需要使用特定的 lib

			// 如果 mingw32 下有 include 文件，说明是安装在此处，如果没有则安装在 mingw32 的上一层目录
			// 5.6  5.7  版本是这样的
			wstring include_path = findFolder(mingw32_path, L"include");	// 如果有 include 文件夹，那么 mingw 安装在这里

			if (include_path == L"")	// 如果 mingw32 下没有 include
				mingw_path = mingw_folder;
			else
				mingw_path = mingw32_path;

			ep->path_lib64 = L"";
			ep->path_lib32 = L"lib\\";
			path_1 = L"头文件路径 " + mingw_path + ep->path_h;
			path_2 = L"库文件路径 " + mingw_path + ep->path_lib32;
			type = 32;
		}
		else
			return NOTFOUND;

		if (mingw_path != L"")
		{
			// 如果是手动选择目录，会进入该逻辑
			if (vec != NULL)
			{
				vec->path_1 = path_1;
				vec->path_2 = path_2;
			}
			else	// 初始化自动检测时，进入该逻辑
			{
				vec = new VSIDE(mingw_Groups[id]->name.c_str(), path_1, path_2, id, true, MINGW);
				exist_list.push_back(vec);
			}
		}
		else
			return NOTFOUND;

		ep->mingw_path = mingw_path;

		wstring cmd = L"\"" + mingw_folder + L"bin\\gcc\" -v";
		string str = ReadProcessOutput(cmd);
		wstring ver_info = ANSIToUnicode(str.c_str());

		wstring ex = check_mingw_exception(mingw_folder + L"bin\\", ver_info);
		wstring thread = check_mingw_thread(ver_info);
		wstring runtime = check_mingw_runtime(ver_info);

		check_mingw(ep);

		int r = Support(vec, type, ex, runtime, thread);
		return r;
	}
	return NOTFOUND;
}

/// <summary>
/// 支持类型：看项目文档 \doc\
/// </summary>
/// <param name="ep"></param>
/// <param name="type"></param>
/// <param name="thread"></param>
/// <param name="exception"></param>
/// <param name="runtime"></param>
/// <returns></returns>
int Page2::Support(VSIDE* vec, int type, wstring exception, wstring runtime, wstring thread)
{
	const wchar_t* e = exception.c_str();
	const wchar_t* r = runtime.c_str();
	const wchar_t* t = thread.c_str();

	// 允许安装，但提示可能不支持
	if (e == L"")
	{
		wstring er = L"当前 mingw64";
		er += t;
		er += r;
		er += L" 可能不支持";
		vec->msg = er;
		//vec->exist = false 允许显示安装
		return NOSURE;
	}

	// ucrt 一定不支持
	if (_wcsicmp(r, _LIBUCRT) == 0)
	{
		vec->msg = L"（不支持的 mignw64_xxx_ucrt 版本）";
		vec->exist = false;
		return ERROR_1;
	}

	EMingWGroups* ep = mingw_Groups[vec->id];

	// 64位、seh、msvcrt 一定支持
	if (_wcsicmp(e, _SEH) == 0 && (type == 64 || ep->both_32_64))
		return OK;

	// (x32|i686)、sjlj、msvcrt 一定支持
	// (x32|i686)、dwarf、msvcrt 一定支持
	if ((_wcsicmp(e, _SJLJ) == 0 || _wcsicmp(e, _DWARF) == 0) && (type == 32 || ep->both_32_64))
		return OK;

	wstring err = L"不支持的 ";
	if (type == 64)
		err += L"x86_64";
	else
		err += L"i686";
	err += t;
	err += e;
	err += r;
	vec->msg = err;
	vec->exist = false;

	return ERROR_1;
}

/// <summary>
/// 
/// </summary>
/// <param name="path"></param>
wstring Page2::find_file(wstring path, wregex rex)
{
	if (path[lstrlenW(path.c_str()) - 1] != L'\\')
		path += L"\\";

	wstring searchPath = path + L"*";;
	WIN32_FIND_DATAW findData;
	HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return L"";

	wsmatch ms;
	do {
		// 跳过 "." 和 ".."
		if (wcscmp(findData.cFileName, L".") == 0 ||
			wcscmp(findData.cFileName, L"..") == 0) {
			continue;
		}

		wstring fc = findData.cFileName;
		bool r = regex_search(fc, ms, rex);
		if (r)
			return path + fc;
	} while (FindNextFileW(hFind, &findData) != 0);
	FindClose(hFind);

	return L"";
}

/// <summary>
/// 传入参数： F:\\xxx\\..\\gcc.exe -v  获取mingw 版本，路径有空格需要加双引号
/// </summary>
/// <param name="command"></param>
/// <returns></returns>
string Page2::ReadProcessOutput(const wstring& command)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hReadPipe, hWritePipe;
	PROCESS_INFORMATION pi;
	STARTUPINFOW si;

	// 设置安全属性
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// 创建管道
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
		return "创建管道失败";
	}

	// 确保读句柄不被继承
	SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

	// 设置进程启动信息
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;	// 必须要有 STARTF_USESTDHANDLES。否则控制台无输出？？
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));

	// 准备命令行（必须可写）
	wstring cmd = L"cmd.exe /c " + command;
	wchar_t* cmdLine = new wchar_t[cmd.length() + 1];
	wcscpy(cmdLine, cmd.c_str());

	// 创建进程
	if (!CreateProcessW(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		delete[] cmdLine;
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		return "创建进程失败: " + to_string(GetLastError());
	}

	delete[] cmdLine;
	CloseHandle(hWritePipe);  // 在子进程中已继承，这里关闭

	// 读取输出
	string output;
	DWORD bytesRead;
	char buffer[4096];
	BOOL success = FALSE;

	while (true)
	{
		success = ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
		if (!success || bytesRead == 0)
			break;

		buffer[bytesRead] = '\0';
		output += buffer;
	}

	// 等待进程结束
	WaitForSingleObject(pi.hProcess, INFINITE);

	// 清理
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return output;
}