#pragma once
#include "CModel.h"
#include "CColliderMesh.h"

class CInteractObject;

class CRDoubleDoor : public CDoubleDoors
{
public:
	// �R���X�g���N�^
	CRDoubleDoor(CVector pos);
	// �f�X�g���N�^
	~CRDoubleDoor();

	// ���̊J�������̊e���W��ݒ�
//	void SetAnimPosR(const CVector& openPos, const CVector& clpsePos) override;
	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:

	//CVector mOpenPosR;	// �J�������̈ʒu
	//CVector mClosePosR;	// �܂������̈ʒu

	float mAnimTime;
	float mElapsedTime;


	bool mIsPlaying;

	CModel* mpRDoorModel;		// �E���̃��f��
	CColliderMesh* mpRDoorCol;	// �E���̃R���C�_�[

};
