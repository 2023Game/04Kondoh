#include "CWall.h"

// �R���X�g���N�^
CWall::CWall(const CVector& pos, const CVector& angle, const CVector& size)
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
{
	// �ǂ̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Wall");

	// �ǂ̃R���C�_�[���쐬
	CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// �ǂ̌o�H�T���p�̃R���C�_�[���쐬
	CModel* navColModel = CResourceManager::Get<CModel>("WallNavCol");
	mpNavColliderMesh = new CColliderMesh(this, ELayer::eWall, navColModel, true);

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// �f�X�g���N�^
CWall::~CWall()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpColliderMesh);
	SAFE_DELETE(mpNavColliderMesh);
}

// ���C�Ƃ̕ǂƂ̏Փ˔���
bool CWall::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �ǂ̃R���C�_�[�����݂��Ȃ���΁A�Փ˂��Ă��Ȃ�
	if (mpColliderMesh == nullptr) return false;
	return CCollider::CollisionRay(mpColliderMesh, start, end, hit);
}

// ���C�Ƃ̕ǂƂ̏Փ˔���i�o�H�T���p�j
bool CWall::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �ǂ̃R���C�_�[�����݂��Ȃ���΁A�Փ˂��Ă��Ȃ�
	if (mpNavColliderMesh == nullptr) return false;
	return CCollider::CollisionRay(mpNavColliderMesh, start, end, hit);
}
// �X�V
void CWall::Update()
{
}

// �`��
void CWall::Render()
{
	mpModel->Render(Matrix());
}

CCollider* CWall::GetFieldCol() const
{
	return mpColliderMesh;
}
