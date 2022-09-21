#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);

typedef struct RGB
{
	GLclampf r;
	GLclampf g;
	GLclampf b;
};

RGB rgb{ 255.0f, 255.0f, 255.0f };
bool isTimer = false;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����(������ �̸�)

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // �ʱ� ���� ���

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else cout << "GLEW Initialized" << endl;

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);	// Ű���� �Է� �ݹ��Լ� ����
	glutTimerFunc(100, TimerFunction, 1);	// Ÿ�̸� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(rgb.r, rgb.g, rgb.b, 1.0f); // �ʱ� ���� ���
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'R': case 'r':
		rgb = { 1.0f, 0.0f, 0.0f };
		break;
	case 'G': case 'g':
		rgb = { 0.0f, 1.0f, 0.0f };
		break;
	case 'B': case 'b':
		rgb = { 0.0f, 0.0f, 1.0f };
		break;
	case 'A': case 'a':	// ������
	{
		random_device rd;
		uniform_int_distribution<int> uid(0, 255);
		rgb.r = (GLclampf)(uid(rd) / 255.0f);
		rgb.g = (GLclampf)(uid(rd) / 255.0f);
		rgb.b = (GLclampf)(uid(rd) / 255.0f);
	}
		break;
	case 'W': case 'w':	// ���
		rgb = { 1.0f, 1.0f, 1.0f };
		break;
	case 'K': case 'k':	// ������
		rgb = { 0.0f, 0.0f, 0.0f };
		break;
	case 'T': case 't':	// Ÿ�̸� ����, Ư�� �ð����� �� �������� �ٲ�
		isTimer = true;
		break;
	case 'S': case 's':	// Ÿ�̸� ����
		isTimer = false;
		break;
	case 'Q': case 'q':	// ���α׷� ����
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

void TimerFunction(int value)
{
	if (isTimer)
	{
		random_device rd;
		uniform_int_distribution<int> uid(0, 255);
		rgb.r = (GLclampf)(uid(rd) / 255.0f);
		rgb.g = (GLclampf)(uid(rd) / 255.0f);
		rgb.b = (GLclampf)(uid(rd) / 255.0f);
	}

	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}
