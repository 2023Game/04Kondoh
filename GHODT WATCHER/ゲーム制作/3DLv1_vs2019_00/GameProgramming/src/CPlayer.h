#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"
#include "CInput.h"
#include "CBullet.h"
#include "CColliderLine.h"
#include "CCollisionManager.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/

class CPlayer : public CCharacter3 {
public:

	enum class EPState  //�v���C���[�s���X�e�[�g
	{
		EMOVE,  //�ړ�
		EJUMP,  //�W�����v
		EIDEL,  //�ҋ@
		ESHOOT, //�U��
	};
	EPState mPState;
	CVector RanX;   //���ړ�
	CVector RanZ;   //�c�ړ�
	float JumpV;  //�W�����v�ϐ�

	float ShootTime;

	//�C���X�^���X�̃|�C���g�̎擾
	static CPlayer* Instance();
	//�Փˏ���
	void Collision();
	//�Փ˔���
	void Collision(CCollider* m, CCollider* o);
	//CBullet bullet;
	CPlayer();
	//CPlayer(�ʒu�A��]�A�X�P�[��)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//�X�V����
	void Update();
private:
	//�v���C���[�̃C���X�^���X
	static CPlayer* spInstance; 
	CColliderLine mLine;  //�����R���C�_
	CColliderLine mLine2;
	CColliderLine mLine3;

	CColliderLine mLine4;
	//�l�p
	CColliderLine mLine5;
	CColliderLine mLine6;
	CColliderLine mLine7;
	CColliderLine mLine8;
	//
	CColliderLine mLine9;
	CColliderLine mLine10;
	CColliderLine mLine11;
	CColliderLine mLine12;
	CInput mInput;
};

#endif // !CPLAYER_H
