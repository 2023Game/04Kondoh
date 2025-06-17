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

	/// <summary>
	/// ���C�Ƃ̕ǂƂ̏Փ˔���i�o�H�T���p�j
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <returns>�Փ˂��Ă�����Atrue��Ԃ�</returns>
	bool NavCollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit);

	// �X�V
	void Update();
	// �`��
	void Render();

	// �R���C�_�擾����
	CCollider* GetNavCol()const;

private:
	// �X�e�[�W�̕ǂ̃C���X�^���X
	static CFieldWall* spInstance;

	// �X�e�[�W1�̕ǂ̃��f���f�[�^
	CModel* mpModel;
	// �X�e�[�W1�̕ǂ̃R���W�����f�[�^
	CColliderMesh* mpColliderMesh;
};