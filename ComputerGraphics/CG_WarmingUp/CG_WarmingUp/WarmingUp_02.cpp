#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// 2. 파일에서 데이터 읽기
// 3. 문자열 다루기 (2번 문제 확장)
void CountWords(vector<char>&, vector<char>&);
void PrintTextFile(vector<char>&);
void ReverseSentence(vector<char>&);
void ReverseByInterval(vector<char>&);
void ChangeWords(vector<char>&, char, char);
void ChangeWordsByInterval(vector<char>&, bool);
void PrintSameWords(vector<char>&);

int main()
{
	string fileName;
	int wordCnt = 0, numCnt = 0;
	bool isNum = false;

	// 파일 읽기, 단어 세기
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

	// 명령어 입력
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
			cout << "바꿀 문자, 새롭게 입력할 문자: ";
			cin >> oldChar >> newChar;
			cout << endl;
		}

		switch (input)
		{
		case 'd': case 'D':	// 문장 전체를 뒤집기
			ReverseSentence(textFile);
			break;
		case 'e': case 'E':	// 동일한 간격으로 특정 문자를 일정 개수 삽입
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
			ReverseByInterval(textFile);
			break;
		case 'g': case 'G':	// 문자 내부의 특정 문자를 다른 문자로 바꾸기
			ChangeWords(textFile, oldChar, newChar);
			break;
		case 'h': case 'H':
			PrintSameWords(textFile);
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
		cout << *i;	// 읽은 문자열 출력

		// 숫자 카운트(48 ~ 57)
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

		// 대문자 카운트 (65 ~ 90)
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

		// 단어 카운트
		if (*i == ' ' || *i == 10 || i + 1 == text.end())
			wordCnt++;
	}

	if (numbers[numbers.size() - 1] == ' ')
		numbers.erase(numbers.begin() + numbers.size() - 1);
	if (capital[capital.size() - 1] == ' ')
		capital.erase(capital.begin() + capital.size() - 1);

	cout << endl << endl;
	cout << "=============== 숫자 출력 ===============" << endl;
	for (auto i = numbers.begin(); i != numbers.end(); ++i)
	{
		cout << *i;
		if (*i == ' ' || i + 1 == numbers.end())
			numCnt++;
	}

	cout << endl << endl;
	cout << "============== 대문자 출력 ==============" << endl;
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
	bool isEnd = false;

	t = text;
	text.clear();

	for (int i = 0; i < t.size(); ++i)
		if (t[i] == 10) row++;

	for (int r = 0; r < row; ++r)
	{
		if (r == row - 1) isEnd = true;
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

		if (!isEnd) text.push_back(10);
		s1.clear();
		s2.clear();
		idx = 0;
	}

	PrintTextFile(text);
}

void ChangeWordsByInterval(vector<char>& text, bool isChange)
{
	// 3문자 후 2개의 @@ 삽입, 공백도 문자로 취급
	int cnt = 0, idx = 3;
	bool isEnter = false;

	if (isChange)
	{
		while (idx < text.size())
		{
			text.insert(text.begin() + idx, 2, '@');

			for (int i = idx; i <= idx + 5; ++i)
			{
				if (i >= text.size())
					break;

				if (text[i] == 10)
				{
					idx += 6;
					isEnter = true;
					break;
				}
			}

			if (!isEnter) idx += 5;
			isEnter = false;
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
	// 띄어쓰기 기준으로 문자 뒤집기
	int row = 1;
	int idx = 0, start = 0, end = 0;
	bool isPush = false;
	bool isEnd = false;
	vector<char> w1, w2;
	vector<char> temp = text;
	text.clear();

	for (int i = 0; i < temp.size(); ++i)
		if (temp[i] == 10) row++;

	int i = start;
	for (int r = 0; r < row;)
	{
		if (i >= temp.size()) break;
		for (i = start; i < temp.size(); ++i)
		{
			if (temp[i] == ' ' || temp[i] == 10 || i + 1 == temp.size())
			{
				if (i == temp.size() - 1)
				{
					end = i;
					isEnd = true;
				}
				else end = i - 1;
				//end = i - 1;
				isPush = true;
			}

			if (isPush)
			{
				for (; end >= start; --end)
				{
					text.push_back(temp[end]);
					int s = 0;
				}

				if (!isEnd) text.push_back(temp[i]);
				start = i + 1;
				isPush = false;
			}

			// 다음 행으로
			if (temp[i] == 10)
			{
				r++;
				break;
			}
		}
	}

	PrintTextFile(text);
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

void PrintSameWords(vector<char>& text)
{
	int row = 1;
	int idx = 0, end = 0, next = 0;
	bool isSame = false;
	bool isEnter = true;

	for (int i = 0; i < text.size(); ++i)
	{
		if (isEnter)
		{
			cout << row << "번째 줄: ";
			isEnter = false;
			row++;
		}

		if (text[i] == 10)
		{
			end = i - 1;
			isEnter = true;
		}
		else if (i == text.size() - 1)
		{
			end = i;
			int s = 0;
		}
		next = end;

		for (; idx != end;)
		{
			if (text[idx] == text[end])
			{
				cout << text[idx];
				idx++; end--;
				isSame = true;
			}
			else
			{
				idx = next + 2;
				break;
			}
		}

		if (text[i] == 10)
		{
			if (!isSame)
			{
				cout << endl;
				isSame = false;
			}
			else
				cout << endl;
		}
	}

	cout << endl;
}