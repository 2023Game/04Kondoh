#include "CEnemy3.h"
#include "CEffect.h"

#define OBJ "res\\f16.obj"
#define MTL "res\\f16.mtl"

CModel CEnemy3::sModel;

//�f�t�H���g�R���X�g���N�^
CEnemy3::CEnemy3()
	: CCharacter3(1)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
{
	//���f���������Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &sModel;
}

//�R���X�g���N�^
//CEnemy(�ʒu�A��]�A�g�k)
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CEnemy3()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position; //�ʒu�̐ݒ�
	mRotation = rotation; //��]�̐ݒ�
	mScale = scale;       //�g�k�̐ݒ�
	CTransform::Update(); //�s��̐ݒ�
}

//�X�V����
void CEnemy3::Update()
{
}

//�Փˏ���
//Collision(�R���C�_�P�A�R���C�_�Q)
void CEnemy3::Collision(CCollider* m, CCollider* o)
{
	if (CCollider::Collision(m, o)){
		//�G�t�F�N�g����
		new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
	}
}

void CEnemy3::Collision()
{
	mCollider.ChangePriority();

	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}





