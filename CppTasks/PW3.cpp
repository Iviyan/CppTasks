#include <iostream>
#include <Windows.h>

using namespace std;

void WriteColor(const char* text, WORD color);
bool GetConsoleColor(WORD& ret);

HANDLE hConsole;

const int DiscplinesCount = 7;
const int FacultiesCount = 6;

const char* Discplines[DiscplinesCount] = { "����������", "������", "�����������", "������� ����", "���-��", "��������", "���������� ����" };

struct Faculty {
	const char* name;
	float minMark;
};

Faculty Faculties[FacultiesCount] = {
	{ "������� � ��������� �����������������", 4.55f },
	{ "�������������� ������� � ����������������", 5 },
	{ "����������� �������������� ������������ ������������������ ������", 4.77f },
	{ "�������������������� ���� � ������� �����", 4.44f },
	{ "���������� � ������-����������� ������� � �������", 4.43f },
	{ "�������", 2 },
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string F, I, O;
	cout << "�������: "; cin >> F;
	cout << "���: "; cin >> I;
	cout << "��������: "; cin >> O;

	int marks[DiscplinesCount];

	cout << "\n������� ������ �� ���������:" << endl;
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

	cout << "\n��. ����: " << averageScore << endl;

	cout << "\n��������:" << endl;

	int choices = 0;
	for (int i = 0; i < FacultiesCount; i++) {
		auto faculty = Faculties[i];

		cout << i << "> " << faculty.name << " - ";

		if (averageScore >= faculty.minMark) {
			WriteColor("�� ���������", FOREGROUND_GREEN);
			choices++;
		}
		else {
			WriteColor("�� �� ���������", FOREGROUND_RED);
		}
		cout << endl;
	}

	if (choices == 0) {
		cout << "\n� ���������, �� ������ �� ������" << endl;
		return 0;
	}

	int selectedFaculty = -1;
	while (1) {
		cout << "\n��������, ���� ��������� (������� �����): ";

		if (!(cin >> selectedFaculty)) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "������ �����" << endl;
			continue;
		};

		if (selectedFaculty >= 0 && selectedFaculty < FacultiesCount) {
			if (averageScore >= Faculties[selectedFaculty].minMark)
				break;
			else
				cout << "�� �� ������ ��������� �� ������ ���������" << endl;
		}
		else {
			cout << "������, ����� ��������� ��� ��������� ������� �����������" << endl;
		}
	}
	cout << "\n��, " << F << " " << I << " " << O << ", ������� ��������� �� ��������� �" << Faculties[selectedFaculty].name << "�" << endl;
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