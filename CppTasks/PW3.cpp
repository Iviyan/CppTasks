#include <iostream>
#include <Windows.h>

using namespace std;

void WriteColor(const char* text, WORD color);
bool GetConsoleColor(WORD& ret);

HANDLE hConsole;

const int DiscplinesCount = 7;
const int FacultiesCount = 6;

const char* Discplines[DiscplinesCount] = { "Математика", "Физика", "Информатика", "Русский язык", "Физ-ра", "Биология", "Английский язык" };

struct Faculty {
	const char* name;
	float minMark;
};

Faculty Faculties[FacultiesCount] = {
	{ "Сетевое и системное администрирование", 4.55f },
	{ "Информационные системы и программирование", 5 },
	{ "Обеспечение информационной безопасности автоматизированных систем", 4.77f },
	{ "Инфокоммуникационные сети и системы связи", 4.44f },
	{ "Оптические и оптико-электронные приборы и системы", 4.43f },
	{ "Дворник", 2 },
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string F, I, O;
	cout << "Фамилия: "; cin >> F;
	cout << "Имя: "; cin >> I;
	cout << "Отчество: "; cin >> O;

	int marks[DiscplinesCount];

	cout << "\nВведите оценки по предметам:" << endl;
	for (int i = 0; i < DiscplinesCount; i++) {
		while (1) {
			cout << Discplines[i] << ": ";
			if (!(cin >> marks[i]))
				cin.clear();
			cin.ignore(INT_MAX, '\n');

			if (marks[i] >= 2 && marks[i] <= 5)
				break;
		}
	}

	float averageScore = 0;
	for (auto mark : marks)
		averageScore += mark;
	averageScore /= DiscplinesCount;

	cout << "\nСр. балл: " << averageScore << endl;

	cout << "\nВарианты:" << endl;

	int choices = 0;
	for (int i = 0; i < FacultiesCount; i++) {
		auto faculty = Faculties[i];

		cout << i << "> " << faculty.name << " - ";

		if (averageScore >= faculty.minMark) {
			WriteColor("вы проходите", FOREGROUND_GREEN);
			choices++;
		}
		else {
			WriteColor("вы не проходите", FOREGROUND_RED);
		}
		cout << endl;
	}

	if (choices == 0) {
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
			if (averageScore >= Faculties[selectedFaculty].minMark)
				break;
			else
				cout << "Вы не можете поступить на данный факультет" << endl;
		}
		else {
			cout << "Ошибка, число находится вне диапазона номеров факультетов" << endl;
		}
	}
	cout << "\nВы, " << F << " " << I << " " << O << ", успешно зачислены на факультет «" << Faculties[selectedFaculty].name << "»" << endl;
}


void WriteColor(const char* text, WORD color) {
	WORD currentColor;
	GetConsoleColor(currentColor);
	SetConsoleTextAttribute(hConsole, color);
	cout << text;
	SetConsoleTextAttribute(hConsole, currentColor);

}

bool GetConsoleColor(WORD& ret) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(hConsole, &info))
		return false;
	ret = info.wAttributes;
	return true;
}