#pragma once
#include "CSceneBase.h"

class CGameMenu;
class CGameClear;
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
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();



private:
	float mElapsedTime; // �o�ߎ��Ԍv���p

	int mSpawnCount;	// �G�̃��X�|�[����
	float mRespawnElapsedTime;	//���X�|�[���o�ߎ��Ԍv���p

	CSound* mpGameBGM;
	CGameMenu* mpGameMenu;
	CGameClear* mpGameClear;

};
