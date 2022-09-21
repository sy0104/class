#include <iostream>
#include <vector>
#include <iomanip>
#include <Windows.h>
#include <algorithm>

using namespace std;

enum COLOR {
	BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY,
	LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};

// A - RED
// B - BLUE
// C - GREEN
// D - MAGENTA
// E - YELLOW
// F - LIGHTGRAY
// G - LIGHTGREEN
// H - BROWN

typedef struct Point
{
	int x;
	int y;
}POS;

// 7. 카드 짝 맞추기 게임 만들기
// 실행 횟수: 5
// 1번 맞추면 +1
// 5턴 후 다시시작 가능

void PrintBoard(vector<vector<char>>&);
void PrintSelectedCard(vector<vector<char>>&, char, int);
void InitializeAlphabet(vector<vector<char>>&);
void SetCompareIndex(char, int, char, int, POS&, POS&);
void CheckCards(vector<vector<char>>&, vector<vector<char>>&, POS&, POS&, int&);
void SetTextColor(int, int background = BLACK);

int main()
{
	vector<vector<char>> board(4, vector<char>(4, '*'));
	vector<vector<char>> alphabet(4, vector<char>(4));

	InitializeAlphabet(alphabet);

	char input1, input2;
	char reStart;
	int num1, num2;
	POS pos1, pos2;
	int turn = 0;
	int score = 0;

	while (true)
	{
		cout << "====== TURN " << turn + 1 << " / 5 ======" << endl << endl;
		PrintBoard(board);
		cout << "< TEST용 출력 >" << endl;
		PrintBoard(alphabet);

		cout << "input card 1: ";
		cin >> input1 >> num1;
		PrintSelectedCard(board, input1, num1);

		cout << "< TEST용 출력 >" << endl;
		PrintBoard(alphabet);
		cout << "input card 2: ";
		cin >> input2 >> num2;
		PrintSelectedCard(board, input2, num2);

		SetCompareIndex(input1, num1, input2, num2, pos1, pos2);
		CheckCards(board, alphabet, pos1, pos2, score);

		turn++;

		if (turn >= 5)
		{
			system("cls");
			cout << "<< RESULT >>" << endl;
			PrintBoard(board);
			cout << "점수: " << score << endl;
			cout << "다시시작(R): ";
			cin >> reStart;

			if (reStart == 'r' || reStart == 'R')
			{
				turn = 0;
				score = 0;

				InitializeAlphabet(alphabet);
				for (int i = 0; i < board.size(); ++i)
					for (int j = 0; j < board[0].size(); ++j)
						board[i][j] = '*';
				system("cls");
			}
			else break;
		}
	}
}

void PrintBoard(vector<vector<char>>& board)
{
	int row = board.size();
	int col = board[0].size();
	int num = 1;

	SetTextColor(WHITE);
	cout << setw(4) << "a" << setw(3) << "b" << setw(3) << "c" << setw(3) << "d" << endl;

	for (int i = 0; i < row; ++i)
	{
		SetTextColor(WHITE);
		cout << num++ << setw(3);
		for (int j = 0; j < col; ++j)
		{
			// 색상 설정
			if (board[i][j] == 'A') SetTextColor(RED);
			else if (board[i][j] == 'B') SetTextColor(BLUE);
			else if (board[i][j] == 'C') SetTextColor(GREEN);
			else if (board[i][j] == 'D') SetTextColor(MAGENTA);
			else if (board[i][j] == 'E') SetTextColor(YELLOW);
			else if (board[i][j] == 'F') SetTextColor(LIGHTGRAY);
			else if (board[i][j] == 'G') SetTextColor(LIGHTGREEN);
			else if (board[i][j] == 'H') SetTextColor(BROWN);
			else SetTextColor(WHITE);
			cout << setw(3) << board[i][j];
		}
		cout << endl;
	}
	cout << endl;

	SetTextColor(WHITE);
}

void InitializeAlphabet(vector<vector<char>>& a)
{
	// 65 ~ 72 (A ~ H)
	int alphabet = 65;
	int idx = 0;

	int row = a.size();
	int col = a[0].size();

	vector<int> v1;
	vector<int> v2;

	for (int i = 65; i <= 72; ++i)
	{
		v1.push_back(i);
		v2.push_back(i);
	}

	srand(time(0));

	random_shuffle(v1.begin(), v1.end());
	random_shuffle(v2.begin(), v2.end());

	for (int i = 0; i < row / 2; ++i)
		for (int j = 0; j < col; ++j)
			a[i][j] = v1[idx++];

	idx = 0;

	for (int i = 2; i < row; ++i)
		for (int j = 0; j < col; ++j)
			a[i][j] = v2[idx++];
}

void PrintSelectedCard(vector<vector<char>>& board, char input, int num)
{
	int row = 0, col = 0;

	if (input == 'a') col = 0;
	else if (input == 'b') col = 1;
	else if (input == 'c') col = 2;
	else if (input == 'd') col = 3;
	row = num - 1;

	board[row][col] = 'O';

	system("cls");
	PrintBoard(board);
}

void SetCompareIndex(char input1, int num1, char input2, int num2, POS& p1, POS& p2)
{
	if (input1 == 'a') p1.x = 0;
	else if (input1 == 'b') p1.x = 1;
	else if (input1 == 'c') p1.x = 2;
	else if (input1 == 'd') p1.x = 3;
	p1.y = num1 - 1;

	if (input2 == 'a') p2.x = 0;
	else if (input2 == 'b') p2.x = 1;
	else if (input2 == 'c') p2.x = 2;
	else if (input2 == 'd') p2.x = 3;
	p2.y = num2 - 1;
}

void CheckCards(vector<vector<char>>& board, vector<vector<char>>& alphabet, POS& pos1, POS& pos2, int& score)
{
	int row = alphabet.size();
	int col = alphabet[0].size();

	// 똑같은 알파벳을 선택했으면 보드의 값 변경해주기
	if (alphabet[pos1.y][pos1.x] == alphabet[pos2.y][pos2.x])
	{
		board[pos1.y][pos1.x] = board[pos2.y][pos2.x] = alphabet[pos1.y][pos1.x];
		score++;
	}

	else
	{
		// 선택 표시 원래대로
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				if (board[i][j] == 'O')
					board[i][j] = '*';
			}
		}
	}

	// 선택한 알파벳 출력

	system("cls");
	int num = 1;

	SetTextColor(WHITE);
	cout << setw(4) << "a" << setw(3) << "b" << setw(3) << "c" << setw(3) << "d" << endl;
	for (int i = 0; i < row; ++i)
	{
		SetTextColor(WHITE);
		cout << num++ << setw(3);
		for (int j = 0; j < col; ++j)
		{
			if ((j == pos1.x && i == pos1.y) || (j == pos2.x && i == pos2.y))
				cout << setw(3) << alphabet[i][j];
			else
			{
				if (board[i][j] == 'A') SetTextColor(RED);
				else if (board[i][j] == 'B') SetTextColor(BLUE);
				else if (board[i][j] == 'C') SetTextColor(GREEN);
				else if (board[i][j] == 'D') SetTextColor(MAGENTA);
				else if (board[i][j] == 'E') SetTextColor(YELLOW);
				else if (board[i][j] == 'F') SetTextColor(LIGHTGRAY);
				else if (board[i][j] == 'G') SetTextColor(LIGHTGREEN);
				else if (board[i][j] == 'H') SetTextColor(BROWN);
				cout << setw(3) << board[i][j];
			}
			SetTextColor(WHITE);
		}
		cout << endl;
	}
	cout << endl;

	system("pause");
	system("cls");
}

void SetTextColor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
