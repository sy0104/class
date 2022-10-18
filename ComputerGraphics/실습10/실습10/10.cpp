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

char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid DrawScene();
GLvoid Reshape(int w, int h);
void InitBuffer();
void InitShader();
void UpdateBuffer();

void TimerFunction(int);

void LineToTriangle();
void TriangleToRectangle();
void RectangleToPentagon();
void PentagonToPoint();

GLuint width, height;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

GLchar* vertexSource;	// 소스코드 저장 변수
GLchar* fragmentSource;	// 소스코드 저장 변수

GLuint VAO, VBO[2];
GLuint s_program;

GLfloat triShape[30][3] = { //--- 삼각형 위치 값
	// Line -> Triangle
	{ -0.8, 0.2, 0.0 }, { -0.5, 0.205, 0.0 }, { -0.2, 0.2, 0.0 },
	// Triangle -> Rectangle
	{ 0.2, 0.2, 0.0 }, { 0.5, 0.2, 0.0 }, { 0.5, 0.8, 0.0 },
	{ 0.5, 0.2, 0.0 }, { 0.8, 0.2, 0.0 }, { 0.5, 0.8, 0.0 },
	{ 0.5, 0.8, 0.0 }, { 0.5, 0.2, 0.0 }, { 0.5, 0.8, 0.0 },
	// Rectangle -> Pentagon
	{ -0.8, -0.2, 0.0 }, { -0.8, -0.8, 0.0 }, { -0.5, -0.2, 0.0 },
	{ -0.8, -0.8, 0.0 }, { -0.2, -0.8, 0.0 }, { -0.5, -0.2, 0.0 },
	{ -0.5, -0.2, 0.0 }, { -0.2, -0.8, 0.0 }, { -0.2, -0.2, 0.0 },
	// Pentagon -> Point
	{ 0.15, -0.4, 0.0 }, { 0.3, -0.8, 0.0 }, { 0.5, -0.1, 0.0 },
	{ 0.3, -0.8, 0.0 }, { 0.7, -0.8, 0.0 }, { 0.5, -0.1, 0.0 },
	{ 0.5, -0.1, 0.0 }, { 0.7, -0.8, 0.0 }, { 0.85, -0.4, 0.0 },
};

GLfloat colors[30][3] = { //--- 삼각형 꼭지점 색상
	// Line -> Triangle
	{ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 },	// black
	// Triangle -> Rectangle
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },	// green
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },	// green
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },	// green
	// Rectangle -> Pentagon
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },	// blue
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },	// blue
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },	// blue
	// Pentagon -> Point
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },	// red
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },	// red
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },	// red

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
	glutCreateWindow("Example10");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(DrawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutTimerFunc(60, TimerFunction, 1);	// 타이머 함수 설정
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

	// 4등분 선 그리기
	glRectf(-0.002, -1.0, 0.002, 1.0);
	glRectf(-1.0, -0.002, 1.0, 0.002);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 30);	// 렌더링 할 인덱스 개수

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
	// triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	// 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성(색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	// 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성(색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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

void TimerFunction(int value)
{
	LineToTriangle();
	TriangleToRectangle();
	RectangleToPentagon();
	PentagonToPoint();
	UpdateBuffer();

	glutPostRedisplay();
	glutTimerFunc(60, TimerFunction, 1);
}

void LineToTriangle()
{
	// 1번 점 y 위로 [1][1]
	if (triShape[1][1] < 0.8) triShape[1][1] += 0.05;
}

void TriangleToRectangle()
{
	// 왼쪽 삼각형 2번 점 x 왼쪽으로 & 1번 점 x 오른쪽으로
	if (triShape[5][0] > 0.2) triShape[5][0] -= 0.05;
	if (triShape[4][0] < 0.8) triShape[4][0] += 0.05;

	// 오른쪽 삼각형 0번 점 x 오른쪽으로, y 위로 & 2번 점 x 왼쪽으로
	if (triShape[8][0] < 0.8) triShape[8][0] += 0.05;

	if (triShape[9][0] > 0.2) triShape[9][0] -= 0.05;
	if (triShape[11][0] < 0.8) triShape[11][0] += 0.05;
}

void RectangleToPentagon()
{
	// 왼쪽 오른쪽
	if (triShape[12][0] > -0.8) triShape[12][0] -= 0.01;
	if (triShape[12][1] > -0.4) triShape[12][1] -= 0.01;
	if (triShape[20][0] < -0.2) triShape[20][0] += 0.01;
	if (triShape[20][1] > -0.4) triShape[20][1] -= 0.01;

	// 아래 두 점 모이도록
	if (triShape[13][0] < -0.7) triShape[13][0] += 0.01;
	if (triShape[15][0] < -0.7) triShape[15][0] += 0.01;
	if (triShape[16][0] > -0.3) triShape[16][0] -= 0.01;
	if (triShape[19][0] > -0.3) triShape[19][0] -= 0.01;

	// 위 올라감
	if (triShape[14][1] < -0.1) triShape[14][1] += 0.01;
	if (triShape[17][1] < -0.1) triShape[17][1] += 0.01;
	if (triShape[18][1] < -0.1) triShape[18][1] += 0.01;
}

void PentagonToPoint()
{
	//if (triShape[21][0] < 0.4) triShape[21][0] += 0.01;
	//if (triShape[22][0] < 0.45) triShape[22][0] += 0.01;
	//if (triShape[22][1] < -0.45) triShape[22][1] += 0.01;
	//if (triShape[23][1] > -0.35) triShape[23][1] -= 0.01;

	//if (triShape[24][0] < 0.45) triShape[24][0] += 0.01;
	//if (triShape[24][1] < -0.45) triShape[24][1] += 0.01;
	//if (triShape[25][0] > 0.55) triShape[25][0] -= 0.01;
	//if (triShape[25][1] < -0.45) triShape[25][1] += 0.01;
	//if (triShape[26][1] > -0.35) triShape[26][1] -= 0.01;

	//if (triShape[27][1] > -0.35) triShape[27][1] -= 0.01;
	//if (triShape[28][0] > 0.55) triShape[28][0] -= 0.01;
	//if (triShape[28][1] < -0.45) triShape[28][1] += 0.01;
	//if (triShape[29][0] > 0.6) triShape[29][0] -= 0.01;

	if (triShape[21][0] < 0.45) triShape[21][0] += 0.01;
	if (triShape[22][0] < 0.48) triShape[22][0] += 0.01;
	if (triShape[22][1] < -0.45) triShape[22][1] += 0.01;
	if (triShape[23][1] > -0.35) triShape[23][1] -= 0.01;

	if (triShape[24][0] < 0.48) triShape[24][0] += 0.01;
	if (triShape[24][1] < -0.45) triShape[24][1] += 0.01;
	if (triShape[25][0] > 0.52) triShape[25][0] -= 0.01;
	if (triShape[25][1] < -0.45) triShape[25][1] += 0.01;
	if (triShape[26][1] > -0.35) triShape[26][1] -= 0.01;

	if (triShape[27][1] > -0.35) triShape[27][1] -= 0.01;
	if (triShape[28][0] > 0.52) triShape[28][0] -= 0.01;
	if (triShape[28][1] < -0.45) triShape[28][1] += 0.01;
	if (triShape[29][0] > 0.55) triShape[29][0] -= 0.01;
}
