#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);

typedef struct RGB
{
	GLclampf r;
	GLclampf g;
	GLclampf b;
};

RGB rgb{ 255.0f, 255.0f, 255.0f };
bool isTimer = false;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // 초기 배경색 흰색

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else cout << "GLEW Initialized" << endl;

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);	// 키보드 입력 콜백함수 지정
	glutTimerFunc(100, TimerFunction, 1);	// 타이머 함수 설정
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(rgb.r, rgb.g, rgb.b, 1.0f); // 초기 배경색 흰색
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
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
	case 'R': case 'r':
		rgb = { 1.0f, 0.0f, 0.0f };
		break;
	case 'G': case 'g':
		rgb = { 0.0f, 1.0f, 0.0f };
		break;
	case 'B': case 'b':
		rgb = { 0.0f, 0.0f, 1.0f };
		break;
	case 'A': case 'a':	// 랜덤색
	{
		random_device rd;
		uniform_int_distribution<int> uid(0, 255);
		rgb.r = (GLclampf)(uid(rd) / 255.0f);
		rgb.g = (GLclampf)(uid(rd) / 255.0f);
		rgb.b = (GLclampf)(uid(rd) / 255.0f);
	}
		break;
	case 'W': case 'w':	// 백색
		rgb = { 1.0f, 1.0f, 1.0f };
		break;
	case 'K': case 'k':	// 검정색
		rgb = { 0.0f, 0.0f, 0.0f };
		break;
	case 'T': case 't':	// 타이머 설정, 특정 시간마다 색 랜덤으로 바뀜
		isTimer = true;
		break;
	case 'S': case 's':	// 타이머 종료
		isTimer = false;
		break;
	case 'Q': case 'q':	// 프로그램 종료
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

void TimerFunction(int value)
{
	if (isTimer)
	{
		random_device rd;
		uniform_int_distribution<int> uid(0, 255);
		rgb.r = (GLclampf)(uid(rd) / 255.0f);
		rgb.g = (GLclampf)(uid(rd) / 255.0f);
		rgb.b = (GLclampf)(uid(rd) / 255.0f);
	}

	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}
