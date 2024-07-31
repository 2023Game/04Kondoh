#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//�L�����N�^�[�N���X�̃C���N���[�h
#include "CCharacter3.h"

/*
�R���C�_�N���X
�Փ˔���f�[�^
*/
class CCollider : public CTransform, public CTask {
	friend CCollisionManager;
public:
	
	//�D��x�̕ύX
	virtual void ChangePriority();
	//�D��x�̕ύX
	void ChangePriority(int priority);

	//CollisionSphaereLine(���R���C�_,�����R���C�_,�����l)
	//return:true(�Փ˂��Ă���)false(�Փ˂��Ă��Ȃ�)
	//�����l:�Փ˂��ĂȂ��ʒu�܂Ŗ߂�
	static bool CollisionSphereLine(CCollider* sphere, CCollider* line, CVector* adjust);
	
	//CollisionTriangleSphere(�O�p�R���C�_,���R���C�_,�����l)
	//retrun:true(�Փ˂��Ă���)false(�Փ˂��Ă��Ȃ�)
	//�����l:�Փ˂��Ȃ��ʒu�܂Ŗ߂��l
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere,
		CVector* adjust);

	//CollisionTriangleLine(�O�p�R���C�_,�����R���C�_,�����l)
	//return:true(�Փ˂��Ă���)false(�Փ˂��Ă��Ȃ�)
	//�����l�F�Փ˂��Ȃ��ʒu�܂Ŗ߂�
	static bool CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust);

	//�R���C�_�^�C�v
	enum class EType {
		ESPHERE,   //���R���C�_
		ETRIANGLE, //�O�p�R���C�_
		ELINE      //�����R���C�_
	};
	//�f�t�H���g�R���X�g���N�^
	CCollider();

	//�Փ˔���
	//CCollider(�R���C�_�P,�R���C�_�Q)
	//return:true(�Փ˂��Ă���),false(�Փ˂��Ă��Ȃ�)
	static bool Collision(CCollider* m, CCollider* o);
	~CCollider();
	//�R���X�g���N�^
	//CCollider(�e�A�e�s��A�ʒu�A���a)
	CCollider(CCharacter3* parent, CMatrix* matrix,
		const CVector& position, float radius);
	//�e�|�C���^�̎擾
	CCharacter3* Parent();
	//�`��
	void Render();
	EType Type();

protected:
	EType mType; //�R���C�_�^�C�v
	//���_
	CVector mV[3];
	CCharacter3* mpParent; //�e
	CMatrix* mpMatrix; //�e�s��
	float mRadius;  //���a

};


#endif // !CCOLLIDER_H

