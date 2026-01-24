#pragma once

#include <string>
#include <map>
#include "Page1.h"
#include "Page2.h"
using namespace std;


class MyWindow
{
	nk_context* _ctx;
	int Width, Height;

	Page1* p1;
	Page2* p2;

public:
	MyWindow(nk_context*, int, int);
	~MyWindow();

	void Init();
	void Draw();
};