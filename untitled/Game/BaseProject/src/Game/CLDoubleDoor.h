#pragma once
#include "CSingleDoor.h"

class CLDoubleDoor : public CSingleDoor
{
public:
	// コンストラクタ
	CLDoubleDoor(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CLDoubleDoor();

private:

};
