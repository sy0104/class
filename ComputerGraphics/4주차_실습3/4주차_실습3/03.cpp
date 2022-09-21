#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
using namespace std;

#define WINDOWX 600
#define WINDOWY 600

typedef struct Rect
{
	GLfloat x;
	GLfloat y;
	GLfloat size = 0.15f;

	GLclampf r = 1.0f;
	GLclampf g = 1.0f;
	GLclampf b = 1.0f;
}RECTANGLE;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int, int, int, int);
void Motion(int, int);
void Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
bool isCollision(RECTANGLE, int, int);

vector<RECTANGLE> rectVec;

bool isLeftDown = false;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WINDOWX, WINDOWY); // �������� ũ�� ����
	glutCreateWindow("Example3"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized" << std::endl;

	// ù��° �簢�� ����
	RECTANGLE r = { 0.0f, 0.0f, 0.15f, 1.0f, 0.0f, 0.0f };
	rectVec.push_back(r);

	r = { 0.05f, 0.05f, 0.15f, 1.0f, 1.0f, 0.0f };
	rectVec.push_back(r);

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);	// ���콺 �Լ� ����
	glutMotionFunc(Motion);	// ��� �Լ� ����
	glutKeyboardFunc(Keyboard);	// Ű���� �Է� �ݹ��Լ� ����
	glutTimerFunc(10, TimerFunction, 1);	// Ÿ�̸� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ���� ������
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	// �簢�� �׸���
	for (int i = 0; i < rectVec.size(); ++i)
	{
		GLfloat x1 = rectVec[i].x - rectVec[i].size;
		GLfloat y1 = rectVec[i].y - rectVec[i].size;
		GLfloat x2 = rectVec[i].x + rectVec[i].size;
		GLfloat y2 = rectVec[i].y + rectVec[i].size;

		glColor3f(rectVec[i].r, rectVec[i].g, rectVec[i].b);
		glRectf(x1, y1, x2, y2);
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) isLeftDown = true;
	else if (button == GLUT_UP) isLeftDown = false;
}

void Motion(int x, int y)
{
	float mouseX = ((float)x) / (WINDOWX / 2) - 1.0;
	float mouseY = -(((float)y) / (WINDOWY / 2) - 1.0);


	if (isLeftDown)
	{
		rectVec[0].x = mouseX;
		rectVec[0].y = mouseY;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a': case 'A':
		if (rectVec.size() < 5)
		{

		}
		break;
	}
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

bool isCollision(RECTANGLE r, int x, int y)
{
	float mouseX = ((float)x) / (WINDOWX / 2) - 1.0;
	float mouseY = -(((float)y) / (WINDOWY / 2) - 1.0);

	GLfloat x1 = r.x - r.size;
	GLfloat y1 = r.y - r.size;
	GLfloat x2 = r.x + r.size;
	GLfloat y2 = r.y + r.size;

	if (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2)
	{
		int a = 0;
		return true;
	}

	return false;
}
