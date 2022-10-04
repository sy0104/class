#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
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
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

GLchar* vertexSource;	// �ҽ��ڵ� ���� ����
GLchar* fragmentSource;	// �ҽ��ڵ� ���� ����

GLuint VBO[2];
GLuint s_program;

GLfloat triShape[12][3] = { //--- �ﰢ�� ��ġ ��
	{ -0.6, 0.3, 0.0 }, { -0.4, 0.3, 0.0 }, { -0.5, 0.7, 0.0 },
	{ 0.4, 0.3, 0.0 }, { 0.6, 0.3, 0.0 }, { 0.5, 0.7, 0.0 },
	{ -0.6, -0.7, 0.0 }, { -0.4, -0.7, 0.0 }, { -0.5, -0.3, 0.0 },
	{ 0.4, -0.7, 0.0 }, { 0.6, -0.7, 0.0 }, { 0.5, -0.3, 0.0 }
};

GLfloat colors[12][3] = { //--- �ﰢ�� ������ ����
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },	// red
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },	// green
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },	// blue
	{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }		// yellow
};

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 800;
	height = 600;

	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();
	InitDirSpeed();

	glutDisplayFunc(DrawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);	// Ű���� �Է� �ݹ��Լ� ����
	glutMouseFunc(Mouse);	// ���콺 �Լ� ����
	glutTimerFunc(10, TimerFunction, 1);	// Ÿ�̸� �Լ� ����
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

//--- ���ؽ� ���̴� ��ü �����
void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");	// vs �о����

	// ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);

	// ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);

	// �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����" << endl << errorLog << endl;
		//return false;
	}
}

//--- �����׸�Ʈ ���̴� ��ü �����
void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����

	// �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);

	// �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);

	// �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		//return false;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� �����

	glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�

	glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�

	glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID); //--- ������� ���̴� ���α׷� ����ϱ�
	//--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
	//--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
	//--- ����ϱ� ������ ȣ���� �� �ִ�.

	return ShaderProgramID;
}

//--- ��� �ݹ� �Լ�
GLvoid DrawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	// ����� ���� ����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������ ���������ο��� ���̴� �ҷ�����
	glUseProgram(s_program);

	// �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 12);	// ������ �� �ε��� ����(12)

	// ȭ�鿡 ����ϱ�
	glutSwapBuffers();
}

//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenBuffers(2, VBO);		// 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	// 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ�(��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// ���� diamond���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// triShape �迭�� ������: 9 * float !!!!
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	// 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ�(����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3 * float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

void UpdateBuffer()
{
	// 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ�(��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// ���� diamond���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// triShape �迭�� ������: 9 * float !!!!
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	// 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ�(����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3 * float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 1���� ��� �����ϰ� ��.
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

	// ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// shader program ����ϱ�
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

	// ������
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


	// �ݴ� ���� ��ȯ
	random_device rd;
	uniform_int_distribution<int> leftUid(0, 1);
	uniform_int_distribution<int> rightUid(2, 3);
	uniform_int_distribution<int> uid(0, 3);

	//if (d == LEFTDOWN || d == LEFTUP) dir[originIdx / 3] = (DIR)(rightUid(rd));
	//else dir[originIdx / 3] = (DIR)(leftUid(rd));

	while (dir[originIdx / 3] != (DIR)(uid(rd)))
		dir[originIdx / 3] = (DIR)(uid(rd));
}