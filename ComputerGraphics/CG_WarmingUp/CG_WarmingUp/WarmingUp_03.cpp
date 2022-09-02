#include <iostream>
#include <random>

using namespace std;

// 3. 도형 간의 충돌 체크

typedef struct Rect
{
	int left;
	int top;
	int right;
	int bottom;
}RECT;

enum DIR { LEFT, RIGHT, UP, DOWN };

void InitializeRect(RECT&);
void PrintRect(RECT&, RECT&);
void MoveRect(RECT&, RECT&, DIR);
bool IntersectRect(RECT&, RECT&);

int main()
{
	// 2개 사각형 생성, 좌표값 설정
	RECT rect1, rect2;
	char input;

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
			MoveRect(rect1, rect2, DIR::UP);
			break;
		case 'a': case 'A':
			MoveRect(rect1, rect2, DIR::LEFT);
			break;
		case 's': case 'S':
			MoveRect(rect1, rect2, DIR::DOWN);
			break;
		case 'd': case 'D':
			MoveRect(rect1, rect2, DIR::RIGHT);
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
	}
}

void InitializeRect(RECT& rect)
{
	random_device rd;
	uniform_int_distribution<int> dis1(0, 450);

	rect.left = dis1(rd);
	rect.top = dis1(rd);

	uniform_int_distribution<int> rightDis(rect.left + 50, 500);
	uniform_int_distribution<int> bottomDis(rect.top + 50, 500);

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

void MoveRect(RECT& r1, RECT& r2, DIR dir)
{
	// 범위 0 ~ 500
	int dis = 40;
	bool isError = false;

	switch (dir)
	{
	case DIR::LEFT:
		if (r2.left - dis >= 0)
		{
			r2.left -= dis;
			r2.right -= dis;
		}
		else isError = true;
		break;
	case DIR::RIGHT:
		if (r2.right + dis <= 500)
		{
			r2.left += dis;
			r2.right += dis;
		}
		else isError = true;
		break;
	case DIR::UP:
		if (r2.top - dis >= 0)
		{
			r2.top -= dis;
			r2.bottom -= dis;
		}
		else isError = true;
		break;
	case DIR::DOWN:
		if (r2.bottom + dis <= 500)
		{
			r2.top += dis;
			r2.bottom += dis;
		}
		else isError = true;
		break;
	}

	if (isError) cout << endl << "!!!! ERROR !!!!" << endl;

	PrintRect(r1, r2);
	if (IntersectRect(r1, r2))
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