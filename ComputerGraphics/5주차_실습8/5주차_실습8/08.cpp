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
void TimerFunction(int);

void InitDirSpeed();
void Move();
void CheckCollision();
void Rotate(SHAPE, GLboolean);

DIR dir = LEFTUP;
GLfloat speed[3] = { 0.0, };
GLboolean isBounce = true, isMove = true;

GLuint width, height;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

GLchar* vertexSource;	// 소스코드 저장 변수
GLchar* fragmentSource;	// 소스코드 저장 변수

GLuint VAO, VBO[2];
GLuint s_program;

GLfloat triShape[9][3] = { //--- 삼각형 위치 값
	{ -0.6, 0.0, 0.0 }, { -0.4, 0.0, 0.0 }, { -0.5, 0.3, 0.0 },		// 튕기는 삼각형
	{ -0.3, -0.34, 0.0 }, { -0.1, -0.34, 0.0 }, { -0.2, -0.09, 0.0 },		// 사각형 속 밑 삼각형
	{ 0.1, 0.34, 0.0 }, { 0.3, 0.34, 0.0 }, { 0.2, 0.09, 0.0 },	// 사각형 속 위 삼각형
};

GLfloat colors[9][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },	// red
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },	// green
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },	// blue
};

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	width = 800;
	height = 700;

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

	// 사용할 VAO 불러오기
	glBindVertexArray(VAO);

	// 중앙 사각형 그리기
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(-0.35, -0.35, 0.35, 0.35);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 9);	// 렌더링 할 인덱스 개수(9)

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
	glGenVertexArrays(1, &VAO);	// VAO를 저장하고 할당하기
	glBindVertexArray(VAO);		// VAO를 바인드하기
	glGenBuffers(2, VBO);		// 2개의 VBO를 지정하고 할당하기

	// 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성(좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// 변수 diamond에서 버텍스 데이터 값을 버퍼에 복사한다.
	// triShape 배열의 사이즈: 9 * float !!!!
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	// 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성(색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	// 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성(색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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
	// 밖의 삼각형이 튕기기를 시작/멈춤
	if (key == 'o' || key == 'O') isBounce = !isBounce;

	// 안의 삼각형 이동/멈춤
	else if (key == 'i' || key == 'I') isMove = !isMove;
}

void TimerFunction(int value)
{
	Move();
	CheckCollision();
	UpdateBuffer();

	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

void InitDirSpeed()
{
	random_device rd;
	uniform_int_distribution<int> uid(0, 3);
	uniform_real_distribution<float> urd(0.007, 0.01);

	dir = (DIR)(uid(rd));

	speed[0] = urd(rd);
	speed[1] = speed[2] = urd(rd);
	speed[2] *= -1;	// 위 삼각형은 처음에 좌측 이동
}

void Move()
{
	int idx = 0;

	for (int i = 0; i < 3; ++i) {
		idx = i * 3;

		if (isBounce && i == 0) {	// 튕기는 삼각형
			switch (dir)
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

		else if (isMove && i > 0) {	// 사각형 내부 삼각형 - 좌우이동
			triShape[idx][0] += speed[i];
			triShape[idx + 1][0] += speed[i];
			triShape[idx + 2][0] += speed[i];

			if (speed[i] >= 0.0 && triShape[idx + 1][0] >= 0.35 ||
				speed[i] <= 0.0 && triShape[idx][0] <= -0.35) {
				// 반대 방향으로 이동
				speed[i] *= -1.0;

				// 색상 변경
				GLfloat r, g, b;

				random_device rd;
				uniform_int_distribution<int> uid(0, 255);
				r = (GLfloat)(uid(rd) / 255.0f);
				g = (GLfloat)(uid(rd) / 255.0f);
				b = (GLfloat)(uid(rd) / 255.0f);

				colors[idx][0] = r;
				colors[idx][1] = g;
				colors[idx++][2] = b;
				colors[idx][0] = r;
				colors[idx][1] = g;
				colors[idx++][2] = b;
				colors[idx][0] = r;
				colors[idx][1] = g;
				colors[idx++][2] = b;
			}
		}
	}
}

void CheckCollision()
{
	GLboolean isXCollision = false, isYCollision = false;

	if (dir == LEFTDOWN) {
		// 벽면 충돌
		if (triShape[0][0] <= -1.0) Rotate(SHAPE2, true);
		else if (triShape[0][1] <= -1.0 || triShape[0 + 1][1] <= -1.0) Rotate(SHAPE4, true);

		// 내부 사각형 충돌
		for (int i = 0; i < 3; ++i) {
			// 오른쪽 벽면 충돌
			if (triShape[i][0] <= 0.35 && triShape[i][0] >= 0.3) {
				if ((triShape[0][1] >= -0.33 && triShape[0][1] <= 0.33) ||
					(triShape[1][1] >= -0.33 && triShape[1][1] <= 0.33) ||
					(triShape[2][1] >= -0.33 && triShape[2][1] <= 0.33)) Rotate(SHAPE2, false);
			}

			// 위쪽 벽면 충돌
			if (triShape[i][1] >= 0.3 && triShape[i][1] <= 0.35) {
				if ((triShape[0][0] >= -0.35 && triShape[0][0] <= 0.35) ||
					(triShape[1][0] >= -0.35 && triShape[1][0] <= 0.35) ||
					(triShape[2][0] >= -0.35 && triShape[2][1] <= 0.35)) Rotate(SHAPE4, false);
			}
		}

	}
	else if (dir == LEFTUP) {
		// 벽면 충돌
		if (triShape[0][0] <= -1.0) Rotate(SHAPE2, true);
		else if (triShape[2][1] >= 1.0) Rotate(SHAPE1, true);

		// 내부 사각형 충돌
		for (int i = 0; i < 3; ++i) {
			// 오른쪽 벽면 충돌
			if (triShape[i][0] <= 0.35 && triShape[i][0] >= 0.3) {
				if ((triShape[0][1] >= -0.33 && triShape[0][1] <= 0.33) ||
					(triShape[1][1] >= -0.33 && triShape[1][1] <= 0.33) ||
					(triShape[2][1] >= -0.33 && triShape[2][1] <= 0.33)) Rotate(SHAPE2, false);
			}
			// 아래쪽 벽면 충돌
			if (triShape[i][1] >= -0.35 && triShape[i][1] <= -0.3) {
				if ((triShape[0][0] >= -0.35 && triShape[0][0] <= 0.35) ||
					(triShape[1][0] >= -0.35 && triShape[1][0] <= 0.35) ||
					(triShape[2][0] >= -0.35 && triShape[2][0] <= 0.35)) Rotate(SHAPE1, false);
			}
		}
	}
	else if (dir == RIGHTDOWN) {
		// 벽면 충돌
		if (triShape[1][0] >= 1.0 || triShape[2][0] >= 1.0) Rotate(SHAPE3, true);
		else if (triShape[0][1] <= -1.0 || triShape[1][1] <= -1.0) Rotate(SHAPE4, true);

		// 내부 사각형 충돌
		for (int i = 0; i < 3; ++i) {
			// 왼쪽 벽면 충돌
			if (triShape[i][0] >= -0.35 && triShape[i][0] <= -0.3) {
				if ((triShape[0][1] >= -0.33 && triShape[0][1] <= 0.33) ||
					(triShape[1][1] >= -0.33 && triShape[1][1] <= 0.33) ||
					(triShape[2][1] >= -0.33 && triShape[2][1] <= 0.33))
					Rotate(SHAPE3, false);
			}
			// 위쪽 벽면 충돌
			if (triShape[i][1] >= 0.3 && triShape[i][1] <= 0.35) {
				if ((triShape[0][0] >= -0.35 && triShape[0][0] <= 0.35) ||
					(triShape[1][0] >= -0.35 && triShape[1][0] <= 0.35) ||
					(triShape[2][0] >= -0.35 && triShape[2][0] <= 0.35))
					Rotate(SHAPE4, false);
			}
		}
	}
	else if (dir == RIGHTUP) {
		// 벽면 충돌
		if (triShape[1][0] >= 1.0 || triShape[2][0] >= 1.0) Rotate(SHAPE3, true);
		else if (triShape[2][1] >= 1.0) Rotate(SHAPE1, true);

		// 내부 사각형 충돌
		for (int i = 0; i < 3; ++i) {
			// 왼쪽 벽면 충돌
			if (triShape[i][0] >= -0.35 && triShape[i][0] <= -0.3) {
				if ((triShape[0][1] >= -0.33 && triShape[0][1] <= 0.33) ||
					(triShape[1][1] >= -0.33 && triShape[1][1] <= 0.33) ||
					(triShape[2][1] >= -0.33 && triShape[2][1] <= 0.33)) Rotate(SHAPE3, false);
			}
			// 아래쪽 벽면 충돌
			if (triShape[i][1] >= -0.35 && triShape[i][1] <= -0.3) {
				if ((triShape[0][0] >= -0.35 && triShape[0][0] <= 0.35) ||
					(triShape[1][0] >= -0.35 && triShape[1][0] <= 0.35) ||
					(triShape[2][0] >= -0.35 && triShape[2][0] <= 0.35)) Rotate(SHAPE1, false);
			}

		}
	}
}

void Rotate(SHAPE s, GLboolean isEdgeCollision)
{
	GLfloat x, y;

	switch (s)
	{
	case SHAPE::SHAPE1:
		x = triShape[0][0];
		if (isEdgeCollision) y = 1.0;
		else y = -0.35;

		triShape[0][1] = y;
		triShape[1][0] = x + 0.1;
		triShape[1][1] = y - 0.3;
		triShape[2][0] = x + 0.2;
		triShape[2][1] = y;
		break;
	case SHAPE::SHAPE2:
		if (isEdgeCollision) x = -1.0;
		else x = 0.35;
		y = triShape[0][1];

		triShape[0][0] = x;
		triShape[1][0] = x + 0.3;
		triShape[1][1] = y + 0.1;
		triShape[2][0] = x;
		triShape[2][1] = y + 0.2;
		break;
	case SHAPE::SHAPE3:
		if (isEdgeCollision) x = 1.0;
		else x = -0.35;
		y = triShape[0][1];

		triShape[0][0] = x - 0.3;
		triShape[1][0] = x;
		triShape[1][1] = y - 0.1;
		triShape[2][0] = x;
		triShape[2][1] = y + 0.1;
		break;
	case SHAPE::SHAPE4:
		x = triShape[0][0];
		if (isEdgeCollision) y = -1.0;
		else y = 0.35;

		triShape[0][1] = y;
		triShape[1][0] = x + 0.2;
		triShape[1][1] = y;
		triShape[2][0] = x + 0.1;
		triShape[2][1] = y + 0.3;
		break;
	}

	// 방향 전환
	random_device rd;
	uniform_int_distribution<int> uid(0, 3);

	//dir = (DIR)(uid(rd));

	while (dir != (DIR)(uid(rd)))
		dir = (DIR)(uid(rd));
}
