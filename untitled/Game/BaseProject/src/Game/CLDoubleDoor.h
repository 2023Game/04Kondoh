#pragma once
#include "CModel.h"
#include "CColliderMesh.h"

class CInteractObject;

class CLDoubleDoor : public CDoubleDoors
{
public:
	// �R���X�g���N�^
	CLDoubleDoor(CVector pos, CVector angle, CVector size);
	// �f�X�g���N�^
	~CLDoubleDoor();

	// �ڑ�����X�C�b�`��ǉ�
	void AddInputObjs(CInteractObject* sw);
	// ���̊J�������̊e���W��ݒ�
	void SetAnimPos(const CVector& openPos, const CVector& closePos);
	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	//// �X�C�b�`�������Ĕ����J�����ǂ���
	//bool IsSwitchOn() const;

	CVector mOpenPos;	// �J�������̈ʒu
	CVector mClosePos;	// �܂������̈ʒu

	float mAnimTime;
	float mElapsedTime;

	bool mIsOpened;
	bool mIsPlaying;

	// ���͌n�̃I�u�W�F�N�g�̃��X�g
	std::list<CInteractObject*> mpInputObjs;

	CModel* mpLDoorModel;		// ���t�g�h�A�̃��f��
	CColliderMesh* mpLDoorCol;	// ���t�g�h�A�̃R���C�_�[

};
