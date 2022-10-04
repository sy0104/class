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
void TimerFunction(int);

void InitDirSpeed();
void Move();
void CheckCollision();
void Rotate(SHAPE, GLboolean);

DIR dir = LEFTUP;
GLfloat speed[3] = { 0.0, };
GLboolean isBounce = true, isMove = true;

GLuint width, height;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

GLchar* vertexSource;	// �ҽ��ڵ� ���� ����
GLchar* fragmentSource;	// �ҽ��ڵ� ���� ����

GLuint VAO, VBO[2];
GLuint s_program;

GLfloat triShape[9][3] = { //--- �ﰢ�� ��ġ ��
	{ -0.6, 0.0, 0.0 }, { -0.4, 0.0, 0.0 }, { -0.5, 0.3, 0.0 },		// ƨ��� �ﰢ��
	{ -0.3, -0.34, 0.0 }, { -0.1, -0.34, 0.0 }, { -0.2, -0.09, 0.0 },		// �簢�� �� �� �ﰢ��
	{ 0.1, 0.34, 0.0 }, { 0.3, 0.34, 0.0 }, { 0.2, 0.09, 0.0 },	// �簢�� �� �� �ﰢ��
};

GLfloat colors[9][3] = { //--- �ﰢ�� ������ ����
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },	// red
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },	// green
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },	// blue
};

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 800;
	height = 700;

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

	// ����� VAO �ҷ�����
	glBindVertexArray(VAO);

	// �߾� �簢�� �׸���
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(-0.35, -0.35, 0.35, 0.35);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 9);	// ������ �� �ε��� ����(9)

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
	// triShape �迭�� ������: 9 * float !!!!
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	// 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ�(����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	// 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ�(����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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
	// ���� �ﰢ���� ƨ��⸦ ����/����
	if (key == 'o' || key == 'O') isBounce = !isBounce;

	// ���� �ﰢ�� �̵�/����
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
	speed[2] *= -1;	// �� �ﰢ���� ó���� ���� �̵�
}

void Move()
{
	int idx = 0;

	for (int i = 0; i < 3; ++i) {
		idx = i * 3;

		if (isBounce && i == 0) {	// ƨ��� �ﰢ��
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

		else if (isMove && i > 0) {	// �簢�� ���� �ﰢ�� - �¿��̵�
			triShape[idx][0] += speed[i];
			triShape[idx + 1][0] += speed[i];
			triShape[idx + 2][0] += speed[i];

			if (speed[i] >= 0.0 && triShape[idx + 1][0] >= 0.35 ||
				speed[i] <= 0.0 && triShape[idx][0] <= -0.35) {
				// �ݴ� �������� �̵�
				speed[i] *= -1.0;

				// ���� ����
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
		// ���� �浹
		if (triShape[0][0] <= -1.0) Rotate(SHAPE2, true);
		else if (triShape[0][1] <= -1.0 || triShape[0 + 1][1] <= -1.0) Rotate(SHAPE4, true);

		// ���� �簢�� �浹
		for (int i = 0; i < 3; ++i) {
			// ������ ���� �浹
			if (triShape[i][0] <= 0.35 && triShape[i][0] >= 0.3) {
				if ((triShape[0][1] >= -0.33 && triShape[0][1] <= 0.33) ||
					(triShape[1][1] >= -0.33 && triShape[1][1] <= 0.33) ||
					(triShape[2][1] >= -0.33 && triShape[2][1] <= 0.33)) Rotate(SHAPE2, false);
			}

			// ���� ���� �浹
			if (triShape[i][1] >= 0.3 && triShape[i][1] <= 0.35) {
				if ((triShape[0][0] >= -0.35 && triShape[0][0] <= 0.35) ||
					(triShape[1][0] >= -0.35 && triShape[1][0] <= 0.35) ||
					(triShape[2][0] >= -0.35 && triShape[2][1] <= 0.35)) Rotate(SHAPE4, false);
			}
		}

	}
	else if (dir == LEFTUP) {
		// ���� �浹
		if (triShape[0][0] <= -1.0) Rotate(SHAPE2, true);
		else if (triShape[2][1] >= 1.0) Rotate(SHAPE1, true);

		// ���� �簢�� �浹
		for (int i = 0; i < 3; ++i) {
			// ������ ���� �浹
			if (triShape[i][0] <= 0.35 && triShape[i][0] >= 0.3) {
				if ((triShape[0][1] >= -0.33 && triShape[0][1] <= 0.33) ||
					(triShape[1][1] >= -0.33 && triShape[1][1] <= 0.33) ||
					(triShape[2][1] >= -0.33 && triShape[2][1] <= 0.33)) Rotate(SHAPE2, false);
			}
			// �Ʒ��� ���� �浹
			if (triShape[i][1] >= -0.35 && triShape[i][1] <= -0.3) {
				if ((triShape[0][0] >= -0.35 && triShape[0][0] <= 0.35) ||
					(triShape[1][0] >= -0.35 && triShape[1][0] <= 0.35) ||
					(triShape[2][0] >= -0.35 && triShape[2][0] <= 0.35)) Rotate(SHAPE1, false);
			}
		}
	}
	else if (dir == RIGHTDOWN) {
		// ���� �浹
		if (triShape[1][0] >= 1.0 || triShape[2][0] >= 1.0) Rotate(SHAPE3, true);
		else if (triShape[0][1] <= -1.0 || triShape[1][1] <= -1.0) Rotate(SHAPE4, true);

		// ���� �簢�� �浹
		for (int i = 0; i < 3; ++i) {
			// ���� ���� �浹
			if (triShape[i][0] >= -0.35 && triShape[i][0] <= -0.3) {
				if ((triShape[0][1] >= -0.33 && triShape[0][1] <= 0.33) ||
					(triShape[1][1] >= -0.33 && triShape[1][1] <= 0.33) ||
					(triShape[2][1] >= -0.33 && triShape[2][1] <= 0.33))
					Rotate(SHAPE3, false);
			}
			// ���� ���� �浹
			if (triShape[i][1] >= 0.3 && triShape[i][1] <= 0.35) {
				if ((triShape[0][0] >= -0.35 && triShape[0][0] <= 0.35) ||
					(triShape[1][0] >= -0.35 && triShape[1][0] <= 0.35) ||
					(triShape[2][0] >= -0.35 && triShape[2][0] <= 0.35))
					Rotate(SHAPE4, false);
			}
		}
	}
	else if (dir == RIGHTUP) {
		// ���� �浹
		if (triShape[1][0] >= 1.0 || triShape[2][0] >= 1.0) Rotate(SHAPE3, true);
		else if (triShape[2][1] >= 1.0) Rotate(SHAPE1, true);

		// ���� �簢�� �浹
		for (int i = 0; i < 3; ++i) {
			// ���� ���� �浹
			if (triShape[i][0] >= -0.35 && triShape[i][0] <= -0.3) {
				if ((triShape[0][1] >= -0.33 && triShape[0][1] <= 0.33) ||
					(triShape[1][1] >= -0.33 && triShape[1][1] <= 0.33) ||
					(triShape[2][1] >= -0.33 && triShape[2][1] <= 0.33)) Rotate(SHAPE3, false);
			}
			// �Ʒ��� ���� �浹
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

	// ���� ��ȯ
	random_device rd;
	uniform_int_distribution<int> uid(0, 3);

	//dir = (DIR)(uid(rd));

	while (dir != (DIR)(uid(rd)))
		dir = (DIR)(uid(rd));
}
