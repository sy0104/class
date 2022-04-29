#pragma once
#include "InstancingBuffer.h"

class GameObject;

class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);

public:
	void Render(vector<shared_ptr<GameObject>>& gameObjects);

	void ClearBuffer();
	void Clear() { _buffers.clear(); }

	// ��ü�� vertex buffer, index buffer, material�� �Ȱ��Ƽ� ���� ���̴��� �׷����� ��� -> �����ϴ�
	// ������ material, mesh ���� ����ϴ� ��ü�鳢�� ������ �� �ֵ��� ID�� ������ -> instanceID
private:
	void AddParam(uint64 instanceId, InstancingParams& data);

private:
	map<uint64/*instanceId*/, shared_ptr<InstancingBuffer>> _buffers;
};

