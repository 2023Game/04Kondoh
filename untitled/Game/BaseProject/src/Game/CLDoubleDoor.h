#pragma once
#include "CModel.h"
#include "CColliderMesh.h"

class CInteractObject;

class CLDoubleDoor : public CDoubleDoors
{
public:
	// �R���X�g���N�^
	CLDoubleDoor(CVector pos);
	// �f�X�g���N�^
	~CLDoubleDoor();

	// ���̊J�������̊e���W��ݒ�
//	void SetAnimPosL(const CVector& openPos, const CVector& closePos) override;
	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:

	//CVector mOpenPosL;	// �J�������̈ʒu
	//CVector mClosePosL;	// �܂������̈ʒu

	float mAnimTime;
	float mElapsedTime;

	bool mIsOpened;
	bool mIsPlaying;

	CModel* mpLDoorModel;		// �����̃��f��
	CColliderMesh* mpLDoorCol;	// �E���̃R���C�_�[

};
