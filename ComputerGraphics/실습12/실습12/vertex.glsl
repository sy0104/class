#version 330 core
layout (location = 0) in vec3 vPos;		// 응용 프로그램에서 받아온 도형 좌표값
layout (location = 1) in vec3 in_Color;

out vec3 out_Color;		// fs에게 전달

uniform mat4 transform;		// 변환행렬: uniform으로 선언하여 응용 프로그램에서 값을 저장한다.

void main()
{
	gl_Position = transform * vec4(vPos, 1.0);	// 객체의 좌표에 변환 행렬을 적용한다.
	out_Color = in_Color;
}