#pragma once
#include "Component.h"

// �츮�� ���������� ���� ���� Ŀ���� ��ǰ�� ..
class GameObject;

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

private:
	// MonoBehaviour�� ��ӹ��� �ֵ��� FinalUpdate�� ������� ���ϵ��� ���Ƶ�
	// �������� ������ ����� �ϰ���� ���� ����ϴ� �Լ�
	virtual void FinalUpdate() sealed {}
};

