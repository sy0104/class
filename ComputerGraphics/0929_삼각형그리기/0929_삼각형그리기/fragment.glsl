#version 330 core

// out_Color: vs���� �Է¹޴� ���� ��
// FragColor: ����� ������ ������ ������ ���۷� ���޵�

in vec3 out_Color;		// vs���Լ� ���޹���
out vec4 FragColor;		// ���� ���

void main()
{
	FragColor = vec4(out_Color, 1.0);
}