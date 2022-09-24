#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
using namespace std;

#define WINDOWX 800
#define WINDOWY 800

typedef struct Rect
{
	GLfloat x;
	GLfloat y;
	GLfloat size = 0.03f;

	GLclampf r;
	GLclampf g;
	GLclampf b;
}RECTANGLE;


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);
void Motion(int, int);
void TimerFunction(int);
bool isCollision(RECTANGLE);

vector<RECTANGLE> rectVec;
RECTANGLE eraser;
GLboolean isLeftDown = false;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WINDOWX, WINDOWY); // �������� ũ�� ����
	glutCreateWindow("Example5"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized" << std::endl;

	// ��� �簢�� 100�� ����
	for (int i = 0; i < 100; ++i)
	{
		random_device rd;
		uniform_real_distribution<float> urd(-1.0f + 0.03f, 1.0f - 0.03f);
		uniform_int_distribution<int> uid(0, 255);

		RECTANGLE rect;
		rect.x = urd(rd);
		rect.y = urd(rd);

		rect.r = (GLclampf)(uid(rd) / 255.0f);
		rect.g = (GLclampf)(uid(rd) / 255.0f);
		rect.b = (GLclampf)(uid(rd) / 255.0f);

		rectVec.push_back(rect);
	}

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);	// Ű���� �Է� �ݹ��Լ� ����
	glutMouseFunc(Mouse);	// ���콺 �Լ� ����
	glutMotionFunc(Motion);	// ��� �Լ� ����
	glutTimerFunc(10, TimerFunction, 1);	// Ÿ�̸� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // ������ ������
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	for (int i = 0; i < rectVec.size(); ++i)
	{
		glColor3f(rectVec[i].r, rectVec[i].g, rectVec[i].b);
		glRectf(rectVec[i].x - rectVec[i].size, rectVec[i].y - rectVec[i].size, 
			rectVec[i].x + rectVec[i].size, rectVec[i].y + rectVec[i].size);
	}

	if (isLeftDown)
	{
		glColor3f(eraser.r, eraser.g, eraser.b);
		glRectf(eraser.x - eraser.size, eraser.y - eraser.size,
			eraser.x + eraser.size, eraser.y + eraser.size);

	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	return GLvoid();
}

void Mouse(int button, int state, int x, int y)
{
	float mouseX = ((float)x) / (WINDOWX / 2) - 1.0;
	float mouseY = -(((float)y) / (WINDOWY / 2) - 1.0);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		isLeftDown = true;

		eraser.x = mouseX;
		eraser.y = mouseY;
		eraser.size = 0.06f;
		eraser.r = eraser.g = eraser.b = 0.0f;
	}
	else isLeftDown = false;
}

void Motion(int x, int y)
{
	float mouseX = ((float)x) / (WINDOWX / 2) - 1.0;
	float mouseY = -(((float)y) / (WINDOWY / 2) - 1.0);

	if (isLeftDown)
	{
		eraser.x = mouseX;
		eraser.y = mouseY;

		for (int i = 0; i < rectVec.size(); ++i)
		{
			if (isCollision(rectVec[i]))
			{
				rectVec.erase(rectVec.begin() + i);
				int a = 0;
			}
		}
	}
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

bool isCollision(RECTANGLE r)
{
	GLfloat left = r.x - r.size;
	GLfloat top = r.y + r.size;
	GLfloat right = r.x + r.size;
	GLfloat bottom = r.y - r.size;

	// ���찳�� �簢�� �浹üũ
	if (left <= eraser.x + eraser.size && right >= eraser.x - eraser.size &&
		top >= eraser.y - eraser.size && bottom <= eraser.y + eraser.size)
	{
		return true;
	}


	return false;
}
