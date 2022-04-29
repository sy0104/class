#pragma once
#include "MonoBehaviour.h"

// �÷��̾�� ������ ī�޶�
class CameraScript : public MonoBehaviour
{
public:
	CameraScript();
	virtual ~CameraScript();

	virtual void LateUpdate() override;

public:
	void Revolve(float rotSize, Vec3 playerPos);

private:
	float		_speed = 5.f;
	Vec3		_offset;

	///////
	shared_ptr<GameObject>	_player = make_shared<GameObject>();
	XMFLOAT3 m_xmf3Offset;		// �÷��̾�� ī�޶��� �������� ��Ÿ���� ����. �ַ� 3��Ī ī�޶󿡼� ����
	///////
};

