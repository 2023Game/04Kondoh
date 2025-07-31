#include "CDoubleDoors.h"
#include "CSingleDoor.h"
#include "CInteractObject.h"

#define ROTATE_Y CVector(0.0f, 180.0f, 0.0f)

CDoubleDoors::CDoubleDoors(const CVector& posL, const CVector& posR,
	const CVector& angle, const CVector& size)
	: CObjectBase(ETag::eDoor, ETaskPriority::eBackground)
	, mIsOpened(false)
	, mpLDoor(nullptr)
	, mpRDoor(nullptr)
{

	mpLDoor = new CSingleDoor(posL, angle, size);
	mpRDoor = new CSingleDoor(posR, -angle + ROTATE_Y, size);
}

CDoubleDoors::~CDoubleDoors()
{

}

// �����̊J�������̊e���W��ݒ�
void CDoubleDoors::SetAnimPosL(const CVector& openPosL, const CVector& closePosL)
{
	mpLDoor->SetAnimPos(openPosL, closePosL);
}

// �E���̊J�������̊e���W��ݒ�
void CDoubleDoors::SetAnimPosR(const CVector& openPosR, const CVector& closePosR)
{
	mpRDoor->SetAnimPos(openPosR, closePosR);
}


// �ڑ�����X�C�b�`��ǉ�
void CDoubleDoors::AddInputObjs(CInteractObject* sw)
{
	mpLDoor->AddInputObjs(sw);
	mpRDoor->AddInputObjs(sw);
}

void CDoubleDoors::Update()
{
}
