#ifndef CENEMY_H
#define CENEMY_H

//�L�����N�^�[�N���X�̃C���N���[�h
#include "CCharacter3.h"
#include "CCollider.h"
#include "CColliderLine.h"
#include "CColliderMesh.h"
#include "CCollisionManager.h"
/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CCharacter3 {
public:  
	enum class EneState  //�v���C���[�s���X�e�[�g
	{
		EIDEL,   //�ҋ@
		EWANING, //�x��
		ECHASE,  //�ǐ�
		ERETUM   //�߂�
	};
	EneState mEneState;
	//�R���X�g���X�^
	CEnemy();
	//CEnemy(�ʒu�A��]�A�g�k)
	CEnemy(const CVector& position,
		const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	void Collision();
	//Collision(�R���C�_�P,�R���C�_�Q)
	void Collision(CCollider* m, CCollider* o);

	float JumpV;  //�W�����v�ϐ�
//	bool 

private:

	CCollider mSphere;  //���R���C�_

	CColliderLine mELine;  //�����R���C�_
	CColliderLine mELine2;
	CColliderLine mELine3;
	CColliderLine mELine4;

	static CModel sModel;
};

#endif // !CENEMY_H
