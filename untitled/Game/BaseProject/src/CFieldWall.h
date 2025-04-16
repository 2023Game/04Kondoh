#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// �X�e�[�W�̕Ǖ\���N���X
class CFieldWall : public CObjectBase
{
public:
	// �X�e�[�W1�̕ǂ̃C���X�^���X
	static CFieldWall* Instance();
	// �R���X�g���N�^
	CFieldWall();
	// �f�X�g���N�^
	~CFieldWall();

	// �X�V
	void Update();
	// �`��
	void Render();

	// �R���C�_�擾����
	CCollider* GetFieldCol()const;

private:
	// �X�e�[�W�̕ǂ̃C���X�^���X
	static CFieldWall* spInstance;

	// �X�e�[�W1�̕ǂ̃��f���f�[�^
	CModel* mpModel;
	// �X�e�[�W1�̕ǂ̃R���W�����f�[�^
	CColliderMesh* mpColliderMesh;
};