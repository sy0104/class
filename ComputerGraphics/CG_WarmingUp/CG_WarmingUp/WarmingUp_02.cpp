#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// 2. ���Ͽ��� ������ �б�
void CountWords(vector<char>&, vector<char>&);
void PrintTextFile(vector<char>&);
void ReverseSentence(vector<char>&);
void ChangeWords(vector<char>&, char, char);

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
	{
		textFile.push_back(c);
	}

	CountWords(textFile, numbers);
	system("pause");
	system("cls");

	// ��ɾ� �Է�
	char input;
	char oldChar, newChar;
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
	int wordCnt = 0, numCnt = 0;
	bool isNum = false;

	for (auto i = text.begin(); i != text.end(); ++i)
	{
		cout << *i;	// ���� ���ڿ� ���

		// ���� ī��Ʈ
		if (*i >= 48 && *i <= 57)
			isNum = true;

		if (isNum)
		{
			if (*i == 10) numbers.push_back(' ');
			else numbers.push_back(*i);

			if (i + 1 == text.end())
				numbers.push_back(' ');

			if (*i == ' ' || *i == 10)
				isNum = false;
		}

		// �ܾ� ī��Ʈ
		if (*i == ' ' || *i == 10 || i + 1 == text.end())
			wordCnt++;
	}

	if (numbers[numbers.size() - 1] == ' ')
		numbers.erase(numbers.begin() + numbers.size() - 1);

	cout << endl << endl;
	cout << "=============== ���� ��� ===============" << endl;
	for (auto i = numbers.begin(); i != numbers.end(); ++i)
	{
		cout << *i;
		if (*i == ' ' || i + 1 == numbers.end())
		{
			numCnt++;
			int s = 0;
		}
	}

	cout << endl << endl;
	cout << "word count: " << wordCnt - numCnt << endl;
	cout << "number count: " << numCnt << endl;
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
	vector<char> s1, s2;
	int idx = 0;

	for (int i = 0; text[i] != 10; ++i)
	{
		s1.push_back(text[i]);
	}

	for (auto iter = s1.rbegin(); iter != s1.rend(); ++iter)
	{
		s2.push_back(*iter);
		cout << s2[idx++];
	}
	
	for (int i = 0; i < s2.size(); ++i)
	{
		text[i] = s2[i];
	}

	for (auto iter = text.begin(); iter != text.end(); ++iter)
		cout << *iter;

	cout << endl;
}

void ChangeWordsByInterval(vector<char>& text)
{
	// 3���� �� 2���� @@ ����, ���鵵 ���ڷ� ���
	int cnt = 0;

	for (auto iter = text.begin(); iter != text.end(); ++iter)
	{
		
	}
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