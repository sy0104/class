#pragma once
#include "MonoBehaviour.h"

enum class ITEM
{
	/*�Ϲ� ������*/	 SPEED_UP, TELEPORT, SHIELD, SPEED_DOWN, BLIND,
	/*����ũ ������*/ DEBUFF_OFF, STUN,
	END
};

// �� �÷��� Ÿ�� 3��
// �Ϲ� �������� 30�ʿ� 5�� ����
// ����ũ �������� 1�п� 2�� ���� - ����
// �����ؾߵ�

class Item : public MonoBehaviour
{
public:
	Item();
	Item(ITEM itemType);
	virtual ~Item();

public:
	virtual void Update() override;

public:
	ITEM GetItemType() { return _itemType; }


private:
	ITEM	_itemType = ITEM::END;

	float	_curTime = 0.f;
	float	_itemTime = 5.f;
};

