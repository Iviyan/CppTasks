#pragma once

#include <string>
#include <vector>
#include <istream>

using std::string;
using std::vector;
using std::istream;

struct Question {
	string question;
	vector<string> answers;
	unsigned int answer;
};

class Test
{
public:
	Test(istream &text);
	Test(char* text);
	Test() = delete;

	size_t count();

	Question& operator[] (const int index);

private:
	vector<Question> _questions;

};

int RunTest(Test& t);