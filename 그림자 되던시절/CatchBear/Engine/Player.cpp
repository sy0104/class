#include "pch.h"
#include "Player.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CameraScript.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::LateUpdate()
{
	KeyCheck();
}

void Player::KeyCheck()
{
	// ��������
	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
		::PostQuitMessage(0);

	Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 rot = GetTransform()->GetLocalRotation();

	// ���� ������ ī�޶�(Main_Camera)�� �����´�.
	// ī�޶� ������ �ִ� ��ũ��Ʈ(CameraScript)���� Followlayer()�� �����Ų��.
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_player = GetGameObject();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Main_Camera")
		{
			_camera = gameObject;
			break;
		}
	}

	_cameraScript = static_pointer_cast<CameraScript>(_camera->GetScript(0));

	// �̵�
	if (INPUT->GetButton(KEY_TYPE::UP))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::DOWN))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	// ȸ��
	float delta = 0.f;
	if (INPUT->GetButton(KEY_TYPE::RIGHT))
	{
		rot.y += DELTA_TIME * _rotSpeed;
		delta = DELTA_TIME * _rotSpeed;

		GetTransform()->SetLocalRotation(rot);
	}

	if (INPUT->GetButton(KEY_TYPE::LEFT))
	{
		rot.y -= DELTA_TIME * _rotSpeed;
		delta = -DELTA_TIME * _rotSpeed;

		GetTransform()->SetLocalRotation(rot);
	}

	GetTransform()->SetLocalPosition(pos);
	_cameraScript->Revolve(delta, GetTransform()->GetLocalPosition());
}
