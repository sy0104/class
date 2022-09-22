#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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
	GLfloat x1;	// 좌측 하단 x
	GLfloat y1;	// 좌측 하단 y
	GLfloat x2;	// 우측 상단 x
	GLfloat y2;	// 우측 상단 y
}RECTANGLE;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int, int, int, int);
bool isCollision(RECTANGLE, int, int);

RGB backRGB = { 0.0f, 0.0f, 1.0f };
RGB rectRGB = { 1.0f, 1.0f, 1.0f };;
RECTANGLE rect = { -0.4f, -0.4f, 0.4f, 0.4f };

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WINDOWX, WINDOWY); // 윈도우의 크기 지정
	glutCreateWindow("Example2"); // 윈도우 생성(윈도우 이름)

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
	glutMouseFunc(Mouse);	// 마우스 함수 지정
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(backRGB.r, backRGB.g, backRGB.b, 1.0f); // 바탕색 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glColor3f(rectRGB.r, rectRGB.g, rectRGB.b);
	glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
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
		// 사각형 색상 랜덤하게 변경
		if (isCollision(rect, x, y))
		{
			rectRGB.r = (GLclampf)(uid(rd) / 255.0f);
			rectRGB.g = (GLclampf)(uid(rd) / 255.0f);
			rectRGB.b = (GLclampf)(uid(rd) / 255.0f);
		}
		// 배경색 랜덤하게 변경
		else
		{
			backRGB.r = (GLclampf)(uid(rd) / 255.0f);
			backRGB.g = (GLclampf)(uid(rd) / 255.0f);
			backRGB.b = (GLclampf)(uid(rd) / 255.0f);
		}
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// 사각형 크기 확대
		if (isCollision(rect, x, y))
		{
			rect.x1 -= sizeX;
			rect.y1 -= sizeY;
			rect.x2 += sizeX;
			rect.y2 += sizeY;
		}
		// 사각형 크기 축소
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
	// 마우스 좌표가 사각형 안에 있는지 확인하는 함수
	float mouseX = ((float)x) / (WINDOWX / 2) - 1.0;
	float mouseY = -(((float)y) / (WINDOWY / 2) - 1.0);

	if (mouseX >= r.x1 && mouseX <= r.x2 && mouseY >= r.y1 && mouseY <= r.y2)
	{
		int a = 0;
		return true;
	}

	return false;
}
