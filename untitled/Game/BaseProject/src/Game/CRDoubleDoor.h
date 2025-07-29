#pragma once
#include "CSingleDoor.h"


class CRDoubleDoor : public CSingleDoor
{
public:
	// コンストラクタ
	CRDoubleDoor(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CRDoubleDoor();

private:

};
