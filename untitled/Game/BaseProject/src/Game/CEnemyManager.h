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

	const std::list<CEnemyBase*>& GetEnemies();


private:
	// �R���X�g���N�^
	CEnemyManager();
	// �f�X�g���N�^
	~CEnemyManager();

	// �C���X�^���X�̃|�C���^
	static CEnemyManager* spInstance;
	// �G�̃��X�g
	std::list<CEnemyBase*> mEnemies;
	// ���ݏo�����Ă���G�̃��X�g
	const std::list<CEnemyBase*> mCurreEnemies;

};
