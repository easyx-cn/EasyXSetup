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


class EMingWGroups
{
public:
	wstring name;
	int identity;
	wstring mingw_path;					// x86_64-w64-mingw32\\     i686-w64-mingw32\\ 路径
	wstring path_h;						// 固定位 include \\。
	wstring path_lib32, path_lib64;		// devcpp 存在 lib32\\ 文件夹
	bool use_devcpp_5_4_x86;				// devcpp5.4.0  x86 版本

	EMingWGroups(wstring n, int id)
	{
		name = n;
		identity = id;
		path_h = L"include\\";
		use_devcpp_5_4_x86 = false;
	}

	~EMingWGroups() {}
};



class EFiles
{
public:
	wstring path;
	wstring name;

	EFiles(wstring p, wstring n) { path = p, name = n; }
	~EFiles() {}
};