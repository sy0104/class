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

GLboolean isLeftDown = false;
GLboolean isMove = false;
auto moveRect = rectVec.rbegin();

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

	if (isMove)
	{
		GLfloat x1 = (*moveRect).x - (*moveRect).size;
		GLfloat y1 = (*moveRect).y - (*moveRect).size;
		GLfloat x2 = (*moveRect).x + (*moveRect).size;
		GLfloat y2 = (*moveRect).y + (*moveRect).size;

		glColor3f((*moveRect).r, (*moveRect).g, (*moveRect).b);
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) isLeftDown = true;
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		isLeftDown = false;
		isMove = false;
	}
}

void Motion(int x, int y)
{
	float mouseX = ((float)x) / (WINDOWX / 2) - 1.0;
	float mouseY = -(((float)y) / (WINDOWY / 2) - 1.0);

	if (isLeftDown)
	{
		// ���콺�� �簢���� ���� Ŭ������ �ʾ����� �浹üũ�ؼ� �ش� �簢�� ����
		if (!isMove)
		{
			// �簢���� ���������� ���߿� ���� �簢���� ���õ�
			for (auto iter = rectVec.rbegin(); iter != rectVec.rend(); ++iter)
			{
				if (isCollision(*iter, x, y))
				{
					isMove = true;
					moveRect = iter;
					break;
				}
			}
		}

		// ���콺 ��ǥ�� ���� �簢�� ������
		else
		{
			(*moveRect).x = mouseX;
			(*moveRect).y = mouseY;
		}
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a': case 'A':
		if (rectVec.size() < 5)
		{
			random_device rd;
			uniform_int_distribution<int> uid(0, 255);
			GLclampf r = uid(rd) / 255.0f;
			GLclampf g = uid(rd) / 255.0f;
			GLclampf b = uid(rd) / 255.0f;
			RECTANGLE rect = { 0.0f, 0.0f, 0.15f, r, g, b };
			rectVec.push_back(rect);
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
		return true;

	return false;
}
