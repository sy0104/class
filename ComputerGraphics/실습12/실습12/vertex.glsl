#version 330 core
layout (location = 0) in vec3 vPos;		// ���� ���α׷����� �޾ƿ� ���� ��ǥ��
layout (location = 1) in vec3 in_Color;

out vec3 out_Color;		// fs���� ����

uniform mat4 transform;		// ��ȯ���: uniform���� �����Ͽ� ���� ���α׷����� ���� �����Ѵ�.

void main()
{
	gl_Position = transform * vec4(vPos, 1.0);	// ��ü�� ��ǥ�� ��ȯ ����� �����Ѵ�.
	out_Color = in_Color;
}