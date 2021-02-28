#include <iostream>

using namespace std;

int** CreateMat(int y, int x) {
	int** mat = new int* [y];
	for (int i = 0; i < y; i++)
		mat[i] = new int[x];
	return mat;
}

void FillMat(int** mat, int y, int x) {
	for (int i = 0; i < y; i++)
		for (int j = 0; j < x; j++) {
			cout << "[" << i << "," << j << "] = ";
			cin >> mat[i][j];
		}
}
void WriteMat(int** mat, int y, int x) {
	for (int i = 0; i < y; i++) {
		cout << "[";
		for (int j = 0; j < x; j++) {
			cout << mat[i][j] << ((j < x - 1) ? ", " : "");
		}
		cout << "]" << endl;
	}
}

int t2021_02_17()
{
	int x = 3, y = 3;
	cout << "¬ведите размеры матрицы:" << endl;
	cout << "y: "; cin >> y;
	cout << "x: "; cin >> x;
	cout << endl;

	int** mat = CreateMat(y, x);

	cout << "¬ведите значени€ матрицы " << y << "х" << x << ":\n" << endl;
	FillMat(mat, y, x);

	int mul;
	cout << "\n¬ведите число: ";
	cin >> mul;

	cout << endl;

	//for (auto &na : mat)
	//	for (auto &n : na)
	//		n *= mul;
	for (int i = 0; i < y; i++)
		for (int j = 0; j < x; j++)
			mat[i][j] *= mul;

	WriteMat(mat, y, x);
	return 0;
}