#pragma once

class CEnemyBase;

// �G�Ǘ��N���X
class CEnemyManager
{
public:
	// �G�̃C���X�^���X
	static CEnemyManager* Instance();
	// �C���X�^���X�폜
	static void ClearInstance();

	// �G�����X�g�ɒǉ�
	void Add(CEnemyBase* enemy);
	// �G�����X�g����폜
	void Remove(CEnemyBase* enemy);

	void GetPosition();

	//// �v���C���[�ɋ߂��G�����b�N�I������
	//CEnemyBase* FindLockOnTarget(float angle, float length) const;
	//// ���ɋ߂��G��
	//CEnemyBase* NextLockOnTarget();




private:
	// �R���X�g���N�^
	CEnemyManager();
	// �f�X�g���N�^
	~CEnemyManager();
	// 
	bool IsFindTarget() const;
	// �G�̈ʒu
	CVector mpTargetPos;

	// �C���X�^���X�̃|�C���^
	static CEnemyManager* spInstance;
	// �G�̃��X�g
	std::list<CEnemyBase*> mEnemies;

};
