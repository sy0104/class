#include "pch.h"
#include "Item.h"
#include "Timer.h"

Item::Item()
{
}

Item::Item(ITEM itemType)
{
	_itemType = itemType;
}

Item::~Item()
{
}

void Item::Update()
{
	// 30�ʸ��� �Ϲ� ������ 5���� ������
	_curTime += DELTA_TIME;

	if (_curTime >= _itemTime)
	{


		_curTime = 0.f;
	}
}
