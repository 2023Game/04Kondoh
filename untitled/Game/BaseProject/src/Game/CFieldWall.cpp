#include "CFieldWall.h"
#include <assert.h>
#include "CNavManager.h"
#include "CNavNode.h"

CFieldWall* CFieldWall::spInstance = nullptr;

CFieldWall* CFieldWall::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CFieldWall::CFieldWall()
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
{
	assert(spInstance == nullptr);
	spInstance = this;

	// �X�e�[�W1�̕ǂ̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("FieldWall");
	// �X�e�[�W�ǂ̃R���C�_���쐬
	CModel* fieldWallColMesh = CResourceManager::Get<CModel>("FieldWallCol");

	mpColliderMesh = new CColliderMesh
	(
		this, ELayer::eWall, fieldWallColMesh,
		true, 1.0f,
		8, 2, 8
	);
	//mpColliderMesh->SetShow(true);
}

// �f�X�g���N�^
CFieldWall::~CFieldWall()
{
	spInstance = nullptr;
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

// ���C�Ƃ̕ǂƂ̏Փ˔���i�o�H�T���p�j
bool CFieldWall::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �ǂ̃R���C�_�[�����݂��Ȃ���΁A�Փ˂��Ă��Ȃ�
	if (mpColliderMesh == nullptr) return false;
	return CCollider::CollisionRay(mpColliderMesh, start, end, hit);
}


// �X�V
void CFieldWall::Update()
{
}

// �`��
void CFieldWall::Render()
{
	mpModel->Render(Matrix());
}

CCollider* CFieldWall::GetFieldWallCol() const
{
	return mpColliderMesh;
}
