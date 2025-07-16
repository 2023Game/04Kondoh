#include "CGameScene.h"
#include "CSceneManager.h"
#include "CGameMenu.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "Maths.h"

#include "CField.h"
#include "CFieldWall.h"
#include "CWall.h"
#include "CPlayer.h"
#include "CEnemyManager.h"
#include "CEnemyA.h"
#include "CBGMManager.h"

#define GAMEOVER_WAIT_TIME 0.5f // �Q�[���I�[�o�[�V�[���ڍs�ҋ@����
#define RESPAWN_INTERVAL 2.0f	// ���X�|�[���܂ł̊Ԋu

// �G�̃|�C���g�f�[�^�̃e�[�u��
const std::vector<CEnemyBase::PointData> POINT_DATA =
{
	{ CVector(0.0f, 0.94f, 0.0f),		false },
	{ CVector(140.0f, 0.94f, 0.0f),		false },
	{ CVector(0.0f, 0.94f, 140.0f),		false },
	{ CVector(-140.0f, 0.94f, 0.0f),	false },
	{ CVector(0.0f, 0.94f, -140.0f),	false },
	{ CVector(110.0f, 0.94f, 110.0f),	false },
	{ CVector(110.0f, 0.94f, -110.0f),	false },
	{ CVector(-110.0f, 0.94f, 110.0f),	false },
	{ CVector(-110.0f, 0.94f, -110.0f),	false },
};

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
	, mElapsedTime(0.0f)
	, mSpawnCount(0)
	, mRespawnElapsedTime(0.0f)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
}

//�V�[���ǂݍ���
void CGameScene::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	// �X�e�[�W
	CResourceManager::Load<CModel>("Field",			"Field\\Demo_FieldFloor.obj");
	CResourceManager::Load<CModel>("FieldWall",		"Field\\Object\\Wall\\Demo_FieldWall.obj");
	CResourceManager::Load<CModel>("FieldWallCol",	"Field\\Object\\Wall\\Demo_FieldWallCol.obj");
	CResourceManager::Load<CModel>("BackGround",	"Field\\BackGround.obj");
	// �I�u�W�F�N�g
	CResourceManager::Load<CModel>("Wall",			"Field\\Object\\Wall\\Wall.obj");
	CResourceManager::Load<CModel>("WallCol",		"Field\\Object\\Wall\\WallCol.obj");
	CResourceManager::Load<CModel>("WallNavCol",	"Field\\Object\\Wall\\WallNavCol.obj");
	// �M�~�b�N
	CResourceManager::Load<CModel>("MoveFloor1",	"Field\\Object\\MoveObject.obj");
	CResourceManager::Load<CModel>("MoveFloor2",	"Field\\Object\\MoveObject_2.obj");
	CResourceManager::Load<CModel>("LeverBase",		"Field\\Object\\Gimmick\\LeverBase.obj");
	CResourceManager::Load<CModel>("Lever",			"Field\\Object\\Gimmick\\Lever.obj");
	CResourceManager::Load<CModel>("SingleDoor",	"Field\\Object\\Gimmick\\SingleDoor.obj");
	CResourceManager::Load<CModel>("RDuobleDoors",	"Field\\Object\\Gimmick\\RDoubleDoors.obj");
	CResourceManager::Load<CModel>("LDuobleDoors",	"Field\\Object\\Gimmick\\LDoubleDoors.obj");
	CResourceManager::Load<CModel>("RastDoor",		"Field\\Object\\Gimmick\\RastDoor.obj");
	// �L�����N�^�[
	CResourceManager::Load<CModelX>("Player",		"Character\\Player\\T_Pose.x");
	CResourceManager::Load<CModelX>("EnemyA",		"Character\\EnemyA\\enemyA.x");
	// �T�E���h
	CResourceManager::Load<CSound>("SlashSound",	"Sound\\SE\\slash.wav");

	//CResourceManager::Load<CTexture>("Laser",         "Effect\\laser.png");
	//CResourceManager::Load<CTexture>("LightningBolt", "Effect\\lightning_bolt.png");
	//CResourceManager::Load<CModel>  ("Slash",         "Effect\\slash.obj");

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();

	CFieldWall* fieldWall = new CFieldWall();
	CField* field = new CField();

	CPlayer* player = new CPlayer();
	player->Scale(1.0f, 1.0f, 1.0f);
	player->Position(10.0f, 1.0, 0.0f);
	player->Rotation(0.0f, -90.0f, 0.0f);

	// CGameCamera2�̃e�X�g
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);

	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 25.0f),
		atPos
	);
	mainCamera->AddCollider(field->GetFieldCol());
	mainCamera->AddCollider(fieldWall->GetFieldWallCol());

	std::list<CWall*> walls = field->GetWalls();
	for (CWall* wall : walls)
	{
		mainCamera->AddCollider(wall->GetFieldCol());
	}

	mainCamera->SetFollowTargetTf(player);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
}

//�V�[���̍X�V����
void CGameScene::Update()
{
	CPlayer* player = CPlayer::Instance();
	//BGM�Đ����łȂ���΁ABGM���Đ�
	/*if (!mpGameBGM->IsPlaying())
	{
		mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	}*/

	if (player->IsDeath())
	{
		if (mElapsedTime < GAMEOVER_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			CSceneManager::Instance()->LoadScene(EScene::eGameOver);
		}
	}

	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}

	RandomRespawn();

}

// �����_�����X�|�[��
void CGameScene::RandomRespawn()
{
	// �����̂Ƃ���̓��X�|�[����������������Ă��܂��B

	CEnemyManager* enemyMgr = CEnemyManager::Instance();

	if (enemyMgr->GetEnemies().size() < 2)
	{
		if (mRespawnElapsedTime < RESPAWN_INTERVAL)
		{
			mRespawnElapsedTime += Times::DeltaTime();
		}
		else
		{
			CEnemyA* enemyA = new CEnemyA
			({
					CVector(100.0f, 0.94f,   0.0f),
					CVector(0.0f, 0.94f,   0.0f),
					CVector(0.0f, 0.94f, 100.0f),
					CVector(100.0f, 0.94f, 100.0f),
			});
			enemyA->Scale(1.0f, 1.0f, 1.0f);
			enemyA->Position(100.0f, 0.0f, 0.0f);
			mSpawnCount++;
			mRespawnElapsedTime -= RESPAWN_INTERVAL;
		}
	}
}

CVector CGameScene::RandomPoint()
{
	int maxSize = POINT_DATA.size();
	int minSize = maxSize - (maxSize - 1);
	int randPoint = Math::Rand(minSize, maxSize);

	//const CEnemyBase::PointData& data = (CEnemyBase::mpPointData

	//return CVector();
}
