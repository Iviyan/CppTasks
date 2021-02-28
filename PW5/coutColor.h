#pragma once

#include <Windows.h>
#include <ostream>

struct ColorStruct
{
	const char* str;
	WORD color;
	ColorStruct(const char* _str, WORD _color) : str(_str), color(_color) { }
};

std::ostream& operator<<(std::ostream& o, const ColorStruct& cs);

inline ColorStruct color(const char* str, WORD color)
{
	return ColorStruct(str, color);
};

bool GetConsoleColor(WORD& ret, HANDLE hConsole);
bool GetConsoleColor(WORD& ret);