#include "CEnemy.h"
#include "CEffect.h"
#include "CPlayer.h"

#define OBJ "res\\BoxMan_No3.obj"
#define MTL "res\\BoxMan_No3.mtl"
//�ړ����x
#define VELOCITY CVector(0.0f,0.0f,0.09f)

CModel CEnemy::sModel;

//�f�t�H���g�R���X�g���N�^
CEnemy::CEnemy()
	:CCharacter3(1)
	, mELine(this, &mMatrix, CVector(0.0f, 0.0f, 1.5f), CVector(0.0f, 0.0f, -1.5f))
	, mELine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -1.1f, 0.0f))
	, mELine3(this, &mMatrix, CVector(2.0f, 0.5f, 0.0f), CVector(-2.0f, 0.5f, 0.0f))
	, mELine4(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), CVector(0.0f, -1.1f, 0.0f))
	, mSphere(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), 0.5)
	, JumpV(0.0)
{
	//���f���������Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &sModel;
}

//CEnemy(�ʒu�A��]�A�g�k)
CEnemy::CEnemy(const CVector& position,
	const CVector& rotation, const CVector& scale)
	:CEnemy()
{
	//�ʒu�A��]�A�g�k
	mPosition = position; //�ʒu�̐ݒ�
	mRotation = rotation; //��]�̐ݒ�
	mScale = scale;       //�g�k�̐ݒ�
	CTransform::Update(); //�s��̐ݒ�
}

//�X�V����
void CEnemy::Update() {
	//�s����X�V
	CTransform::Update();
	switch (mEneState)
	{
	case EneState::EIDEL:


		Rotation(CVector(0.0f, 0.0f, 0.0f));

		break;
	}

}


//�Փˏ���
//CCollision(�R���C�_�P,�R���C�_�Q)
void CEnemy::Collision(CCollider* m, CCollider* o) {

	//���g�̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::EType::ESPHERE: //���R���C�_�̎�
		//����̃R���C�_���O�p�`�R���C�_�̎�
		if (o->Type() == CCollider::EType::ELINE) {
			CVector adjust; //�����p�x�N�g��

			//�R���C�_��m��o���Փ˂��Ă��邩����
			if (CCollider::Collision(&mSphere, &CPlayer::Instance()->mPLine)) {
				mPosition = mPosition + CVector(0.0f, 0.0f, 10.0f);
			}
			/*
			if (CCollider::CollisionTriangleSphere(&CPlayer::Instance()->mPLine, m, &adjust) && JumpV != 0)
			{
				JumpV = 0;
				//CTransform::Update();
			}

			
			//�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//�ʒu�X�V
				mPosition = mPosition + adjust;
				//�s��X�V
				CTransform::Update();
			}
			*/
			
		}
		break;

	case CCollider::EType::ETRIANGLE: //�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{   //�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
		}
		break;
	}
}

//�Փˏ���
void CEnemy::Collision()
{

	//�R���C�_�̗D��x�ύX
	mELine4.ChangePriority();
	mELine.ChangePriority();
	mELine2.ChangePriority();
	mELine3.ChangePriority();
	mSphere.ChangePriority();

	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mELine4, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mELine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mELine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mELine3, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mSphere, COLLISIONRANGE);

	////�R���C�_�̗D��x
	//mCollider.ChangePriority();
	////�Փˏ��������s
	//CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

