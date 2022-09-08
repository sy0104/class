#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// 2. ���Ͽ��� ������ �б�
void CountWords(vector<char>&, vector<char>&);
void PrintTextFile(vector<char>&);
void ReverseSentence(vector<char>&);
void ChangeWords(vector<char>&, char, char);
void ChangeWordsByInterval(vector<char>&, bool);

int main()
{
	string fileName;
	int wordCnt = 0, numCnt = 0;
	bool isNum = false;

	// ���� �б�, �ܾ� ����
	cout << "input data file name: ";
	cin >> fileName;

	ifstream in{ fileName };

	vector<char> textFile;
	vector<char> numbers;
	char c;

	while (in >> noskipws >> c)
		textFile.push_back(c);

	CountWords(textFile, numbers);
	system("pause");
	system("cls");

	// ���ɾ� �Է�
	char input;
	char oldChar = ' ', newChar = ' ';
	bool isChanged = false;
	while (true)
	{
		PrintTextFile(textFile);
		cout << "command: ";
		cin >> input;
		if (input == 'g' || input == 'G')
		{
			cout << "�ٲ� ����, ���Ӱ� �Է��� ����: ";
			cin >> oldChar >> newChar;
			cout << endl;
		}

		switch (input)
		{
		case 'd': case 'D':	// ���� ��ü�� ������
			ReverseSentence(textFile);
			break;
		case 'e': case 'E':	// ������ �������� Ư�� ���ڸ� ���� ���� ����
			if (isChanged)
			{
				ChangeWordsByInterval(textFile, false);
				isChanged = false;
			}
			else
			{
				ChangeWordsByInterval(textFile, true);
				isChanged = true;
			}
			break;
		case 'f': case 'F':

			break;
		case 'g': case 'G':	// ���� ������ Ư�� ���ڸ� �ٸ� ���ڷ� �ٲٱ�
			ChangeWords(textFile, oldChar, newChar);
			break;
		case 'h': case 'H':

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

void CountWords(vector<char>& text, vector<char>& numbers)
{
	int wordCnt = 0, numCnt = 0, capCnt = 0;
	bool isNum = false, isPush = true;
	vector<char> temp;
	vector<char> capital;

	for (auto i = text.begin(); i != text.end(); ++i)
	{
		cout << *i;	// ���� ���ڿ� ���

		// ���� ī��Ʈ(48 ~ 57)
		if (*i >= 48 && *i <= 57)
			isNum = true;

		if (isNum)
		{
			if (*i == 10) temp.push_back(' ');
			else if ((*i < 48 || *i > 57) && *i != ' ' && *i != 10) isPush = false;
			else temp.push_back(*i);

			if (i + 1 == text.end())
				temp.push_back(' ');

			if (*i == ' ' || *i == 10)
			{
				if (isPush)
				{
					for (auto j = temp.begin(); j != temp.end(); ++j)
					{
						numbers.push_back(*j);
					}
				}

				isNum = false;
				isPush = true;
				temp.clear();
			}
		}

		// �빮�� ī��Ʈ (65 ~ 90)
		auto start = i;
		if (*i >= 65 && *i <= 90)
		{
			auto j = i;
			if (i != text.begin())
			{
				for (auto j = i; ; --j)
				{
					if (*(j - 1) == ' ' || *(j - 1) == 10)
					{
						start = j;
						break;
					}

				}
			}

			for (; start != text.end(); ++start)
			{
				if (*start == ' ' || *start == 10)
				{
					capital.push_back(' ');
					break;
				}
				else capital.push_back(*start);
			}
		}

		// �ܾ� ī��Ʈ
		if (*i == ' ' || *i == 10 || i + 1 == text.end())
			wordCnt++;
	}

	if (numbers[numbers.size() - 1] == ' ')
		numbers.erase(numbers.begin() + numbers.size() - 1);
	if (capital[capital.size() - 1] == ' ')
		capital.erase(capital.begin() + capital.size() - 1);

	cout << endl << endl;
	cout << "=============== ���� ��� ===============" << endl;
	for (auto i = numbers.begin(); i != numbers.end(); ++i)
	{
		cout << *i;
		if (*i == ' ' || i + 1 == numbers.end())
			numCnt++;
	}

	cout << endl << endl;
	cout << "============== �빮�� ��� ==============" << endl;
	for (auto i = capital.begin(); i != capital.end(); ++i)
	{
		cout << *i;
		if (*i == ' ' || i + 1 == capital.end())
			capCnt++;
	}


	cout << endl << endl;
	cout << "word count: " << wordCnt - numCnt << endl;
	cout << "number count: " << numCnt << endl;
	cout << "Capital count: " << capCnt << endl;
	cout << endl;
}

void PrintTextFile(vector<char>& text)
{
	for (auto i = text.begin(); i != text.end(); ++i)
		cout << *i;

	cout << endl << endl;
}

void ReverseSentence(vector<char>& text)
{
	vector<char> t, s1, s2;
	int idx = 0;
	int row = 1;
	int start = 0;

	t = text;
	text.clear();

	for (int i = 0; i < t.size(); ++i)
		if (t[i] == 10) row++;

	for (int r = 0; r < row; ++r)
	{
		for (int i = start; i < t.size(); ++i)
		{
			if (i + 1 < t.size() && t[i] != 10 && t[i + 1] == 10)
			{
				s1.push_back(t[i]);
				start = i + 2;
				break;
			}
			else s1.push_back(t[i]);
		}

		for (auto iter = s1.rbegin(); iter != s1.rend(); ++iter)
			s2.push_back(*iter);

		for (int i = 0; i < s2.size(); ++i)
			text.push_back(s2[i]);

		text.push_back(10);
		s1.clear();
		s2.clear();
		idx = 0;
	}

	//for (auto iter = text.begin(); iter != text.end(); ++iter)
	//	cout << *iter;
	PrintTextFile(text);
	cout << endl;
}

void ChangeWordsByInterval(vector<char>& text, bool isChange)
{
	// 3���� �� 2���� @@ ����, ���鵵 ���ڷ� ���
	int cnt = 0, idx = 3;

	if (isChange)
	{
		while (idx < text.size())
		{
			text.insert(text.begin() + idx, 2, '@');
			idx += 5;
		}
	}
	else
	{
		text.erase(remove(text.begin(), text.end(), '@'), text.end());
	}

	PrintTextFile(text);
}

void ReverseByInterval(vector<char>& text)
{

}

void ChangeWords(vector<char>& text, char oldChar, char newChar)
{
	for (auto iter = text.begin(); iter != text.end(); ++iter)
	{
		if (*iter == oldChar)
			*iter = newChar;
	}

	PrintTextFile(text);
}