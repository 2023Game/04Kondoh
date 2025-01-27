#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CWall.h"
#include "CPlayer.h"
#include "CEnemyA.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
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

	CResourceManager::Load<CModel>  ("Field",         "Field\\Stage1.obj");
	CResourceManager::Load<CModel>  ("FieldCol",      "Field\\StageCol.obj");
	CResourceManager::Load<CModel>  ("FieldCube",     "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>  ("FieldCylinder", "Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModel>  ("Wall",          "Field\\Object\\Wall\\Wall.obj");
	CResourceManager::Load<CModel>  ("WallCol",       "Field\\Object\\Wall\\WallCol.obj");
	CResourceManager::Load<CModel>  ("WallNavCol",    "Field\\Object\\Wall\\WallNavCol.obj");
	CResourceManager::Load<CModelX> ("Player",        "Character\\New Player\\Playre T-Pose.x");
	CResourceManager::Load<CModelX> ("EnemyA",        "Character\\EnemyA\\enemyA.x");
	CResourceManager::Load<CTexture>("Laser",         "Effect\\laser.png");
	CResourceManager::Load<CTexture>("LightningBolt", "Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>  ("Slash",         "Effect\\slash.obj");
	CResourceManager::Load<CSound>  ("SlashSound",    "Sound\\SE\\slash.wav");

	// �Q�[��BGM��ǂݍ���
	//CBGMManager::Instance()->Play(EBGMType::eGame);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();

	CField* field = new CField();

	CPlayer* player = new CPlayer();
	player->Scale(1.0f, 1.0f, 1.0f);
	player->Position(-50.0f, 0.94, 0.0f);
	player->Rotation(0.0f, -90.0f, 0.0f);

	CEnemyA* enemyA = new CEnemyA
	(
		{
			CVector(100.0f,0.94f,  0.0f),
			CVector(  0.0f,0.94f,  0.0f),
			CVector(  0.0f,0.94f,100.0f),
			CVector(100.0f,0.94f,100.0f),
		}
	);
	enemyA->Scale(1.0f, 1.0f, 1.0f);
	enemyA->Position(100.0f, 0.0f, 0.0f);

	// CGameCamera�̃e�X�g
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2�̃e�X�g
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);

	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);
	mainCamera->AddCollider(field->GetFieldCol());
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
	// BGM�Đ����łȂ���΁ABGM���Đ�
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

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
}
