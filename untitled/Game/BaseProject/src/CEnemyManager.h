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

	CEnemyBase* FindLockOnTarget(float angle, float length) const;

private:
	// �R���X�g���N�^
	CEnemyManager();
	// �f�X�g���N�^
	~CEnemyManager();

	// �C���X�^���X�̃|�C���^
	static CEnemyManager* spInstance;
	// �G�̃��X�g
	std::list<CEnemyBase*> mEnemies;

};
