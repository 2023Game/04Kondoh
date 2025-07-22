#pragma once
#include "CObjectBase.h"
#include "CInteractObject.h"
#include "CModel.h"

class CLever : public CInteractObject
{
public:
	CLever(const CVector& pos, const CVector& angle);
	~CLever();

	// ���ׂ�
	void Interact() override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	CModel* mpLeverBaseModel;	// ���o�[�̓y��̃��f��
	CModel* mpLeverModel;		// ���o�[�̃��f��

	CColliderMesh* mpLeverBaseCol;// ���o�[�̓y��R���C�_�[
	CColliderCapsule* mpLeverCol;	// ���o�[�̃R���C�_�[

	float mLeverAngle;	// ���o�[�̌��݂̊p�x
	float mStartAngle;	// ���o�[��]�J�n���̊p�x
	float mEndAngle;	// ���o�[��]�I�����̊p�x
	float mElapsedTime;	// �o�ߎ��Ԍv���p

};