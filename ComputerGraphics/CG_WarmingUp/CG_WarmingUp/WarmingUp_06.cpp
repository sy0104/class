#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <algorithm>
#include <Windows.h>

using namespace std;

enum DIR { RIGHT, DOWN, LEFT, UP };

// 6. 경로 만들기
void PrintBoard(vector<vector<int>>&);
void CreateRoute(vector<vector<int>>&);
bool SetRouteToBoard(vector<vector<int>>&, DIR, int&, int, int);
void TextColor(int, int);
void ClearBoard(vector<vector<int>>&);
void MoveBoard(vector<vector<int>>&, DIR);

int x = 0, y = 0;

int main()
{
	vector<vector<int>> board(10, vector<int>(10, 0));
	int route = 0;
	char input;

	CreateRoute(board);

	while (true)
	{
		PrintBoard(board);
		TextColor(7, 0);
		cout << "command: ";
		cin >> input;

		switch (input)
		{
		case 'r': case 'R':	// 경로 다시 만듦
			x = y = 0;
			ClearBoard(board);
			CreateRoute(board);
			break;
		case 'a': case 'A':	// 경로 좌측으로 한칸 이동
			MoveBoard(board, DIR::LEFT);
			break;
		case 'd': case 'D':	// 경로 우측으로 한칸 이동
			MoveBoard(board, DIR::RIGHT);
			break;
		case 'q': case 'Q':
			return 0;
			break;
		default:
			continue;
		}

		system("pause");
		system("cls");
	}

}

void PrintBoard(vector<vector<int>>& board)
{
	int row = board.size();
	int col = board[0].size();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			if (board[i][j] == 0) TextColor(7, 0);
			else TextColor(12, 0);
			cout.setf(ios::left);
			cout << setw(5) << board[i][j] << setw(5);

		}
		cout << endl;
	}
	cout << endl;
}

void CreateRoute(vector<vector<int>>& board)
{
	int maxMove = 7;
	int move = 0, moveCnt = 0, route = 1;
	int dirIdx = 0;
	DIR dir;

	vector<DIR> dirVec;
	dirVec.push_back(DIR::LEFT);
	dirVec.push_back(DIR::RIGHT);
	dirVec.push_back(DIR::UP);
	dirVec.push_back(DIR::DOWN);

	// 1. 맨 처음 이동방향 결정(right or down), 그 방향으로 이동 칸수 랜덤 설정
	random_device rd;
	uniform_int_distribution<int> startDir(0, 1);
	uniform_int_distribution<int> moveDis(1, maxMove);
	dir = (DIR)(startDir(rd));
	move = moveDis(rd);

	while (dirVec[0] != dir)
		random_shuffle(dirVec.begin(), dirVec.end());

	while (true)
	{
		if (x == 9 && y == 9)
		{
			board[y][x] = route;
			break;
		}

		// 방향 재설정
		if (dirIdx > 3)
		{
			random_shuffle(dirVec.begin(), dirVec.end());
			dirIdx = 0;
		}
		moveCnt = 0;
		dir = dirVec[dirIdx++];
		move = moveDis(rd);

		SetRouteToBoard(board, dir, route, move, moveCnt);
	}

	//PrintBoard(board);
}

bool SetRouteToBoard(vector<vector<int>>& board, DIR dir, int& route, int move, int moveCnt)
{
	bool isEdge = false;

	while (moveCnt <= move)
	{
		if (isEdge) break;

		switch (dir)
		{
		case DIR::LEFT:
			if (x == 0)
			{
				board[y][x] = route;
				isEdge = true;
			}
			else board[y][x--] = route;
			break;
		case DIR::RIGHT:
			if (x == 9)
			{
				board[y][x] = route;
				isEdge = true;
			}
			else board[y][x++] = route;
			break;
		case DIR::UP:
			if (y == 0)
			{
				board[y][x] = route;
				isEdge = true;
			}
			else board[y--][x] = route;
			break;
		case DIR::DOWN:
			if (y == 9)
			{
				board[y][x] = route;
				isEdge = true;
			}
			else board[y++][x] = route;
			break;
		}
		if (!isEdge)
		{
			moveCnt++;
			route++;
		}
	}

	//PrintBoard(board);
	return isEdge;
}

void TextColor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ClearBoard(vector<vector<int>>& board)
{
	int row = board.size();
	int col = board[0].size();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			board[i][j] = 0;
		}
	}
}

void MoveBoard(vector<vector<int>>& board, DIR dir)
{
	vector<vector<int>> temp(10, vector<int>(10, 0));;
	int row = board.size();
	int col = board[0].size();

	if (dir == DIR::LEFT)
	{
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				if (j == 9) temp[i][j] = board[i][0];
				else temp[i][j] = board[i][j + 1];
			}
		}
	}

	else if (dir == DIR::RIGHT)
	{
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				if (j == 0) temp[i][j] = board[i][9];
				else temp[i][j] = board[i][j - 1];
			}
		}
	}

	board.clear();
	board = temp;
}