#include "CDoubleDoors.h"
#include "CLDoubleDoor.h"
#include "CRDoubleDoor.h"
#include "CInteractObject.h"


CDoubleDoors::CDoubleDoors()
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
	, mIsOpened(false)
	, mpLDoor(nullptr)
	, mpRDoor(nullptr)
{

	mpLDoor = new CLDoubleDoor(CVector(0.0f, 0.0f, 0.0f));
	mpRDoor = new CRDoubleDoor(CVector(0.0f, 0.0f, 0.0f));
}

CDoubleDoors::~CDoubleDoors()
{

}

// �����̊J�������̊e���W��ݒ�
void CDoubleDoors::SetAnimPosL(const CVector& openPosL, const CVector& closePosL)
{
	mOpenPosL = openPosL;
	mClosePosL = closePosL;
	Position(mIsOpened ? mOpenPosL : mClosePosL);
}

// �E���̊J�������̊e���W��ݒ�
void CDoubleDoors::SetAnimPosR(const CVector& openPosR, const CVector& closePosR)
{
	mOpenPosR = openPosR;
	mClosePosR = closePosR;
	Position(mIsOpened ? mOpenPosR : mClosePosR);
}


// �ڑ�����X�C�b�`��ǉ�
void CDoubleDoors::AddInputObjs(CInteractObject* sw)
{
	mpInputObjs.push_back(sw);
}

// �X�C�b�`�������Ĕ����J�����ǂ���
bool CDoubleDoors::IsSwitchOn() const
{
	for (CInteractObject* sw : mpInputObjs)
	{
		if (!sw->IsOnInteractObj()) return false;
	}
	return true;
}

void CDoubleDoors::Update()
{
}
