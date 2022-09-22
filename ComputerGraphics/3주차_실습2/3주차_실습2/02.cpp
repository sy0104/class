#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
using namespace std;

#define WINDOWX 600
#define WINDOWY 600

typedef struct rgb
{
	GLclampf r;
	GLclampf g;
	GLclampf b;
}RGB;

typedef struct Rect
{
	GLfloat x1;	// ���� �ϴ� x
	GLfloat y1;	// ���� �ϴ� y
	GLfloat x2;	// ���� ��� x
	GLfloat y2;	// ���� ��� y
}RECTANGLE;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int, int, int, int);
bool isCollision(RECTANGLE, int, int);

RGB backRGB = { 0.0f, 0.0f, 1.0f };
RGB rectRGB = { 1.0f, 1.0f, 1.0f };;
RECTANGLE rect = { -0.4f, -0.4f, 0.4f, 0.4f };

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WINDOWX, WINDOWY); // �������� ũ�� ����
	glutCreateWindow("Example2"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized" << std::endl;

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);	// ���콺 �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(backRGB.r, backRGB.g, backRGB.b, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glColor3f(rectRGB.r, rectRGB.g, rectRGB.b);
	glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	random_device rd;
	uniform_int_distribution<int> uid(0, 255);
	GLfloat sizeX = 0.1f, sizeY = 0.1f;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// �簢�� ���� �����ϰ� ����
		if (isCollision(rect, x, y))
		{
			rectRGB.r = (GLclampf)(uid(rd) / 255.0f);
			rectRGB.g = (GLclampf)(uid(rd) / 255.0f);
			rectRGB.b = (GLclampf)(uid(rd) / 255.0f);
		}
		// ���� �����ϰ� ����
		else
		{
			backRGB.r = (GLclampf)(uid(rd) / 255.0f);
			backRGB.g = (GLclampf)(uid(rd) / 255.0f);
			backRGB.b = (GLclampf)(uid(rd) / 255.0f);
		}
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// �簢�� ũ�� Ȯ��
		if (isCollision(rect, x, y))
		{
			rect.x1 -= sizeX;
			rect.y1 -= sizeY;
			rect.x2 += sizeX;
			rect.y2 += sizeY;
		}
		// �簢�� ũ�� ���
		else
		{
			if (rect.x2 > 0.2f)
			{
				rect.x1 += sizeX;
				rect.y1 += sizeY;
				rect.x2 -= sizeX;
				rect.y2 -= sizeY;
			}
		}
	}

	glutPostRedisplay();
}

bool isCollision(RECTANGLE r, int x, int y)
{
	// ���콺 ��ǥ�� �簢�� �ȿ� �ִ��� Ȯ���ϴ� �Լ�
	float mouseX = ((float)x) / (WINDOWX / 2) - 1.0;
	float mouseY = -(((float)y) / (WINDOWY / 2) - 1.0);

	if (mouseX >= r.x1 && mouseX <= r.x2 && mouseY >= r.y1 && mouseY <= r.y2)
	{
		int a = 0;
		return true;
	}

	return false;
}
