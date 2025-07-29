#include "CDoubleDoors.h"
#include "CSingleDoor.h"
#include "CInteractObject.h"


CDoubleDoors::CDoubleDoors()
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
	, mIsOpened(false)
	, mpLDoor(nullptr)
	, mpRDoor(nullptr)
{

	mpLDoor = new CSingleDoor
	(
		CVector(50.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	mpRDoor = new CSingleDoor
	(
		CVector(50.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
}

CDoubleDoors::~CDoubleDoors()
{

}

// 左扉の開閉した時の各座標を設定
void CDoubleDoors::SetAnimPosL(const CVector& openPosL, const CVector& closePosL)
{
	mpLDoor->SetAnimPos(openPosL, closePosL);
}

// 右扉の開閉した時の各座標を設定
void CDoubleDoors::SetAnimPosR(const CVector& openPosR, const CVector& closePosR)
{
	mpRDoor->SetAnimPos(openPosR, closePosR);
}


// 接続するスイッチを追加
void CDoubleDoors::AddInputObjs(CInteractObject* sw)
{
	mpLDoor->AddInputObjs(sw);
	mpRDoor->AddInputObjs(sw);
}

void CDoubleDoors::Update()
{
}
