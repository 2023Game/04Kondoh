#pragma once
#include "CSceneBase.h"

class CGameMenu;
class CSound;

//�^�C�g���V�[��
class CGameScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CGameScene();
	//�f�X�g���N�^
	~CGameScene();
	// �����_�����X�|�[��
	void RandomRespawn();
	// �����_����
	CVector RandomPoint();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	float mElapsedTime; // �o�ߎ��Ԍv���p

	int mSpawnCount;	// �G�̃��X�|�[����
	float mRespawnElapsedTime;	//���X�|�[���o�ߎ��Ԍv���p

	std::vector<CEnemyBase*> point;

	CSound* mpGameBGM;
	CGameMenu* mpGameMenu;
};
