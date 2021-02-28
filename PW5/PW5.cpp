#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include <algorithm>
#include "resource.h"
#include "Test.h"
#include "coutColor.h"

using namespace std;

LPVOID GetResource(int ID, LPCWSTR type, bool* success = NULL);
inline char* GetTextResource(int ID, bool* success = NULL);

double Average(const int* arr, int size, bool(*pred)(int) = nullptr);

int GetMark(int score, int questionsCount);
void WriteFaculties(char* options);

HANDLE hConsole;

const int DisciplinesCount = 8;
const int FacultiesCount = 6;

const char* Disciplines[DisciplinesCount] = {
	"Математика",
	"Физика",
	"Информатика",
	"Русский язык",
	"Физ-ра",
	"Биология",
	"Английский язык",
	"Литература",
};

struct Faculty {
	const char* name;
	float minMark;
	int testResID;
};

Faculty Faculties[FacultiesCount] = {
	{ "Сетевое и системное администрирование", 4.55f, IDR_TXT1 },
	{ "Информационные системы и программирование", 5, IDR_TXT2 },
	{ "Обеспечение информационной безопасности автоматизированных систем", 4.77f, IDR_TXT3 },
	{ "Инфокоммуникационные сети и системы связи", 4.44f, IDR_TXT4 },
	{ "Оптические и оптико-электронные приборы и системы", 4.43f, IDR_TXT5 },
	{ "Дворник", 2.5f, IDR_TXT6 },
};

const char o_pass = 0;
const char o_fail = -1;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string F, I, O;
	cout << "Фамилия: "; cin >> F;
	cout << "Имя: "; cin >> I;
	cout << "Отчество: "; cin >> O;

	int marks[DisciplinesCount + 1];

	cout << "\nВведите оценки по предметам:" << endl;
	for (int i = 0; i < DisciplinesCount; i++) {
		while (1) {
			cout << Disciplines[i] << ": ";
			if (!(cin >> marks[i]))
				cin.clear();
			cin.ignore(INT_MAX, '\n');

			if (marks[i] >= 2 && marks[i] <= 5)
				break;
		}
	}

	double averageScore = Average(marks, DisciplinesCount);

	cout << "\nСр. балл: " << averageScore << endl;

	char options[FacultiesCount];

	for (int i = 0; i < FacultiesCount; i++) {
		auto faculty = Faculties[i];

		if (averageScore >= faculty.minMark)
			options[i] = o_pass;
		else
		{
			int tmark = 0;
			for (int t = 3; t <= 5; t++) {
				marks[DisciplinesCount] = t;
				if (Average(marks, DisciplinesCount + 1) >= faculty.minMark) {
					tmark = t;
					break;
				}
			}
			marks[DisciplinesCount] = 0;

			options[i] = (tmark != 0) ? tmark : o_fail;
		}
	}

	cout << "\nВарианты:\n" << endl;
	WriteFaculties(options);

	if (std::all_of(options, options + FacultiesCount, [&](int x) { return x == o_fail; })) {
		cout << "\nК сожалению, вы никуда не прошли" << endl;
		return 0;
	}

	int selectedFaculty = -1;
	while (1) {
		cout << "\nВыберите, кида поступить (введите число): ";

		if (!(cin >> selectedFaculty)) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Ошибка ввода" << endl;
			continue;
		};

		if (selectedFaculty >= 0 && selectedFaculty < FacultiesCount) {
			char& option = options[selectedFaculty];
			if (option == o_pass)
				break;
			else if (option == o_fail)
				cout << "Вы не можете поступить на данный факультет" << endl;
			else {
				cout << endl;
				char* testResPos = GetTextResource(Faculties[selectedFaculty].testResID);
				Test test(testResPos);
				int score = RunTest(test);
				int questionsCount = test.count();

				int mark = GetMark(score, questionsCount);

				cout << "\nБаллы: " << score << "\nОценка: " << mark << endl;

				if (mark >= option) {
					marks[DisciplinesCount] = mark;
					break;
				}
				else {
					option = o_fail;
					cout << "\nК сожалению, вы провалили тест и не можете поступить на данный факультет, выберите другой.\n" << endl;
					WriteFaculties(options);
				}
			}
		}
		else {
			cout << "Ошибка, число находится вне диапазона номеров факультетов" << endl;
			continue;
		}

		if (std::all_of(options, options + FacultiesCount, [&](int x) { return x == o_fail; })) {
			cout << "\nБольше не осталось факультетов для поступления." << endl;
			return 0;
		}
	}
	cout << "\nВы, " << F << " " << I << " " << O << ", успешно зачислены на факультет «" << Faculties[selectedFaculty].name << "»" << endl;
}

LPVOID GetResource(int ID, LPCWSTR type, bool* success) {
	HRSRC hRes = FindResourceW(nullptr, MAKEINTRESOURCE(ID), type);
	if (hRes == NULL) {
		if (success != nullptr) *success = false;
		return NULL;
	}
	HGLOBAL hGlob = LoadResource(nullptr, hRes);
	if (hGlob == NULL) {
		if (success != nullptr) *success = false;
		return NULL;
	}
	return LockResource(hGlob);
}

inline char* GetTextResource(int ID, bool* success) {
	return (char*)GetResource(ID, L"txt", success);
}

double Average(const int* arr, int size, bool(*pred)(int))
{
	double sum = 0;
	int count = size;

	if (pred != nullptr) {
		for (int i = 0; i < size; i++) {
			if (pred(arr[i]))
				sum += arr[i];
			else
				count--;
		}
	}
	else {
		for (int i = 0; i < size; i++)
			sum += arr[i];
	}

	return sum / count;
};

int GetMark(int score, int questionsCount) {
	int m5_score = questionsCount - (questionsCount / 7);
	if (score >= m5_score) return 5;

	int m4_score = questionsCount - round(questionsCount / 3.0);
	if (score >= m4_score) return 4;

	int m3_score = questionsCount - ceil(questionsCount / 2.0);
	if (score >= m3_score) return 3;

	return 2;
}

void WriteFaculties(char* options) {
	cout << setw(70) << left << "Специальность" << "Проходной балл" << "  Возможность послупления" << endl;
	for (int i = 0; i < FacultiesCount; i++) {
		auto faculty = Faculties[i];

		ostringstream ss;
		ss << i << "> " << faculty.name;
		cout << setw(70) << left << ss.str();

		switch (options[i]) {
			case o_pass:
				cout << setw(16) << left << faculty.minMark << color("вы проходите", FOREGROUND_INTENSITY | FOREGROUND_GREEN) << endl;
				break;
			case o_fail:
				cout << setw(16) << left << faculty.minMark << color("вы не проходите", FOREGROUND_INTENSITY | FOREGROUND_RED) << endl;
				break;
			default:
				cout << setw(16) << left << faculty.minMark
					<< color(string("нужно сдать тест на ")
						.append(to_string(options[i])).c_str(), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN)
					<< endl;
				break;
		}
	}
}