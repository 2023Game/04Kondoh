#include "CEnemy.h"
#include "CEffect.h"

#define OBJ "res\\BoxMan_No3.obj"
#define MTL "res\\BoxMan_No3.mtl"
//�ړ����x
#define VELOCITY CVector(0.0f,0.0f,0.09f)

CModel CEnemy::sModel;

//�f�t�H���g�R���X�g���N�^
CEnemy::CEnemy()
	:CCharacter3(1)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
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
	//�ʒu���ړ�
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}


//�Փˏ���
//CCollision(�R���C�_�P,�R���C�_�Q)
void CEnemy::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::EType::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o)) {
			//�G�t�F�N�g����
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//�Փ˂��Ă��鎞�͖���
			//mEnabled = false;
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
	//�R���C�_�̗D��x
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

