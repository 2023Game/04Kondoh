#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// �Ǖ\���N���X
class CWall : public CObjectBase
{
public:
	// �R���X�g���N�^
	CWall(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CWall();

	/// <summary>
	/// ���C�Ƃ̕ǂƂ̏Փ˔���
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <returns>�Փ˂��Ă�����Atrue��Ԃ�</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	CModel* mpModel;    // �ǂ̃��f���f�[�^
	CColliderMesh* mpColliderMesh;  // �ǂ̃R���W�����f�[�^

};