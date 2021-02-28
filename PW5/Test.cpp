#include "Test.h"

Test::Test(istream& text)
{
	if (!text)
		throw std::invalid_argument("Test loading error, data reading error");

	while (1) {
		Question q;

		std::getline(text, q.question);

		text >> q.answer;
		--q.answer;

		if (text.fail()) {
			_questions.clear();
			throw std::invalid_argument("Test loading error, number reading error");
		}
		text.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		while (!text.eof()) {
			string tanswer;
			std::getline(text, tanswer);
			if (tanswer.empty())
				break;
			q.answers.push_back(tanswer);
		}

		if (q.answers.size() >= q.answer)
			throw std::invalid_argument("Test loading error, the answer is greater than the number of answers");


		if (text.eof()) {
			_questions.push_back(q);
			break;
		}
		if (text.good())
			_questions.push_back(q);
		else
			throw std::invalid_argument("Test loading error");
	}
}

string readline(char*& pos) {
	string s;

	char* p = pos;
	char* nextline;
	for (;; p++) {
		if (*p == '\r') {
			nextline = (*(p + 1) == '\n') ? p + 2 : p + 1;
			break;
		}
		if (*p == '\n') {
			nextline = p + 1;
			break;
		}
		if (*p == '\0') {
			nextline = p;
			break;
		}
	}

	s = string(pos, p - pos);
	pos = nextline;
	return s;
}

Test::Test(char* text)
{
	while (1) {
		Question q;

		q.question = readline(text);

		q.answer = std::stoi(readline(text)) - 1;

		while (*text != '\0') {
			string tanswer = readline(text);
			if (tanswer.empty())
				break;
			q.answers.push_back(tanswer);
		}

		if (q.answers[0].empty())
			throw std::invalid_argument("Test loading error, the question must have at least one answer");

		if (q.answers.size() <= q.answer)
			throw std::invalid_argument("Test loading error, the answer is greater than the number of answers");


		if (*text == '\0') {
			_questions.push_back(q);
			break;
		}
		_questions.push_back(q);
	}
}

size_t Test::count()
{
	return _questions.size();
}

Question& Test::operator[](const int index)
{
	return _questions[index];
}


#include <iostream>
using std::cout;
using std::cin;
using std::endl;

int RunTest(Test& t) {
	int score = 0;

	for (int i = 0; i < t.count(); i++) {
		Question& q = t[i];
		cout << i + 1 << ". " << q.question << endl;

		for (int j = 0; j < q.answers.size(); j++) {
			cout << " " << j + 1 << ") " << q.answers[j] << endl;
		}

		int answer = 0;
		while (1) {
			cout << "\n_> ";
			if (!(cin >> answer))
				cin.clear();
			cin.ignore(INT_MAX, '\n');

			if (answer >= 1 && answer <= q.answers.size())
				break;
			else {
				cout << "Ошибка, число должно быть в пределах 1 - " << q.answers.size() << "." << endl;
				continue;
			}
			cout << "Ошибка, нужно ввести число."<< endl;
		}
		cout << endl;

		if (answer == q.answer + 1)
			score++;
	}

	return score;
}