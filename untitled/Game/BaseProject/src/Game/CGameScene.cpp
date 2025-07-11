#include "CGameScene.h"
#include "CSceneManager.h"
#include "CGameMenu.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"

#include "CField.h"
#include "CFieldWall.h"
#include "CWall.h"
#include "CPlayer.h"
#include "CEnemyManager.h"
#include "CEnemyA.h"
#include "CBGMManager.h"

#define GAMEOVER_WAIT_TIME 0.5f //　ゲームオーバーシーン移行待機時間

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
	, mElapsedTime(0.0f)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
}

//シーン読み込み
void CGameScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う

	// ステージ
	CResourceManager::Load<CModel>("Field",			"Field\\Demo_FieldFloor.obj");
	CResourceManager::Load<CModel>("FieldCol",		"Field\\Stage1_FieldFloorCol.obj");
	CResourceManager::Load<CModel>("FieldWall",		"Field\\Object\\Wall\\Demo_FieldWall.obj");
	CResourceManager::Load<CModel>("FieldWallCol",	"Field\\Object\\Wall\\Demo_FieldWallCol.obj");
	// オブジェクト
	CResourceManager::Load<CModel>("Wall",			"Field\\Object\\Wall\\Wall.obj");
	CResourceManager::Load<CModel>("WallCol",		"Field\\Object\\Wall\\WallCol.obj");
	CResourceManager::Load<CModel>("WallNavCol",	"Field\\Object\\Wall\\WallNavCol.obj");
	// ギミック
	CResourceManager::Load<CModel>("MoveFloor1",	"Field\\Object\\MoveObject.obj");
	CResourceManager::Load<CModel>("MoveFloor2",	"Field\\Object\\MoveObject_2.obj");
	CResourceManager::Load<CModel>("LeverBase",		"Field\\Object\\Gimmick\\LeverBase.obj");
	CResourceManager::Load<CModel>("Lever",			"Field\\Object\\Gimmick\\Lever.obj");
	CResourceManager::Load<CModel>("SingleDoor",	"Field\\Object\\Gimmick\\SingleDoor.obj");
	CResourceManager::Load<CModel>("RDuobleDoors",	"Field\\Object\\Gimmick\\RDoubleDoors.obj");
	CResourceManager::Load<CModel>("LDuobleDoors",	"Field\\Object\\Gimmick\\LDoubleDoors.obj");
	CResourceManager::Load<CModel>("RastDoor",		"Field\\Object\\Gimmick\\RastDoor.obj");
	// キャラクター
	CResourceManager::Load<CModelX>("Player",		"Character\\Player\\T_Pose.x");
	CResourceManager::Load<CModelX>("EnemyA",		"Character\\EnemyA\\enemyA.x");
	// サウンド
	CResourceManager::Load<CSound>("SlashSound",	"Sound\\SE\\slash.wav");

	//CResourceManager::Load<CTexture>("Laser",         "Effect\\laser.png");
	//CResourceManager::Load<CTexture>("LightningBolt", "Effect\\lightning_bolt.png");
	//CResourceManager::Load<CModel>  ("Slash",         "Effect\\slash.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// 経路探索管理クラスを作成
	new CNavManager();

	CFieldWall* fieldWall = new CFieldWall();
	CField* field = new CField();

	CPlayer* player = new CPlayer();
	player->Scale(1.0f, 1.0f, 1.0f);
	player->Position(10.0f, 1.0, 0.0f);
	player->Rotation(0.0f, -90.0f, 0.0f);

	CEnemyA* enemyA = new CEnemyA
	(
		{
			CVector(100.0f, 0.94f,   0.0f),
			CVector(  0.0f, 0.94f,   0.0f),
			CVector(  0.0f, 0.94f, 100.0f),
			CVector(100.0f, 0.94f, 100.0f),
			CVector( 50.0f, 2.0f,   50.0f),
		}
	);
	enemyA->Scale(1.0f, 1.0f, 1.0f);
	enemyA->Position(100.0f, 0.0f, 0.0f);

	/*CEnemyA* enemyA2 = new CEnemyA
	(
		{
			CVector(250.0f, 0.94f, 150.0f),
			CVector(150.0f, 0.94f, 150.0f),
			CVector(150.0f, 0.94f, 250.0f),
			CVector(250.0f, 0.94f, 250.0f),
		}
	);*/

	// CGameCamera2のテスト
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

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CGameScene::Update()
{
	CPlayer* player = CPlayer::Instance();
	//BGM再生中でなければ、BGMを再生
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

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}
}
