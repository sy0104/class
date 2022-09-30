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

GLint width, height;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

GLchar* vertexSource;	// 소스코드 저장 변수
GLchar* fragmentSource;	// 소스코드 저장 변수

GLuint VAO, VBO[2];
GLuint s_program;

const GLfloat triShape[12][3] = { //--- 삼각형 위치 값
	{ -0.75, 0.25, 0.0 }, { -0.25, 0.25, 0.0 }, { -0.5, 0.75, 0.0},
	{ 0.25, 0.25, 0.0 }, { 0.75, 0.25, 0.0 }, { 0.5, 0.75, 0.0 },
	{ -0.75, -0.75, 0.0 }, { -0.25, -0.75, 0.0 }, { -0.5, -0.25, 0.0 }, 
	{ 0.25, -0.75, 0.0 }, { 0.75, -0.75, 0.0 }, { 0.5, -0.25, 0.0 }
};

const GLfloat colors[12][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 }
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

	//triShape.push_back({ -0.5, -0.5, 0.0 });
	//triShape.push_back({ 0.5, -0.5, 0.0 });
	//triShape.push_back({ 0.0, 0.5, 0.0 });

	InitShader();
	InitBuffer();

	glutDisplayFunc(DrawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
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
	GLfloat rColor, gColor, bColor;

	// 변경된 배경색 설정
	//glClearColor(rColor, gColor, bColor, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 렌더링 파이프라인에서 셰이더 불러오기
	glUseProgram(s_program);

	// 사용할 VAO 불러오기
	glBindVertexArray(VAO);

	// 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 12);	// 렌더링 할 인덱스 개수(6)

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
