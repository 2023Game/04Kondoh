#include "CFieldWall.h"
#include <assert.h>

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
//	CModel* fieldWallColMesh = CResourceManager::Get<CModel>("FieldWallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, mpModel, true);
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

// �X�V
void CFieldWall::Update()
{
}

// �`��
void CFieldWall::Render()
{
	mpModel->Render(Matrix());
}

CCollider* CFieldWall::GetFieldCol() const
{
	return mpColliderMesh;
}
