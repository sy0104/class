#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
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
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

GLchar* vertexSource;	// �ҽ��ڵ� ���� ����
GLchar* fragmentSource;	// �ҽ��ڵ� ���� ����

GLuint VAO[3], VBO[6];
GLuint s_program;

enum SHAPE { HEXA, TETRAHED };

SHAPE shape = HEXA;
GLuint inputNum;
GLchar inputChar;
GLuint emptyNum = 99;
GLchar emptyChar = 'z';

GLfloat coordinate[] =		// xy ��ǥ��
{
	0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

GLfloat coordinateColor[] =		// xy ��ǥ�� �÷� - �Ͼ��
{
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};

GLfloat hexahedron[] =		// ����ü
{
	// ����
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,
   0.2f,0.2f, 0.2f,			0.0f, 1.0f, 0.0f,
   -0.2f, 0.2f, -0.2f,		0.5f, 0.5f, 0.5f,

   0.2f, 0.2f, 0.2f,		0.0f, 1.0f, 0.0f,
   0.2f, 0.2f, -0.2f,		0.0f, 0.0f, 1.0f,
   -0.2f, 0.2f, -0.2f,		0.5f, 0.5f, 0.5f,

   // �ظ�
   -0.2f, -0.2f, 0.2f,		1.0f, 1.0f, 1.0f,
   -0.2f, -0.2f, -0.2f,		0.3f, 1.0f, 0.7f,
   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,

   -0.2f, -0.2f, 0.2f,		1.0f, 1.0f, 1.0f,
   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,
   0.2f, -0.2f, 0.2f,		0.0f, 1.0f, 1.0f,

   // �޸�
   -0.2f, -0.2f, 0.2f,		0.0f, 0.0f, 1.0f,
   0.2f, -0.2f, 0.2f,		0.0f, 1.0f, 1.0f,
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,

   0.2f, -0.2f, 0.2f,		0.0f, 1.0f, 1.0f,
   0.2f, 0.2f, 0.2f,		0.0f, 1.0f, 0.0f,
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,

   // �ո�
   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,
   -0.2f, -0.2f, -0.2f,		0.3f, 0.0f, 0.7f,
   0.2f, 0.2f, -0.2f,		0.0f, 0.0f, 1.0f,

   -0.2f, -0.2f, -0.2f,		0.3f, 0.0f, 0.7f,
   -0.2f, 0.2f, -0.2f,		0.5f, 0.5f, 0.5f,
   0.2f, 0.2f, -0.2f,		0.0f, 0.0f, 1.0f,

   // ���ʸ�
   -0.2f, -0.2f, -0.2f,		0.3f, 0.0f, 0.3f,
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,
   -0.2f, 0.2f, -0.2f,		0.5f, 0.5f, 0.5f,

   -0.2f, -0.2f, -0.2f,		0.3f, 0.0f, 0.4f,
   -0.2f, -0.2f, 0.2f,		0.0f, 0.0f, 0.0f,
   -0.2f, 0.2f, 0.2f,		1.0f, 0.0f, 0.0f,

   // �����ʸ�
   0.2f, -0.2f, 0.2f,		0.0f, 1.0f, 1.0f,
   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,
   0.2f, 0.2f, 0.2f,		0.0f, 1.0f, 0.0f,

   0.2f, -0.2f, -0.2f,		0.5f, 0.3f, 0.7f,
   0.2f, 0.2f, -0.2f,		0.0f, 0.0f, 1.0f,
   0.2f, 0.2f, 0.2f,		0.0f, 1.0f, 0.0f
};

GLfloat hexahedronColor[] =		// ����ü �÷� - �� �鸶�� �ٸ���
{
	// ����
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,

	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f,		0.0f, 0.5f, 1.0f,

	// �ظ�
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,

	// �޸�
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	// �ո�
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,

	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,

	// ���ʸ�
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,

	// �����ʸ�
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

GLfloat tetrahedron[] =		// ���ü
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

GLfloat tetrahedronColor[] =	// ���ü �÷� - �� �鸶�� �ٸ���
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

	glutKeyboardFunc(KeyBoard);
	glutDisplayFunc(DrawScene); //--- ��� �ݹ� �Լ�
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
	// ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InitBuffer();

	// ������ ���������ο��� ���̴� �ҷ�����
	glUseProgram(s_program);

	// xy ��ǥ��
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::rotate(mat, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int trans = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(mat));
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINES, 0, 4);

	// ����ü
	if (shape == SHAPE:: HEXA)
	{
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, glm::vec3(0.f, 0.f, 0.0f));
		mat = glm::rotate(mat, glm::radians(-20.f), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, glm::radians(20.f), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int trans = glGetUniformLocation(s_program, "transform");
		glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(mat));
		glBindVertexArray(VAO[1]);

		// Ű�Է� 1 ~ 6 -> ����ü�� �� ���� �׸���.
		switch (inputNum)
		{
		case 1: glDrawArrays(GL_TRIANGLES, 0, 6); break;
		case 2: glDrawArrays(GL_TRIANGLES, 6, 6); break;
		case 3: glDrawArrays(GL_TRIANGLES, 12, 6); break;
		case 4: glDrawArrays(GL_TRIANGLES, 18, 6); break;
		case 5: glDrawArrays(GL_TRIANGLES, 24, 6); break;
		case 6: glDrawArrays(GL_TRIANGLES, 30, 6); break;
		}

		// Ű�Է� a/b/c -> ����ü�� ���� �ݴ������ �� ���� �����Ͽ� �׸��� (�ո�+����, ������+������, ����+�Ʒ���)
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

	// ���ü
	else if (shape == SHAPE::TETRAHED)
	{
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, glm::vec3(0.f, 0.f, 0.0f));
		mat = glm::rotate(mat, glm::radians(20.f), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, glm::radians(-20.f), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int trans = glGetUniformLocation(s_program, "transform");
		glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(mat));
		glBindVertexArray(VAO[2]);

		// Ű�Է� 7 ~ 0 -> ���ü�� �� ���� �׸���.
		switch (inputNum)
		{
		case 7: glDrawArrays(GL_TRIANGLES, 0, 3); break;
		case 8: glDrawArrays(GL_TRIANGLES, 3, 3); break;
		case 9: glDrawArrays(GL_TRIANGLES, 6, 3); break;
		case 0: glDrawArrays(GL_TRIANGLES, 9, 3); break;
		}

		// Ű�Է� e/f/g -> ���ü���� �Ʒ����� �⺻���� �׸���, �� �ܿ� �ﰢ���� ������ �׸���.
		// �Ʒ���+�ո�1, �Ʒ���+�ո�2, �Ʒ���+�ո�3
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
	glGenVertexArrays(3, VAO);	// VAO�� �����ϰ� �Ҵ��ϱ�

	//// 0: ��ǥ�� ��ǥ�� 1: ��ǥ�� �÷�
	glBindVertexArray(VAO[0]);	// VA)�� ���ε��ϱ�
	glGenBuffers(2, &VBO[0]);	// 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	// 0: ��ǥ�� ��ǥ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinate), coordinate, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	// 1: ��ǥ�� �÷�
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinateColor), coordinateColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	//// 2: ����ü ��ǥ�� 3: ����ü �÷�
	glBindVertexArray(VAO[1]);
	glGenBuffers(2, &VBO[2]);

	// 2: ����ü ��ǥ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexahedron), hexahedron, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	// 3: ����ü �÷�
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexahedronColor), hexahedronColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//// 4: ���ü ��ǥ�� 5: ���ü �÷�
	glBindVertexArray(VAO[2]);
	glGenBuffers(2, &VBO[4]);

	// 4: ���ü ��ǥ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron), tetrahedron, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	// 5: ���ü �÷�
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

	// ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// shader program ����ϱ�
	glUseProgram(s_program);
}

GLvoid KeyBoard(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	// HEXA Ű�Է�
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

	// TETRAHED Ű�Է�
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