#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/ext.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
using namespace std;

char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid DrawScene();
GLvoid Reshape(int w, int h);
void InitBuffer();
void InitShader();

GLvoid KeyBoard(unsigned char, int, int);

GLuint width, height;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

GLchar* vertexSource;	// 소스코드 저장 변수
GLchar* fragmentSource;	// 소스코드 저장 변수

GLuint VAO[3], VBO[6];
GLuint s_program;

enum SHAPE { HEXA, TETRAHED };

SHAPE shape = HEXA;
GLuint inputNum;
GLchar inputChar;
GLuint emptyNum = 99;
GLchar emptyChar = 'z';

GLfloat coordinate[] =		// xy 좌표계
{
	0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

GLfloat coordinateColor[] =		// xy 좌표계 컬러 - 하얀색
{
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};

GLfloat hexahedron[] =		// 육면체
{
	// 윗면
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,
   0.2f,0.2f, 0.2f,			0.0f, 1.0f, 0.0f,
   -0.2f, 0.2f, -0.2f,		0.5f, 0.5f, 0.5f,

   0.2f, 0.2f, 0.2f,		0.0f, 1.0f, 0.0f,
   0.2f, 0.2f, -0.2f,		0.0f, 0.0f, 1.0f,
   -0.2f, 0.2f, -0.2f,		0.5f, 0.5f, 0.5f,

   // 밑면
   -0.2f, -0.2f, 0.2f,		1.0f, 1.0f, 1.0f,
   -0.2f, -0.2f, -0.2f,		0.3f, 1.0f, 0.7f,
   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,

   -0.2f, -0.2f, 0.2f,		1.0f, 1.0f, 1.0f,
   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,
   0.2f, -0.2f, 0.2f,		0.0f, 1.0f, 1.0f,

   // 뒷면
   -0.2f, -0.2f, 0.2f,		0.0f, 0.0f, 1.0f,
   0.2f, -0.2f, 0.2f,		0.0f, 1.0f, 1.0f,
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,

   0.2f, -0.2f, 0.2f,		0.0f, 1.0f, 1.0f,
   0.2f, 0.2f, 0.2f,		0.0f, 1.0f, 0.0f,
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,

   // 앞면
   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,
   -0.2f, -0.2f, -0.2f,		0.3f, 0.0f, 0.7f,
   0.2f, 0.2f, -0.2f,		0.0f, 0.0f, 1.0f,

   -0.2f, -0.2f, -0.2f,		0.3f, 0.0f, 0.7f,
   -0.2f, 0.2f, -0.2f,		0.5f, 0.5f, 0.5f,
   0.2f, 0.2f, -0.2f,		0.0f, 0.0f, 1.0f,

   // 왼쪽면
   -0.2f, -0.2f, -0.2f,		0.3f, 0.0f, 0.3f,
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,
   -0.2f, 0.2f, -0.2f,		0.5f, 0.5f, 0.5f,

   -0.2f, -0.2f, -0.2f,		0.3f, 0.0f, 0.4f,
   -0.2f, -0.2f, 0.2f,		0.0f, 0.0f, 0.0f,
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,

   // 오른쪽면
   0.2f, -0.2f, 0.2f,		0.0f, 1.0f, 1.0f,
   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,
   0.2f, 0.2f, 0.2f,		0.0f, 1.0f, 0.0f,

   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,
   0.2f, 0.2f, -0.2f,		0.0f, 0.0f, 1.0f,
   0.2f, 0.2f, 0.2f,		0.0f, 1.0f, 0.0f
};

GLfloat hexahedronColor[] =		// 육면체 컬러 - 각 면마다 다르게
{
	// 윗면
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,

	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,

	// 밑면
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,

	// 뒷면
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	// 앞면
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,

	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,

	// 왼쪽면
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,

	// 오른쪽면
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

GLfloat tetrahedron[] =		// 사면체
{
	-0.3f, -0.3f, -0.3f, 0.5f, 0.5f, 0.5f,
	-0.0f, 0.3f, 0.0f, 1.0f, 0.3f, 0.0f,
	0.2f, -0.2f, -0.2f, 1.0f, 0.0f, 0.3f,

	0.2f, -0.2f, -0.2f, 1.0f, 0.0f, 0.3f,
	0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.3f, -0.3f, 0.3f, 0.0f, 0.0f, 0.1f,

	0.3f, -0.3f, 0.3f, 0.0f, 0.5f, 0.3f,
	0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.3f, -0.3f, -0.3f, 0.3f, 0.5f, 0.5f,

	-0.3f, -0.3f, -0.3f, 0.0f, 1.0f, 0.1f,
	0.2f, -0.2f, -0.2f, 1.0f, 0.0f, 0.3f,
	0.3f, -0.3f, 0.3f, 0.0f, 0.3f, 1.0f
};

GLfloat tetrahedronColor[] =	// 사면체 컬러 - 각 면마다 다르게
{
	// red
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	// green
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	// blue
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	// orange
	1.0f, 0.5f, 0.0f, 1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f, 1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f, 1.0f, 0.5f, 0.0f
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

	glutKeyboardFunc(KeyBoard);
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
	// 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InitBuffer();

	// 렌더링 파이프라인에서 셰이더 불러오기
	glUseProgram(s_program);

	// xy 좌표계
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::rotate(mat, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int trans = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(mat));
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINES, 0, 4);

	// 육면체
	if (shape == SHAPE:: HEXA)
	{
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, glm::vec3(0.f, 0.f, 0.0f));
		mat = glm::rotate(mat, glm::radians(-20.f), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, glm::radians(20.f), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int trans = glGetUniformLocation(s_program, "transform");
		glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(mat));
		glBindVertexArray(VAO[1]);

		// 키입력 1 ~ 6 -> 육면체의 각 면을 그린다.
		switch (inputNum)
		{
		case 1: glDrawArrays(GL_TRIANGLES, 0, 6); break;
		case 2: glDrawArrays(GL_TRIANGLES, 6, 6); break;
		case 3: glDrawArrays(GL_TRIANGLES, 12, 6); break;
		case 4: glDrawArrays(GL_TRIANGLES, 18, 6); break;
		case 5: glDrawArrays(GL_TRIANGLES, 24, 6); break;
		case 6: glDrawArrays(GL_TRIANGLES, 30, 6); break;
		}

		// 키입력 a/b/c -> 육면체의 서로 반대방향의 두 면을 선택하여 그린다 (앞면+윗면, 좌측면+우측면, 윗면+아랫면)
		switch (inputChar)
		{
		case 'a': case 'A':
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawArrays(GL_TRIANGLES, 6, 6);
			break;
		case 'b': case 'B':
			glDrawArrays(GL_TRIANGLES, 12, 6);
			glDrawArrays(GL_TRIANGLES, 18, 6);
			break;
		case 'c': case 'C':
			glDrawArrays(GL_TRIANGLES, 24, 6);
			glDrawArrays(GL_TRIANGLES, 30, 6);
			break;
		}
	}

	// 사면체
	else if (shape == SHAPE::TETRAHED)
	{
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, glm::vec3(0.f, 0.f, 0.0f));
		mat = glm::rotate(mat, glm::radians(20.f), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, glm::radians(-20.f), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int trans = glGetUniformLocation(s_program, "transform");
		glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(mat));
		glBindVertexArray(VAO[2]);

		// 키입력 7 ~ 0 -> 사면체의 각 면을 그린다.
		switch (inputNum)
		{
		case 7: glDrawArrays(GL_TRIANGLES, 0, 3); break;
		case 8: glDrawArrays(GL_TRIANGLES, 3, 3); break;
		case 9: glDrawArrays(GL_TRIANGLES, 6, 3); break;
		case 0: glDrawArrays(GL_TRIANGLES, 9, 3); break;
		}

		// 키입력 e/f/g -> 사면체에서 아랫면은 기본으로 그리고, 그 외에 삼각형을 번갈아 그린다.
		// 아랫면+앞면1, 아랫면+앞면2, 아랫면+앞면3
		switch (inputChar)
		{
		case 'e': case 'E':
			glDrawArrays(GL_TRIANGLES, 9, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			break;
		case 'f': case 'F':
			glDrawArrays(GL_TRIANGLES, 9, 3);
			glDrawArrays(GL_TRIANGLES, 3, 3);
			break;
		case 'g': case 'G':
			glDrawArrays(GL_TRIANGLES, 9, 3);
			glDrawArrays(GL_TRIANGLES, 6, 3);
			break;
		}
	}

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
	glGenVertexArrays(3, VAO);	// VAO를 저장하고 할당하기

	//// 0: 좌표계 좌표값 1: 좌표계 컬러
	glBindVertexArray(VAO[0]);	// VA)를 바인드하기
	glGenBuffers(2, &VBO[0]);	// 2개의 VBO를 지정하고 할당하기

	// 0: 좌표계 좌표값
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinate), coordinate, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	// 1: 좌표계 컬러
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinateColor), coordinateColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	//// 2: 육면체 좌표값 3: 육면체 컬러
	glBindVertexArray(VAO[1]);
	glGenBuffers(2, &VBO[2]);

	// 2: 육면체 좌표값
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexahedron), hexahedron, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	// 3: 육면체 컬러
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexahedronColor), hexahedronColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//// 4: 사면체 좌표값 5: 사면체 컬러
	glBindVertexArray(VAO[2]);
	glGenBuffers(2, &VBO[4]);

	// 4: 사면체 좌표값
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron), tetrahedron, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	// 5: 사면체 컬러
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronColor), tetrahedronColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
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

GLvoid KeyBoard(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	// HEXA 키입력
	case '1':
		shape = HEXA;
		inputNum = 1;
		inputChar = emptyChar;
		break;
	case '2':
		shape = HEXA;
		inputNum = 2;
		inputChar = emptyChar;
		break;
	case '3':
		shape = HEXA;
		inputNum = 3;
		inputChar = emptyChar;
		break;
	case '4':
		shape = HEXA;
		inputNum = 4;
		inputChar = emptyChar;
		break;
	case '5':
		shape = HEXA;
		inputNum = 5;
		inputChar = emptyChar;
		break;
	case '6':
		shape = HEXA;
		inputNum = 6;
		inputChar = emptyChar;
		break;
	case 'a':
		shape = HEXA;
		inputChar = 'a';
		inputNum = emptyNum;
		break;
	case 'b':
		shape = HEXA;
		inputChar = 'b';
		inputNum = emptyNum;
		break;
	case 'c':
		shape = HEXA;
		inputChar = 'c';
		inputNum = emptyNum;
		break;

	// TETRAHED 키입력
	case '7':
		shape = TETRAHED;
		inputNum = 7;
		inputChar = emptyChar;
		break;
	case '8':
		shape = TETRAHED;
		inputNum = 8;
		inputChar = emptyChar;
		break;
	case '9':
		shape = TETRAHED;
		inputNum = 9;
		inputChar = emptyChar;
		break;
	case '0':
		shape = TETRAHED;
		inputNum = 0;
		inputChar = emptyChar;
		break;
	case 'e':
		shape = TETRAHED;
		inputChar = 'e';
		inputNum = emptyNum;
		break;
	case 'f':
		shape = TETRAHED;
		inputChar = 'f';
		inputNum = emptyNum;
		break;
	case 'g':
		shape = TETRAHED;
		inputChar = 'g';
		inputNum = emptyNum;
		break;
	}

	DrawScene();
}