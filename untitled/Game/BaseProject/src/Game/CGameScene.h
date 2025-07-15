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
	void RandomRespawn(CEnemyManager* enemy);
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	float mElapsedTime; // �o�ߎ��Ԍv���p

	int mSpawnCount;	// �G�̃��X�|�[����

	CSound* mpGameBGM;
	CGameMenu* mpGameMenu;
};
