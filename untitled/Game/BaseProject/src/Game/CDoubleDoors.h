#pragma once

class CInteractObject;
class CLDoubleDoor;
class CRDoubleDoor;

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

protected:

	bool IsSwitchOn() const;

	CVector mOpenPosL;	// �����̊J�������̈ʒu
	CVector mClosePosL;	// �����̕܂������̈ʒu
	CVector mOpenPosR;	// �E���̊J�������̈ʒu
	CVector mClosePosR;	// �E���̕܂������̈ʒu

private:

	// ���͌n�̃I�u�W�F�N�g���X�g
	std::list<CInteractObject*> mpInputObjs;

	CLDoubleDoor* mpLDoor;
	CRDoubleDoor* mpRDoor;

};