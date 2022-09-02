#include <iostream>
#include <list>
using namespace std;

#define MAXSIZE 10

// 4. ���� ����Ʈ ����� (����ü ������ ���)

typedef struct Point
{
	int x;
	int y;
	int z;
}POINT;

void PrintList(list<POINT>&);

int main()
{
	list<POINT> points;
	PrintList(points);
}

void PrintList(list<POINT>& l)
{
	cout << "== LIST ==" << endl;
	for (auto iter = l.rbegin(); iter != l.rend(); ++iter)
	{
		cout << (*iter).x << "   " << (*iter).y << "   " << (*iter).z << endl;
	}
	cout << "==========" << endl;
}