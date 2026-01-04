#pragma once
#include <string>
using namespace std;

class EGroups
{
public:
	wstring name;
	int ver;
	wstring vcpath;
	wstring path_h;
	wstring path_libx86;
	wstring path_libx64;
	int* files_h;
	int* files_libx86;
	int* files_libx64;

	EGroups(wstring n, int v, wstring vp, wstring p_h, wstring px86, wstring px64, int fh[2], int* fx86, int* fx64)
	{
		name = n;
		ver = v;
		vcpath = vp;
		path_h = p_h;
		path_libx86 = px86;
		path_libx64 = px64;
		files_h = fh;
		files_libx86 = fx86;
		files_libx64 = fx64;	// 对应全局变量 eFiles 的下标，决定使用哪两个 lib 文件
	}
	~EGroups() {}
};



class EFiles
{
public:
	wstring path;
	wstring name;

	EFiles(wstring p, wstring n) { path = p, name = n; }
	~EFiles() {}
};

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