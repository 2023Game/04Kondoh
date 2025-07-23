#pragma once
#include "CDoubleDoors.h"

class CInteractObject;

class CRDoubleDoor : public CDoubleDoors
{
public:
	// �R���X�g���N�^
	CRDoubleDoor(CVector pos, CVector angle, CVector size);
	// �f�X�g���N�^
	~CRDoubleDoor();

	// �ڑ�����X�C�b�`��ǉ�
	void AddInputObjs(CInteractObject* sw);
	// ���̊J�������̊e���W��ݒ�
	void SetAnimPos(const CVector& openPos, const CVector& clpsePos);
	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	// �X�C�b�`�������Ĕ����J�����ǂ���
	bool IsSwitchOn() const;

	CVector mOpenPos;	// �J�������̈ʒu
	CVector mClosePos;	// �܂������̈ʒu

	float mAnimTime;
	float mElapsedTime;

};
