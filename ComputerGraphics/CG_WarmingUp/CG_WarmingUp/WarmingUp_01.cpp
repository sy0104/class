#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
using namespace std;


// 1. ��� �ٷ��
// - 3 X 3 ����� ����, ����, �����ϴ� ���α׷��� ���Ѵ�.
// - ������ �� 2���� ��Ŀ��� 0 �Ǵ� 1�� ���� �����ϰ� �����Ѵ�.
// - ���� ��ɾ �Է��� ������ ����� ���������� ������ �� �ֵ��� �Ѵ�.

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

	// ��� ������ �ο�
	CreateMatrix(mat1);
	CreateMatrix(mat2);
	
	// ��ɾ� �Է�
	while (true)
	{
		cout << "======== MATRIX 1 ========" << endl;
		PrintMatrix(mat1);
		cout << "======== MATRIX 2 ========" << endl;
		PrintMatrix(mat2);

		cout << "��ɾ� �Է�: ";
		cin >> input;

		switch (input)
		{
		case 'm': case 'M':	// ����� ����
			Command_M(mat1, mat2, resMat);
			break;
		case 'a': case 'A':	// ����� ����
			Command_A(mat1, mat2, resMat);
			break;
		case 'd': case 'D':	// ����� ����
			Command_D(mat1, mat2, resMat);
			break;
		case 'r': case 'R':	// ��Ľ��� �� -> �Է��� 2���� ��İ� ��Ľ� ���� ��� ���
			Command_R(mat1, mat2);
			break;
		case 't': case 'T':	// ��ġ ��İ� �� ��Ľ��� �� -> �Է��� 2���� ��Ŀ� ��� ����
			Command_T(mat1, mat2);
			break;
		case 'h': case 'H':	// 3X3 ����� 4X4 ��ķ� ��ȯ�ϰ� �� ��Ľ��� ��(4X4 ��Ľ� ��) ���
			Commnad_H(mat1, mat2, resMat);
			break;
		case 's': case 'S':	// ����� ���� ���� �����ϰ� ����
			Command_S(mat1, mat2);
			break;
		case 'q': case 'Q':	// ���α׷� ����
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
	// ����� ����
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
	// ����� ����
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
	// ����� ����
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
	// ��Ľ��� ��
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
	// ��ġ ��İ� �� ��Ľ��� ��, �Է��� 2���� ��Ŀ� ��� ����
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
	// 3x3 ����� 4x4 ��ķ� ��ȯ�ϰ� ������� ��(4x4 ��Ľ� ��) ���

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