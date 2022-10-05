#version 330 core

// out_Color: vs에서 입력받는 색상 값
// FragColor: 출력할 색상의 값으로 프레임 버퍼로 전달됨

in vec3 out_Color;		// vs에게서 전달받음
out vec4 FragColor;		// 색상 출력

void main()
{
	FragColor = vec4(out_Color, 1.0);
}