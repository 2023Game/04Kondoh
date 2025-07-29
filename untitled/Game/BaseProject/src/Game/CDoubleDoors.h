#pragma once
#include "CObjectBase.h"

class CSingleDoor;
class CInteractObject;

class CDoubleDoors : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDoubleDoors();
	// �f�X�g���N�^
	~CDoubleDoors();

	// �ڑ�����X�C�b�`��ǉ�
	void AddInputObjs(CInteractObject* sw);
	// �����̊J�������̊e���W��ݒ�
	void SetAnimPosL(const CVector& openPosL, const CVector& closePosL);
	// �E���̊J�������̊e���W��ݒ�
	void SetAnimPosR(const CVector& openPosR, const CVector& closePosR);
	// �X�V����
	void Update();

private:

	bool mIsOpened;

	CSingleDoor* mpLDoor;
	CSingleDoor* mpRDoor;
};