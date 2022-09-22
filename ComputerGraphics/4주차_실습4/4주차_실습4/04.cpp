#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
using namespace std;

#define WINDOWX 700
#define WINDOWY 700

class Rect
{
public:
	enum STATE { IDLE, DIAGONAL, ZIGZAG, SIZE, END };
	enum DIR { LEFT, RIGHT };
public:
	Rect() {};
	~Rect() {};

public:
	void Initialize(GLfloat _x, GLfloat _y);	// ��ǥ, ũ��, �� ����
	void Draw();

public:
	void MoveDiagonal();
	void MoveZigzag();
	void ChangeSize();
	void MoveToOriginalPos();

public:
	void SetPos(GLfloat _x, GLfloat _y) { x = _x; y = _y; }
	void SetState(STATE s) { state = s; }

public:
	const STATE GetState() { return state; }

private:
	GLfloat	originalX;
	GLfloat	originalY;
	GLfloat	x;
	GLfloat	y;
	GLfloat	size = 0.15f;
	GLfloat maxSize = 0.3f;
	GLfloat minSize = 0.05f;

	GLclampf r;
	GLclampf g;
	GLclampf b;

	STATE state = IDLE;
	DIR dir = RIGHT;
	GLboolean isExtend = true;
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);
void TimerFunction(int);

vector<Rect*> rectVec;
GLboolean isAnimation = true;
GLboolean isMoveDiagonal = false;
GLboolean isMoveZigzag = false;
GLboolean isChangeSize = false;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WINDOWX, WINDOWY); // �������� ũ�� ����
	glutCreateWindow("Example4"); // ������ ����(������ �̸�)

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
	glutKeyboardFunc(Keyboard);	// Ű���� �Է� �ݹ��Լ� ����
	glutMouseFunc(Mouse);	// ���콺 �Լ� ����
	glutTimerFunc(10, TimerFunction, 1);	// Ÿ�̸� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f); // ������ £�� ȸ��
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	for (int i = 0; i < rectVec.size(); ++i) rectVec[i]->Draw();

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
	case 'a': case 'A':	// �밢�� �̵�
		isMoveDiagonal = ~isMoveDiagonal;
		break;
	case 'i': case 'I':	// ������� �̵�
		isMoveZigzag = ~isMoveZigzag;
		break;
	case 'c': case 'C':	// ũ�� ����
		isChangeSize = ~isChangeSize;
		break;
	case 's': case 'S':	// ��� �ִϸ��̼� ����
		isMoveDiagonal = isMoveZigzag = isChangeSize = false;
		break;
	case 'm': case 'M':	// ���� ��ġ�� �̵�
		isMoveDiagonal = isMoveZigzag = isChangeSize = false;
		for (int i = 0; i < rectVec.size(); ++i) rectVec[i]->MoveToOriginalPos();
		break;
	case 'r': case 'R':	// �簢�� ��� ����, �ٽ� �Է¹���
		rectVec.clear();
		break;
	case 'q': case 'Q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	// �簢�� ���콺 ��ǥ�� ����
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (rectVec.size() < 5)
		{
			float mouseX = ((float)x) / (WINDOWX / 2) - 1.0;
			float mouseY = -(((float)y) / (WINDOWY / 2) - 1.0);
			Rect* r = new Rect();
			r->Initialize(mouseX, mouseY);
			rectVec.push_back(r);
		}
	}
}

void TimerFunction(int value)
{
	for (int i = 0; i < rectVec.size(); ++i)
	{
		if (isMoveDiagonal)
			rectVec[i]->MoveDiagonal();
		if (isMoveZigzag)
			rectVec[i]->MoveZigzag();
		if (isChangeSize)
			rectVec[i]->ChangeSize();
	}

	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

void Rect::Initialize(GLfloat _x, GLfloat _y)
{
	originalX = x = _x;
	originalY = y = _y;

	random_device rd;
	uniform_int_distribution<int> uid(0, 255);

	r = (GLclampf)(uid(rd) / 255.0f);
	g = (GLclampf)(uid(rd) / 255.0f);
	b = (GLclampf)(uid(rd) / 255.0f);

	if (isMoveDiagonal) state = Rect::DIAGONAL;
	else if (isMoveZigzag) state = Rect::ZIGZAG;
	else if (isChangeSize) state = Rect::SIZE;
}

void Rect::Draw()
{
	glColor3f(r, g, b);
	glRectf(x - size, y - size, x + size, y + size);
}

void Rect::MoveDiagonal()
{
	if (dir == Rect::LEFT)
	{
		x -= 0.01f;
		y -= 0.01f;

		if (x - size <= -1.0f || y - size <= -1.0f) dir = Rect::RIGHT;
	}
	else if (dir == Rect::RIGHT)
	{
		x += 0.01f;
		y += 0.01f;

		if (x + size >= 1.0f || y + size >= 1.0f) dir = Rect::LEFT;
	}
}

void Rect::MoveZigzag()
{
	if (dir == Rect::LEFT)
	{
		x -= 0.01f;

		if (x - size <= -1.0f) dir = Rect::RIGHT;
	}
	else if (dir == Rect::RIGHT)
	{
		x += 0.01f;

		if (x + size >= 1.0f) dir = Rect::LEFT;
	}
}

void Rect::ChangeSize()
{
	if (isExtend)
	{
		if (size <= maxSize) size += 0.001f;
		else isExtend = false;
	}

	else
	{
		if (size >= minSize) size -= 0.001f;
		else isExtend = true;
	}
}

void Rect::MoveToOriginalPos()
{
	x = originalX;
	y = originalY;
}
