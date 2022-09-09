#include <iostream>
#include <random>

using namespace std;

// 4. 사각형과 사각형 간의 충돌 체크

typedef struct Rect
{
	int left;
	int top;
	int right;
	int bottom;
}RECT;

enum DIR { LEFT, RIGHT, UP, DOWN };
enum TURN { TURN_RECT1, TURN_RECT2 };

void InitializeRect(RECT&);
void PrintRect(RECT&, RECT&);
void MoveRect(RECT&, RECT&, DIR, TURN);
bool IntersectRect(RECT&, RECT&);

int main()
{
	// 2개 사각형 생성, 좌표값 설정
	RECT rect1, rect2;
	char input;
	int curTurn = 0;
	TURN turn = TURN::TURN_RECT1;

	InitializeRect(rect1);
	InitializeRect(rect2);
	PrintRect(rect1, rect2);

	while (true)
	{
		cout << "Command: ";
		cin >> input;

		switch (input)
		{
		case 'w': case 'W':
			if (curTurn % 2 == 0) MoveRect(rect1, rect2, DIR::UP, TURN::TURN_RECT1);
			else MoveRect(rect2, rect1, DIR::UP, TURN::TURN_RECT2);
			break;
		case 'a': case 'A':
			if (curTurn % 2 == 0) MoveRect(rect1, rect2, DIR::LEFT, TURN::TURN_RECT1);
			else MoveRect(rect2, rect1, DIR::LEFT, TURN::TURN_RECT2);
			break;
		case 's': case 'S':
			if (curTurn % 2 == 0) MoveRect(rect1, rect2, DIR::DOWN, TURN::TURN_RECT1);
			else MoveRect(rect2, rect1, DIR::DOWN, TURN::TURN_RECT2);
			break;
		case 'd': case 'D':
			if (curTurn % 2 == 0) MoveRect(rect1, rect2, DIR::RIGHT, TURN::TURN_RECT1);
			else MoveRect(rect2, rect1, DIR::RIGHT, TURN::TURN_RECT2);
			break;
		case 'r': case 'R':
			system("cls");
			InitializeRect(rect1);
			InitializeRect(rect2);
			PrintRect(rect1, rect2);
			break;
		default:
			continue;
		}
		curTurn++;
	}
}

void InitializeRect(RECT& rect)
{
	random_device rd;
	uniform_int_distribution<int> disX(0, 750);
	uniform_int_distribution<int> disY(0, 550);

	rect.left = disX(rd);
	rect.top = disY(rd);

	uniform_int_distribution<int> rightDis(rect.left + 50, 800);
	uniform_int_distribution<int> bottomDis(rect.top + 50, 600);

	rect.right = rightDis(rd);
	rect.bottom = bottomDis(rd);
}

void PrintRect(RECT& r1, RECT& r2)
{
	cout << endl;
	cout << "Shape 1: (" << r1.left << ", " << r1.top << ") ("
		<< r1.right << ", " << r1.bottom << ")" << endl;

	cout << "Shape 2: (" << r2.left << ", " << r2.top << ") ("
		<< r2.right << ", " << r2.bottom << ")" << endl;
	cout << endl;
}

void MoveRect(RECT& moveRect, RECT& otherRect, DIR dir, TURN turn)
{
	// 범위 0 ~ 500
	int dis = 50;
	bool isError = false;

	if (turn == TURN_RECT1) cout << "RECT1 Move" << endl;
	else cout << "RECT2 Move" << endl;

	switch (dir)
	{
	case DIR::LEFT:
		if (moveRect.left - dis >= 0)
		{
			moveRect.left -= dis;
			moveRect.right -= dis;
		}
		else isError = true;
		break;
	case DIR::RIGHT:
		if (moveRect.right + dis <= 800)
		{
			moveRect.left += dis;
			moveRect.right += dis;
		}
		else isError = true;
		break;
	case DIR::UP:
		if (moveRect.top - dis >= 0)
		{
			moveRect.top -= dis;
			moveRect.bottom -= dis;
		}
		else isError = true;
		break;
	case DIR::DOWN:
		if (moveRect.bottom + dis <= 600)
		{
			moveRect.top += dis;
			moveRect.bottom += dis;
		}
		else isError = true;
		break;
	}

	if (isError) cout << endl << "!!!! ERROR !!!!" << endl;

	if (turn == TURN_RECT1) PrintRect(moveRect, otherRect);
	else if (turn == TURN_RECT2) PrintRect(otherRect, moveRect);

	if (IntersectRect(moveRect, otherRect))
	{
		cout << "Rectangle 1 & Rectangle 2 collide !!" << endl;
	}
}

bool IntersectRect(RECT& r1, RECT& r2)
{
	if (r1.left <= r2.right && 
		r1.right >= r2.left &&
		r1.top <= r2.bottom && 
		r1.bottom >= r2.top) 
		return true;

	return false;
}