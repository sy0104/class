#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WINDOWX, WINDOWY); // 윈도우의 크기 지정
	glutCreateWindow("Example5"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized" << std::endl;

	// 배경 사각형 100개 생성
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

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);	// 키보드 입력 콜백함수 지정
	glutMouseFunc(Mouse);	// 마우스 함수 지정
	glutMotionFunc(Motion);	// 모션 함수 지정
	glutTimerFunc(10, TimerFunction, 1);	// 타이머 함수 설정
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 바탕색 검은색
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
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

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
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

	// 지우개와 사각형 충돌체크
	if (left <= eraser.x + eraser.size && right >= eraser.x - eraser.size &&
		top >= eraser.y - eraser.size && bottom <= eraser.y + eraser.size)
	{
		return true;
	}


	return false;
}
