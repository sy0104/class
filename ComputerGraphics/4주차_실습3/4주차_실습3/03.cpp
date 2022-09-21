#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WINDOWX, WINDOWY); // 윈도우의 크기 지정
	glutCreateWindow("Example3"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized" << std::endl;

	// 첫번째 사각형 생성
	RECTANGLE r = { 0.0f, 0.0f, 0.15f, 1.0f, 0.0f, 0.0f };
	rectVec.push_back(r);

	r = { 0.05f, 0.05f, 0.15f, 1.0f, 1.0f, 0.0f };
	rectVec.push_back(r);

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);	// 마우스 함수 지정
	glutMotionFunc(Motion);	// 모션 함수 지정
	glutKeyboardFunc(Keyboard);	// 키보드 입력 콜백함수 지정
	glutTimerFunc(10, TimerFunction, 1);	// 타이머 함수 설정
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕 검은색
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	// 사각형 그리기
	for (int i = 0; i < rectVec.size(); ++i)
	{
		GLfloat x1 = rectVec[i].x - rectVec[i].size;
		GLfloat y1 = rectVec[i].y - rectVec[i].size;
		GLfloat x2 = rectVec[i].x + rectVec[i].size;
		GLfloat y2 = rectVec[i].y + rectVec[i].size;

		glColor3f(rectVec[i].r, rectVec[i].g, rectVec[i].b);
		glRectf(x1, y1, x2, y2);
	}

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
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
