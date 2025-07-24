#pragma once
#include "CModel.h"
#include "CColliderMesh.h"

class CInteractObject;

class CSingleDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSingleDoor(CVector pos, CVector angle, CVector size);
	// �f�X�g���N�^
	~CSingleDoor();

	// �ڑ�����X�C�b�`��ǉ�
	void AddInputObjs(CInteractObject* sw);
	// ���̊J�������̊e���W��ݒ�
	void SetAnimPos(const CVector& openPos, const CVector& closePos);
	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	// �X�C�b�`�������Ĕ����J�����ǂ���
	bool IsSwitchOn() const;

	CVector mOpenPosR; // �J�������̈ʒu
	CVector mClosePosR;// �܂������̈ʒu

	float mAnimTime;
	float mElapsedTime;

	bool mIsOpened;  // �J�������H
	bool mIsPlaying;

	// ���͌n�̃I�u�W�F�N�g�̃��X�g
	std::list<CInteractObject*> mpInputObjs;

	CModel* mpDoorModel; // �V���O���h�A�̃��f��
	CColliderMesh* mpDoorCol; // �V���O���h�A�̃R���C�_�[
};
