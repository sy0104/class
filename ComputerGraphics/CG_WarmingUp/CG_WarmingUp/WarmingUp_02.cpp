#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// 2. ���Ͽ��� ������ �б�

int main()
{
	string fileName;
	int wordCnt = 0, numCnt = 0;
	bool isNum = false;

	cout << "input data file name: ";
	cin >> fileName;

	ifstream in{ fileName };

	vector<char> textFile;
	vector<char> words;
	vector<char> numbers;
	char c;

	while (in >> noskipws >> c)
	{
		textFile.push_back(c);
	}

	// ���� �ƽ�Ű�ڵ� 48 ~ 57
	for (auto i = textFile.begin(); i != textFile.end(); ++i)
	{
		cout << *i;	// ���� ���ڿ� ���

		// ���� ī��Ʈ
		if (*i >= 48 && *i <= 57)
			isNum = true;

		if (isNum)
		{
			if (*i == 10) numbers.push_back(' ');
			else numbers.push_back(*i);

			if (i + 1 == textFile.end())
				numbers.push_back(' ');

			if (*i == ' ' || *i == 10)
				isNum = false;
		}

		// �ܾ� ī��Ʈ
		if (*i == ' ' || *i == 10 || i + 1 == textFile.end())
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
		}
	}
	
	cout << endl << endl;
	cout << "word count: " << wordCnt - numCnt << endl;
	cout << "number count: " << numCnt << endl;
	cout << endl;

}