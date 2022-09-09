#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

#define MAXSIZE 10
#define EMPTY	999

// 4. ���� ����Ʈ ����� (����ü ������ ���)

typedef struct Point
{
	int x;
	int y;
	int z;
}POINT;

void PrintList(vector<POINT>&);
void AddFront(vector<POINT>&, POINT);
void DeleteFront(vector<POINT>&);
void AddBack(vector<POINT>&, POINT);
void DeleteBack(vector<POINT>&);
void PrintListLength(vector<POINT>&);
void ClearList(vector<POINT>&);
void FarthestPoint(vector<POINT>&);
void NearestPoint(vector<POINT>&);
void SortByDistance(vector<POINT>&);

int main()
{
	vector<POINT> points(10, { EMPTY, EMPTY, EMPTY });
	char command;
	int x, y, z;

	while (true)
	{
		cout << "Command: ";
		cin >> command;
		if (command == '+' || command == 'e' || command == 'E')
			cin >> x >> y >> z;

		switch (command)
		{
		case '+':	// ����Ʈ �� ���� �Է�
			AddFront(points, { x, y, z });
			break;
		case '-':	// ����Ʈ �� ������ ����
			DeleteFront(points);
			break;
		case 'e': case 'E':	// ����Ʈ �� �Ʒ��� �Է�
			AddBack(points, { x, y, z });
			break;
		case 'd': case 'D':	// ����Ʈ �� �Ʒ����� ����
			DeleteBack(points); 
			break;
		case 'l': case 'L':	// ����Ʈ�� ���� ���
			PrintListLength(points);
			break;
		case 'c': case 'C':	// ����Ʈ ���
			ClearList(points);
			break;
		case 'm': case 'M':	// �������� ���� �� �Ÿ��� �ִ� ���� ��ǥ�� ���
			FarthestPoint(points);
			break;
		case 'n': case 'N':	// �������� ���� ����� �Ÿ��� �ִ� ���� ��ǥ�� ���
			NearestPoint(points);
			break;
		case 's': case 'S':	// �������� �Ÿ��� �����Ͽ� ���������Ͽ� ���, �ε��� 0������ ��ĭ���� ����
			SortByDistance(points);
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

void PrintList(vector<POINT>& points)
{
	cout << "========== LIST ==========" << endl;
	int idx = 9;

	for (auto iter = points.rbegin(); iter != points.rend(); ++iter)
	{
		cout << idx-- << " | ";
		if ((*iter).x != EMPTY)
			cout << '\t' << (*iter).x << '\t' << (*iter).y << '\t' << (*iter).z << endl;
		else cout << endl;
	}
	cout << "=========================" << endl;
}

void AddFront(vector<POINT>& points, POINT p)
{
	bool isFull = true;

	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		if ((*iter).x == EMPTY)
		{
			(*iter).x = p.x;
			(*iter).y = p.y;
			(*iter).z = p.z;
			isFull = false;
			break;
		}
	}

	PrintList(points);
	if (isFull) cout << "�� �ڸ��� �����ϴ�." << endl;
}

void DeleteFront(vector<POINT>& points)
{
	bool isEmpty = true;

	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		if ((*iter).x != EMPTY)
		{
			(*iter).x = EMPTY;
			(*iter).y = EMPTY;
			(*iter).z = EMPTY;
			isEmpty = false;
			break;
		}
	}

	if (isEmpty) cout << "����Ʈ�� ����ֽ��ϴ�." << endl;
	PrintList(points);
}

void AddBack(vector<POINT>& points, POINT p)
{
	bool isFull = true;
	for (auto iter = points.rbegin(); iter != points.rend(); ++iter)
	{
		if ((*iter).x == EMPTY)
		{
			(*iter).x = p.x;
			(*iter).y = p.y;
			(*iter).z = p.z;
			isFull = false;
			break;
		}
	}

	if (isFull) cout << "�� �ڸ��� �����ϴ�." << endl;
	PrintList(points);
}

void DeleteBack(vector<POINT>& points)
{
	bool isEmpty = true;

	for (auto iter = points.rbegin(); iter != points.rend(); ++iter)
	{
		if ((*iter).x != EMPTY)
		{
			(*iter).x = EMPTY;
			(*iter).y = EMPTY;
			(*iter).z = EMPTY;
			isEmpty = false;
			break;
		}
	}

	if (isEmpty) cout << "����Ʈ�� ����ֽ��ϴ�." << endl;
	PrintList(points);
}

void PrintListLength(vector<POINT>& points)
{
	int cnt = 0;

	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		if ((*iter).x != EMPTY) cnt++;
	}

	cout << "����Ʈ ����: " << cnt << endl;
	PrintList(points);
}

void ClearList(vector<POINT>& points)
{
	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		(*iter).x = EMPTY;
		(*iter).y = EMPTY;
		(*iter).z = EMPTY;
	}

	PrintList(points);
}

void FarthestPoint(vector<POINT>& points)
{
	float dis = 0.f;
	vector<POINT> p;

	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		if ((*iter).x != EMPTY)
		{
			float d = sqrt(pow((*iter).x, 2) + pow((*iter).y, 2) + pow((*iter).z, 2));
			int a = 0;
			if (dis <= d)
			{
				dis = d;
			}
		}
	}

	cout << "�������� ���� �� �� (�Ÿ�: " << dis << ")" << endl;

	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		float d = sqrt(pow((*iter).x, 2) + pow((*iter).y, 2) + pow((*iter).z, 2));
		if (d == dis)
			cout << (*iter).x << '\t' << (*iter).y << '\t' << (*iter).z << endl;
	}
}

void NearestPoint(vector<POINT>& points)
{
	float dis = 9999.f;

	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		if ((*iter).x != EMPTY)
		{
			float d = sqrt(pow((*iter).x, 2) + pow((*iter).y, 2) + pow((*iter).z, 2));
			int a = 0;
			if (dis >= d)
			{
				dis = d;
			}
		}
	}

	cout << "�������� ���� ����� �� (�Ÿ�: " << dis << ")" << endl;

	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		float d = sqrt(pow((*iter).x, 2) + pow((*iter).y, 2) + pow((*iter).z, 2));
		if (d == dis)
			cout << (*iter).x << '\t' << (*iter).y << '\t' << (*iter).z << endl;
	}

}

void SortByDistance(vector<POINT>& points)
{
	// ����
	for (int i = 0; i < points.size() - 1; ++i)
	{
		for (int j = i + 1; j < points.size(); ++j)
		{
			float d1 = sqrt(pow(points[i].x, 2) + pow(points[i].y, 2) + pow(points[i].z, 2));
			float d2 = sqrt(pow(points[j].x, 2) + pow(points[j].y, 2) + pow(points[j].z, 2));
			if (points[i].x != EMPTY && points[j].x != EMPTY && d1 > d2)
			{
				POINT temp = points[j];
				points[j] = points[i];
				points[i] = temp;
			}
		}
	}

	// ��ĭ ����
	for (int i = 0; i < points.size(); ++i)
	{
		if (points[i].x == EMPTY)
		{
			for (int j = i + 1; j < points.size(); ++j)
			{
				if (points[j].x != EMPTY)
				{
					POINT temp = points[i];
					points[i] = points[j];
					points[j] = temp;
					break;
				}
			}
		}
	}
	
	PrintList(points);
}