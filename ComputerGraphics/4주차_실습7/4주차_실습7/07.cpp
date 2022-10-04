#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <vector>
using namespace std;

enum DIR { LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN };
enum SHAPE { SHAPE1, SHAPE2, SHAPE3, SHAPE4 };

char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid DrawScene();
GLvoid Reshape(int w, int h);
void InitBuffer();
void InitShader();
void UpdateBuffer();

GLvoid Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);
void TimerFunction(int);

void InitDirSpeed();
void Move();
void CheckCollision();
void Rotate(int, DIR, SHAPE);

DIR dir[4] = { LEFTUP, };
GLfloat speed[4] = { 0.0, };

GLuint width, height;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

GLchar* vertexSource;	// 소스코드 저장 변수
GLchar* fragmentSource;	// 소스코드 저장 변수

GLuint VBO[2];
GLuint s_program;

GLfloat triShape[12][3] = { //--- 삼각형 위치 값
	{ -0.6, 0.3, 0.0 }, { -0.4, 0.3, 0.0 }, { -0.5, 0.7, 0.0 },
	{ 0.4, 0.3, 0.0 }, { 0.6, 0.3, 0.0 }, { 0.5, 0.7, 0.0 },
	{ -0.6, -0.7, 0.0 }, { -0.4, -0.7, 0.0 }, { -0.5, -0.3, 0.0 },
	{ 0.4, -0.7, 0.0 }, { 0.6, -0.7, 0.0 }, { 0.5, -0.3, 0.0 }
};

GLfloat colors[12][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },	// red
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },	// green
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },	// blue
	{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }		// yellow
};

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	width = 800;
	height = 600;

	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();
	InitDirSpeed();

	glutDisplayFunc(DrawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);	// 키보드 입력 콜백함수 지정
	glutMouseFunc(Mouse);	// 마우스 함수 지정
	glutTimerFunc(10, TimerFunction, 1);	// 타이머 함수 설정
	glutMainLoop();
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

//--- 버텍스 세이더 객체 만들기
void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");	// vs 읽어오기

	// 버텍스 셰이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// 셰이더 코드를 셰이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);

	// 버텍스 셰이더 컴파일하기
	glCompileShader(vertexShader);

	// 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패" << endl << errorLog << endl;
		//return false;
	}
}

//--- 프래그먼트 세이더 객체 만들기
void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기

	// 프래그먼트 셰이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// 셰이더 코드를 셰이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);

	// 프래그먼트 셰이더 컴파일
	glCompileShader(fragmentShader);

	// 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		//return false;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기

	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기

	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기

	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.

	return ShaderProgramID;
}

//--- 출력 콜백 함수
GLvoid DrawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	// 변경된 배경색 설정
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 렌더링 파이프라인에서 셰이더 불러오기
	glUseProgram(s_program);

	// 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 12);	// 렌더링 할 인덱스 개수(12)

	// 화면에 출력하기
	glutSwapBuffers();
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenBuffers(2, VBO);		// 2개의 VBO를 지정하고 할당하기

	// 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성(좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// 변수 diamond에서 버텍스 데이터 값을 버퍼에 복사한다.
	// triShape 배열의 사이즈: 9 * float !!!!
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	// 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성(색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	// 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3 * float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void UpdateBuffer()
{
	// 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성(좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// 변수 diamond에서 버텍스 데이터 값을 버퍼에 복사한다.
	// triShape 배열의 사이즈: 9 * float !!!!
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	// 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성(색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	// 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3 * float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	// shader program
	s_program = glCreateProgram();

	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");

	// 셰이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// shader program 사용하기
	glUseProgram(s_program);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
}

void Mouse(int button, int state, int x, int y)
{
	float mouseX = ((float)x) / (width / 2) - 1.0;
	float mouseY = -(((float)y) / (height / 2) - 1.0);
}

void TimerFunction(int value)
{
	CheckCollision();
	Move();

	UpdateBuffer();

	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

void InitDirSpeed()
{
	random_device rd;
	uniform_int_distribution<int> uid(0, 3);
	uniform_real_distribution<float> urd(0.007, 0.01);

	for (int i = 0; i < 4; ++i) {
		dir[i] = (DIR)(uid(rd));
		speed[i] = urd(rd);
		//dir[i] = DIR::LEFTUP;
		//speed[i] = 0.005;
	}
}

void Move()
{
	int idx = 0;

	for (int i = 0; i < 4; ++i) {
		idx = i * 3;
		switch (dir[i])
		{
		case DIR::LEFTDOWN:
			triShape[idx][0] -= speed[i];
			triShape[idx++][1] -= speed[i];
			triShape[idx][0] -= speed[i];
			triShape[idx++][1] -= speed[i];
			triShape[idx][0] -= speed[i];
			triShape[idx++][1] -= speed[i];
			break;
		case DIR::LEFTUP:
			triShape[idx][0] -= speed[i];
			triShape[idx++][1] += speed[i];
			triShape[idx][0] -= speed[i];
			triShape[idx++][1] += speed[i];
			triShape[idx][0] -= speed[i];
			triShape[idx++][1] += speed[i];
			break;
		case RIGHTDOWN:
			triShape[idx][0] += speed[i];
			triShape[idx++][1] -= speed[i];
			triShape[idx][0] += speed[i];
			triShape[idx++][1] -= speed[i];
			triShape[idx][0] += speed[i];
			triShape[idx++][1] -= speed[i];
			break;
		case RIGHTUP:
			triShape[idx][0] += speed[i];
			triShape[idx++][1] += speed[i];
			triShape[idx][0] += speed[i];
			triShape[idx++][1] += speed[i];
			triShape[idx][0] += speed[i];
			triShape[idx++][1] += speed[i];
			break;
		}
	}
}

void CheckCollision()
{
	int idx = 0;

	for (int i = 0; i < 4; ++i) {
		idx = i * 3;

		if (dir[i] == LEFTDOWN) {
			if (triShape[idx][0] <= -1.0) Rotate(idx, dir[i], SHAPE2);
			else if (triShape[idx][1] <= -1.0 || triShape[idx + 1][1] <= -1.0) Rotate(idx, dir[i], SHAPE4);
		}
		else if (dir[i] == LEFTUP) {
			if (triShape[idx][0] <= -1.0) Rotate(idx, dir[i], SHAPE2);
			else if (triShape[idx + 2][1] >= 1.0) Rotate(idx, dir[i], SHAPE1);
		}
		else if (dir[i] == RIGHTDOWN) {
			if (triShape[idx + 1][0] >= 1.0 || triShape[idx + 2][0] >= 1.0) Rotate(idx, dir[i], SHAPE3);
			else if (triShape[idx][1] <= -1.0 || triShape[idx + 1][1] <= -1.0) Rotate(idx, dir[i], SHAPE4);
		}
		else if (dir[i] == RIGHTUP) {
			if (triShape[idx + 1][0] >= 1.0 || triShape[idx + 2][0] >= 1.0) Rotate(idx, dir[i], SHAPE3);
			else if (triShape[idx + 2][1] >= 1.0) Rotate(idx, dir[i], SHAPE1);
		}
	}
}

void Rotate(int idx, DIR d, SHAPE changeShape)
{
	int index = idx / 3;
	int originIdx = idx;

	// 기준점
	GLfloat x, y;

	switch (changeShape)
	{
	case SHAPE::SHAPE1:
		x = triShape[idx][0];
		y = 1.0;

		triShape[idx++][1] = y;
		triShape[idx][0] = x + 0.1;
		triShape[idx++][1] = y - 0.4;
		triShape[idx][0] = x + 0.2;
		triShape[idx][1] = y;
		break;
	case SHAPE::SHAPE2:
		x = -1.0;
		y = triShape[idx][1];

		triShape[idx++][0] = x;
		triShape[idx][0] = x + 0.4;
		triShape[idx++][1] = y + 0.1;
		triShape[idx][0] = x;
		triShape[idx][1] = y + 0.2;
		break;
	case SHAPE::SHAPE3:
		x = 1.0;
		y = triShape[idx][1];

		triShape[idx++][0] = x - 0.4;
		triShape[idx][0] = x;
		triShape[idx++][1] = y - 0.1;
		triShape[idx][0] = x;
		triShape[idx][1] = y + 0.1;
		break;
	case SHAPE::SHAPE4:
		x = triShape[idx][0];
		y = -1.0;

		triShape[idx++][1] = y;
		triShape[idx][0] = x + 0.2;
		triShape[idx++][1] = y;
		triShape[idx][0] = x + 0.1;
		triShape[idx][1] = y + 0.4;
		break;
	}


	// 반대 방향 전환
	random_device rd;
	uniform_int_distribution<int> leftUid(0, 1);
	uniform_int_distribution<int> rightUid(2, 3);
	uniform_int_distribution<int> uid(0, 3);

	//if (d == LEFTDOWN || d == LEFTUP) dir[originIdx / 3] = (DIR)(rightUid(rd));
	//else dir[originIdx / 3] = (DIR)(leftUid(rd));

	while (dir[originIdx / 3] != (DIR)(uid(rd)))
		dir[originIdx / 3] = (DIR)(uid(rd));
}