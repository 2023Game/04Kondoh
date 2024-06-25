#ifndef CENEMY_H
#define CENEMY_H

//�L�����N�^�[�N���X�̃C���N���[�h
#include "CCharacter3.h"
#include "CCollider.h"
#include "CCollisionManager.h"
/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CCharacter3 {
public:  
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

private:
	//�R���C�_
	CCollider mCollider;
	static CModel sModel;
};

#endif // !CENEMY_H
