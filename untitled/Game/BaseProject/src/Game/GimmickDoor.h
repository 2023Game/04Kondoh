#pragma once
#include "CInteractObject.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CLever;

class CGimmickDoor : public CObjectBase
{
	// �R���X�g���N�^
	CGimmickDoor(CVector pos, CVector angle, CVector size);

	// �f�X�g���N�^
	~CGimmickDoor();


	// �ڑ�����X�C�b�`��ǉ�
	void AddInputObjs(CInteractObject* sw);
	//�A�j���[�V��������Ƃ��̈ړ��ʒu
	void SetAnimPos(const CVector& openPos, const CVector& closePos);


	// �X�V����
	void Update() override;

	// �`�揈��
	void Render() override;

private:
	// �����J�����ǂ���
	bool IsSwitchOn() const;

	bool mIsOpened;	// �J���Ă��邩�ǂ���

	CVector mOpenPos;
	CVector mClosePos;

	float mAnimTime;
	float mElapsedTime;

	// �e�h�A�̃��f���f�[�^

	CModel* mpRDoubleDoorsModel;
	CModel* mpLDoubleDoorsModel;
	CModel* mpRastDoorModel;
	// �e�h�A�̃R���C�_�[

	CColliderMesh* mpRDoubleDoorsCol;
	CColliderMesh* mpLDoubleDoorsCol;
	CColliderMesh* mpRastDoorCol;
};
