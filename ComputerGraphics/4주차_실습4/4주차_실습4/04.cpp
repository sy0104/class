#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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
	void Initialize(GLfloat _x, GLfloat _y);	// 좌표, 크기, 색 설정
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

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WINDOWX, WINDOWY); // 윈도우의 크기 지정
	glutCreateWindow("Example4"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized" << std::endl;

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);	// 키보드 입력 콜백함수 지정
	glutMouseFunc(Mouse);	// 마우스 함수 지정
	glutTimerFunc(10, TimerFunction, 1);	// 타이머 함수 설정
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f); // 바탕색 짙은 회색
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	for (int i = 0; i < rectVec.size(); ++i) rectVec[i]->Draw();

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a': case 'A':	// 대각선 이동
		isMoveDiagonal = ~isMoveDiagonal;
		break;
	case 'i': case 'I':	// 지그재그 이동
		isMoveZigzag = ~isMoveZigzag;
		break;
	case 'c': case 'C':	// 크기 변함
		isChangeSize = ~isChangeSize;
		break;
	case 's': case 'S':	// 모든 애니메이션 멈춤
		isMoveDiagonal = isMoveZigzag = isChangeSize = false;
		break;
	case 'm': case 'M':	// 원래 위치로 이동
		isMoveDiagonal = isMoveZigzag = isChangeSize = false;
		for (int i = 0; i < rectVec.size(); ++i) rectVec[i]->MoveToOriginalPos();
		break;
	case 'r': case 'R':	// 사각형 모두 삭제, 다시 입력받음
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
	// 사각형 마우스 좌표로 생성
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
