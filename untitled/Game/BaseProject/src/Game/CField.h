#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CWall;

class CField : public CObjectBase
{
public:
	// �t�B�[���h�̃C���X�^���X��Ԃ�
	static CField* Instance();
	CField();
	~CField();

	/// <summary>
	/// ���C�Ƃ̃t�B�[���h�I�u�W�F�N�g�Փ˔���
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <returns>�Փ˂��Ă�����Atrue��Ԃ�</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	/// <summary>
	/// ���C�Ƃ̃t�B�[���h�I�u�W�F�N�g�Փ˔���i�o�H�T���p�j
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <returns>�Փ˂��Ă�����Atrue��Ԃ�</returns>
	bool NavCollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit);

	void Update();
	void Render();

	// �R���C�_�擾����
	CColliderMesh* GetFieldCol()const;
	// ���������ǂ̃��X�g���擾
	std::list<CWall*> GetWalls() const;

private:
	// �ǂ𐶐�
	void CreateWalls();
	// �t�B�[���h�I�u�W�F�N�g���쐬
	void CreateFieldObjects();
	// �M�~�b�N���쐬
	void CreateGimmick();
	// �o�H�T���p�̃m�[�h���쐬
	void CreateNavNodes();
	 
	// �t�B�[���h�̃C���X�^���X
	static CField* spInstance;

	// �X�e�[�W1�̏��̃��f��
	CModel* mpFieldModel;
	CModel* mpBackGroundModel;
	// �X�e�[�W1�̏��̃R���W����
	CColliderMesh* mpFieldCol;

	// �t�B�[���h�I�u�W�F�N�g�̃��f��
	CModel* mpMoveFloor1Model;	// �����K�i��̓�����
	CModel* mpMoveFloor2Model;	// ������
	CModel* mpStaircaseModel;	// �����K�i

	TexAnimData mEffectAnimData;

	// �ǃI�u�W�F�N�g�̃��X�g
	std::list<CWall*> mWalls;
};