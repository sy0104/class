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
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

GLchar* vertexSource;	// �ҽ��ڵ� ���� ����
GLchar* fragmentSource;	// �ҽ��ڵ� ���� ����

GLuint VAO, VBO[2];
GLuint s_program;

GLfloat triShape[30][3] = { //--- �ﰢ�� ��ġ ��
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

GLfloat colors[30][3] = { //--- �ﰢ�� ������ ����
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 800;
	height = 600;

	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example10");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(DrawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutTimerFunc(60, TimerFunction, 1);	// Ÿ�̸� �Լ� ����
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

	// ����� VAO �ҷ�����
	glBindVertexArray(VAO);

	// 4��� �� �׸���
	glRectf(-0.002, -1.0, 0.002, 1.0);
	glRectf(-1.0, -0.002, 1.0, 0.002);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 30);	// ������ �� �ε��� ����

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
	glGenVertexArrays(1, &VAO);	// VAO�� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(VAO);		// VAO�� ���ε��ϱ�
	glGenBuffers(2, VBO);		// 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	// 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ�(��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// ���� diamond���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	// 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ�(����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	// 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ�(����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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
	// 1�� �� y ���� [1][1]
	if (triShape[1][1] < 0.8) triShape[1][1] += 0.05;
}

void TriangleToRectangle()
{
	// ���� �ﰢ�� 2�� �� x �������� & 1�� �� x ����������
	if (triShape[5][0] > 0.2) triShape[5][0] -= 0.05;
	if (triShape[4][0] < 0.8) triShape[4][0] += 0.05;

	// ������ �ﰢ�� 0�� �� x ����������, y ���� & 2�� �� x ��������
	if (triShape[8][0] < 0.8) triShape[8][0] += 0.05;

	if (triShape[9][0] > 0.2) triShape[9][0] -= 0.05;
	if (triShape[11][0] < 0.8) triShape[11][0] += 0.05;
}

void RectangleToPentagon()
{
	// ���� ������
	if (triShape[12][0] > -0.8) triShape[12][0] -= 0.01;
	if (triShape[12][1] > -0.4) triShape[12][1] -= 0.01;
	if (triShape[20][0] < -0.2) triShape[20][0] += 0.01;
	if (triShape[20][1] > -0.4) triShape[20][1] -= 0.01;

	// �Ʒ� �� �� ���̵���
	if (triShape[13][0] < -0.7) triShape[13][0] += 0.01;
	if (triShape[15][0] < -0.7) triShape[15][0] += 0.01;
	if (triShape[16][0] > -0.3) triShape[16][0] -= 0.01;
	if (triShape[19][0] > -0.3) triShape[19][0] -= 0.01;

	// �� �ö�
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
