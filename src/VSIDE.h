#pragma once
#include <string>
using namespace std;


class VSIDE
{
public:
	int id;			// 对应 eGroups 下标
	wstring name;
	wstring path_1;
	wstring path_2;
	bool checked;		// 是否勾选安装
	bool installed;		// 是否安装成功
	wstring msg;
	bool exist;
	BYTE type;			// 是 【0】vs 还是 【1】devcpp...其它

	VSIDE(wstring n, wstring p1, wstring p2, int i, bool e, BYTE t) {
		name = n;
		path_1 = p1;
		path_2 = p2;
		id = i;
		checked = false;
		installed = false;
		exist = e;
		type = t;
		msg = L"（未检测到）";
	}
	~VSIDE() {}
};