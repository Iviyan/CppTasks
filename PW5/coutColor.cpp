#include "coutColor.h"

std::ostream& operator<<(std::ostream& o, const ColorStruct& cs)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD currentColor;
	GetConsoleColor(currentColor);
	SetConsoleTextAttribute(hConsole, cs.color);
	auto& ostr = o << cs.str;
	SetConsoleTextAttribute(hConsole, currentColor);
	return ostr;
}

bool GetConsoleColor(WORD& ret, HANDLE hConsole) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		return false;
	ret = info.wAttributes;
	return true;
}

bool GetConsoleColor(WORD& ret) {
	return GetConsoleColor(ret, GetStdHandle(STD_OUTPUT_HANDLE));
}