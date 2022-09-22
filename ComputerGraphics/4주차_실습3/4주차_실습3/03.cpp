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

GLboolean isLeftDown = false;
GLboolean isMove = false;
auto moveRect = rectVec.rbegin();

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

	if (isMove)
	{
		GLfloat x1 = (*moveRect).x - (*moveRect).size;
		GLfloat y1 = (*moveRect).y - (*moveRect).size;
		GLfloat x2 = (*moveRect).x + (*moveRect).size;
		GLfloat y2 = (*moveRect).y + (*moveRect).size;

		glColor3f((*moveRect).r, (*moveRect).g, (*moveRect).b);
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
		// 마우스가 사각형을 아직 클릭하지 않았으면 충돌체크해서 해당 사각형 선택
		if (!isMove)
		{
			// 사각형이 겹쳐있으면 나중에 만든 사각형이 선택됨
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

		// 마우스 좌표에 따라 사각형 움직임
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
