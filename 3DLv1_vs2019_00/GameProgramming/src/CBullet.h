#ifndef CBULLET_H
#define CBULLET_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"
//�O�p�`�N���X�̃C���N���[�h
#include "CTriangle.h"
#include "CModel.h"
#include "CCollider.h"

/*
�e�N���X
�O�p�`���΂�
*/

class CBullet : public CCharacter3 {
public:
	//�Փˏ���
	//Collision(�R���C�_�P,�R���C�_�Q)
	void Collision(CCollider* m, CCollider* o);
	CBullet();
	//���Ɖ��s���̐ݒ�
	//Set(��,���s)
	void Set(float w, float d);
	//�X�V
	void Update();
	//�`��
	void CBullet::Render();

private:
	//��������
	int mLife;
	//�O�p�`
	CTriangle mT;
	CCollider mCollider;

};

#endif // !CBULLET_H
