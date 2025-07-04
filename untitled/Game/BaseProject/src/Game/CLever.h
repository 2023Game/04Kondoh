#pragma once
#include "CObjectBase.h"
#include "CInteractObject.h"
#include "CModel.h"

class CLever : public CInteractObject
{
public:
	CLever(const CVector& pos);
	~CLever();

	// ���ׂ�
	void Interact() override;

	bool IsOn() const;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	CModel* mpLeverBaseModel;	// ���o�[�̓y��̃��f��
	CModel* mpLeverModel;		// ���o�[�̃��f��

	CColliderMesh* mpLeverBaseCol;// ���o�[�̓y��R���C�_�[
	CColliderMesh* mpLeverCol;	// ���o�[�̃R���C�_�[

	CTransform* mpParentObj;

	bool mIsPlaying;	// ���o�[�̃A�j���[�V�����Đ�����
	bool mIsOn;			// ���o�[���I���ɂȂ��Ă��邩


	float mLeverAngle;	// ���o�[�̌��݂̊p�x
	float mStartAngle;	// ���o�[��]�J�n���̊p�x
	float mEndAngle;	// ���o�[��]�I�����̊p�x
	float mElapsedTime;

};