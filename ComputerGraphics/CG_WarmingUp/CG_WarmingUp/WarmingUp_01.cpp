#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
using namespace std;


// 1. 행렬 다루기
// - 3 X 3 행렬의 덧셈, 뺄셈, 곱셈하는 프로그램을 구한다.
// - 연산을 할 2개의 행렬에는 0 또는 1의 값을 랜덤하게 설정한다.
// - 종료 명령어를 입력할 때까지 명령을 연속적으로 수행할 수 있도록 한다.

void CreateMatrix(vector<vector<int>>&);
void PrintMatrix(vector<vector<int>>&);
void ClearResMatrix(vector<vector<int>>&);

void Command_M(vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&);
void Command_A(vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&);
void Command_D(vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&);
void Command_R(vector<vector<int>>&, vector<vector<int>>&);
void Command_T(vector<vector<int>>&, vector<vector<int>>&);
void Commnad_H(vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&);
void Command_S(vector<vector<int>>&, vector<vector<int>>&);

int main()
{
	vector<vector<int>> mat1(3, vector<int>(3));
	vector<vector<int>> mat2(3, vector<int>(3));
	vector<vector<int>> resMat(3, vector<int>(3, 0));

	char input;

	// 행렬 랜덤값 부여
	CreateMatrix(mat1);
	CreateMatrix(mat2);
	
	// 명령어 입력
	while (true)
	{
		cout << "======== MATRIX 1 ========" << endl;
		PrintMatrix(mat1);
		cout << "======== MATRIX 2 ========" << endl;
		PrintMatrix(mat2);

		cout << "명령어 입력: ";
		cin >> input;

		switch (input)
		{
		case 'm': case 'M':	// 행렬의 곱셈
			Command_M(mat1, mat2, resMat);
			break;
		case 'a': case 'A':	// 행렬의 덧셈
			Command_A(mat1, mat2, resMat);
			break;
		case 'd': case 'D':	// 행렬의 뺄셈
			Command_D(mat1, mat2, resMat);
			break;
		case 'r': case 'R':	// 행렬식의 값 -> 입력한 2개의 행렬과 행렬식 값을 모두 출력
			Command_R(mat1, mat2);
			break;
		case 't': case 'T':	// 전치 행렬과 그 행렬식의 값 -> 입력한 2개의 행렬에 모두 적용
			Command_T(mat1, mat2);
			break;
		case 'h': case 'H':	// 3X3 행렬을 4X4 행렬로 변환하고 그 행렬식의 값(4X4 행렬식 값) 출력
			Commnad_H(mat1, mat2, resMat);
			break;
		case 's': case 'S':	// 행렬의 값을 새로 랜덤하게 설정
			Command_S(mat1, mat2);
			break;
		case 'q': case 'Q':	// 프로그램 종료
			return 0;
			break;
		default:
			continue;
		}

		system("pause");
		system("cls");
	}
}

void CreateMatrix(vector<vector<int>>& mat)
{
	random_device rd;
	uniform_int_distribution<int> dis(0, 1);

	int row = mat.size();
	int col = mat[0].size();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			mat[i][j] = dis(rd);
		}
	}
}

void PrintMatrix(vector<vector<int>>& mat)
{
	int row = mat.size();
	int col = mat[0].size();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
			cout << mat[i][j] << '\t';

		cout << endl;
	}

	cout << endl;
}

void ClearResMatrix(vector<vector<int>>& res)
{
	int row = res.size();
	int col = res[0].size();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			res[i][j] = 0;
		}
	}
}

void Command_M(vector<vector<int>>& mat1, vector<vector<int>>& mat2, vector<vector<int>>& res)
{
	// 행렬의 곱셈
	int row = mat1.size();
	int col = mat1[0].size();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			for (int k = 0; k < col; ++k)
				res[i][j] += mat1[i][k] * mat2[k][j];
		}
	}
	cout << endl;
	cout << "========= RESULT =========" << endl;
	PrintMatrix(res);
	ClearResMatrix(res);
}

void Command_A(vector<vector<int>>& mat1, vector<vector<int>>& mat2, vector<vector<int>>& res)
{
	// 행렬의 덧셈
	int row = mat1.size();
	int col = mat1[0].size();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
			res[i][j] = mat1[i][j] + mat2[i][j];
	}

	cout << endl;
	cout << "========= RESULT =========" << endl;
	PrintMatrix(res);
	ClearResMatrix(res);
}

void Command_D(vector<vector<int>>& mat1, vector<vector<int>>& mat2, vector<vector<int>>& res)
{
	// 행렬의 뺄셈
	int row = mat1.size();
	int col = mat1[0].size();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
			res[i][j] = mat1[i][j] - mat2[i][j];
	}

	cout << endl;
	cout << "========= RESULT =========" << endl;
	PrintMatrix(res);
	ClearResMatrix(res);
}

void Command_R(vector<vector<int>>& mat1, vector<vector<int>>& mat2)
{
	// 행렬식의 값
	int det1 = 0, det2 = 0;

	det1 = ((mat1[0][0] * mat1[1][1] * mat1[2][2]) + (mat1[0][1] * mat1[1][2] * mat1[2][0])
		+ (mat1[0][2] * mat1[1][0] * mat1[2][1])) - ((mat1[0][0] * mat1[1][2] * mat1[2][1])
		+ (mat1[0][1] * mat1[1][0] * mat1[2][2]) + (mat1[0][2] * mat1[1][1] * mat1[2][0]));

	det2 = ((mat2[0][0] * mat2[1][1] * mat2[2][2]) + (mat2[0][1] * mat2[1][2] * mat2[2][0])
		+ (mat2[0][2] * mat2[1][0] * mat2[2][1])) - ((mat2[0][0] * mat2[1][2] * mat2[2][1])
			+ (mat2[0][1] * mat2[1][0] * mat2[2][2]) + (mat2[0][2] * mat2[1][1] * mat2[2][0]));

	cout << endl;
	cout << "det(mat1): " << det1 << endl;
	cout << "det(mat2): " << det2 << endl;
	cout << endl;
}

void Command_T(vector<vector<int>>& mat1, vector<vector<int>>& mat2)
{
	// 전치 행렬과 그 행렬식의 값, 입력한 2개의 행렬에 모두 적용
	int row = mat1.size();
	int col = mat1[0].size();

	// mat1
	for (int i = 0; i < row; ++i)
	{
		for (int j = i; j < row; j++)
		{
			int temp = mat1[i][j];
			mat1[i][j] = mat1[j][i];
			mat1[j][i] = temp;
		}
	}

	// mat2
	for (int i = 0; i < row; ++i)
	{
		for (int j = i; j < row; j++)
		{
			int temp = mat2[i][j];
			mat2[i][j] = mat2[j][i];
			mat2[j][i] = temp;
		}
	}

	cout << endl;
	cout << "===== Transpose Mat1 =====" << endl;
	PrintMatrix(mat1);
	cout << "===== Transpose Mat2 =====" << endl;
	PrintMatrix(mat2);
	cout << endl;
}

void Commnad_H(vector<vector<int>>& mat1, vector<vector<int>>& mat2, vector<vector<int>>& res)
{
	// 3x3 행렬을 4x4 행렬로 변환하고 행려식의 값(4x4 행렬식 값) 출력

	for (int i = 0; i < 3; ++i)
	{
		mat1[i].push_back(0);
		mat2[i].push_back(0);
		res[i].push_back(0);
	}

	mat1.push_back(vector<int>(4));
	mat2.push_back(vector<int>(4));
	res.push_back(vector<int>(4));

	mat1[3][3] = mat2[3][3] = 1;

	cout << "======== MATRIX 1 ========" << endl;
	PrintMatrix(mat1);
	cout << "======== MATRIX 2 ========" << endl;
	PrintMatrix(mat2);
}

void Command_S(vector<vector<int>>& mat1, vector<vector<int>>& mat2)
{
	CreateMatrix(mat1);
	CreateMatrix(mat2);

	cout << endl;
	cout << "===== Transpose Mat1 =====" << endl;
	PrintMatrix(mat1);
	cout << "===== Transpose Mat2 =====" << endl;
	PrintMatrix(mat2);
	cout << endl;

}