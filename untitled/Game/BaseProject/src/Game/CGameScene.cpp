#include "CGameScene.h"
#include "CSceneManager.h"
#include "CGameMenu.h"
#include "CGameClear.h"
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

#define GAMEOVER_WAIT_TIME 0.5f // ゲームオーバーシーン移行待機時間
#define RESPAWN_INTERVAL 2.0f	// リスポーンまでの間隔
#define MAX_RESPAWN 1	// 最大リスポーン数
#define RESPAWN 1

// 敵のポイントデータのテーブル
const std::vector<RespawnData> RESPAWN_DATA =
{
	{ 
		CVector(140.0f, 0.94f, 0.0f),
		{
			CVector( 110.0f, 0.94f,    0.0f),
			CVector(   0.0f, 0.94f, -110.0f),
			CVector( 110.0f, 0.94f,  110.0f),
			CVector(-110.0f, 0.94f, -110.0f),
		}
	},
	{
		CVector(-140.0f, 0.94f, 0.0f),
		{
			CVector( 110.0f, 0.94f,    0.0f),
			CVector(   0.0f, 0.94f, -110.0f),
			CVector( 110.0f, 0.94f,  110.0f),
			CVector(-110.0f, 0.94f, -110.0f),
		}
	},
	{
		CVector(0.0f, 0.94f, 140.0f),
		{
			CVector(110.0f, 0.94f,    0.0f),
			CVector(0.0f, 0.94f, -110.0f),
			CVector(110.0f, 0.94f,  110.0f),
			CVector(-110.0f, 0.94f, -110.0f),
		}
	},
	{ 
		CVector(0.0f, 0.94f, -140.0f),
		{
			CVector( 110.0f, 0.94f,    0.0f),
			CVector(  0.0f,  0.94f, -110.0f),
			CVector( 110.0f, 0.94f,  110.0f),
			CVector(-110.0f, 0.94f, -110.0f),
		}
	},
	//{ CVector(0.0f, 0.94f, -140.0f),	false },
	//{ CVector(110.0f, 0.94f, 110.0f),	false },
	//{ CVector(110.0f, 0.94f, -110.0f),	false },
	//{ CVector(-110.0f, 0.94f, 110.0f),	false },
	//{ CVector(-110.0f, 0.94f, -110.0f),	false },
};

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
	, mpGameClear(nullptr)
	, mElapsedTime(0.0f)
	, mSpawnCount(0)
	, mRespawnElapsedTime(0.0f)
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
	CResourceManager::Load<CModel>("Field",			"Field\\FieldFloor.obj");
	CResourceManager::Load<CModel>("FieldWall",		"Field\\Object\\Wall\\FieldWall.obj");
	CResourceManager::Load<CModel>("FieldWallCol",	"Field\\Object\\Wall\\Demo_FieldWallCol.obj");
	CResourceManager::Load<CModel>("BackGround",	"Field\\BackGround.obj");
	// オブジェクト
	CResourceManager::Load<CModel>("Wall",			"Field\\Object\\Wall\\Wall.obj");
	CResourceManager::Load<CModel>("WallCol",		"Field\\Object\\Wall\\WallCol.obj");
	CResourceManager::Load<CModel>("WallNavCol",	"Field\\Object\\Wall\\WallNavCol.obj");
	CResourceManager::Load<CModel>("MoveFloor1",	"Field\\Object\\MoveObject.obj");
	CResourceManager::Load<CModel>("MoveFloor2",	"Field\\Object\\MoveObject_2.obj");
	CResourceManager::Load<CModel>("Staircase",		"Field\\Object\\Gimmick\\SpiralStaircase.obj");
	// ギミック
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
	//CBGMManager::Instance()->Play(EBGMType::eGame);

	// 経路探索管理クラスを作成
	new CNavManager();

	CFieldWall* fieldWall = new CFieldWall();
	CField* field = new CField();

	CPlayer* player = new CPlayer();
	player->Scale(1.0f, 1.0f, 1.0f);
	player->Position(-334.0f, 1.0f, -4.0f);
	player->Rotation(0.0f, -90.0f, 0.0f);

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
	mpGameClear = new CGameClear();
}

//シーンの更新処理
void CGameScene::Update()
{
	CPlayer* player = CPlayer::Instance();
	CEnemyManager* enemies = CEnemyManager::Instance();

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

	if (mSpawnCount == MAX_RESPAWN && enemies->GetEnemies().size() == 0)
	{
		if (!mpGameClear->IsOpened())
		{
			mpGameClear->Open();
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

	RandomRespawn();

}

// ランダムリスポーン
void CGameScene::RandomRespawn()
{
	if (mSpawnCount >= MAX_RESPAWN) return;

	CEnemyManager* enemyMgr = CEnemyManager::Instance();

	if (enemyMgr->GetEnemies().size() < RESPAWN)
	{
		if (mRespawnElapsedTime < RESPAWN_INTERVAL)
		{
			mRespawnElapsedTime += Times::DeltaTime();
		}
		else
		{
			int index = Math::Rand(0, RESPAWN_DATA.size() - 1);
			const RespawnData& data = RESPAWN_DATA[index];
			CEnemyA* enemyA = new CEnemyA(data.respawnPoint, data.patrolPoints);
			enemyA->Scale(1.0f, 1.0f, 1.0f);
			mSpawnCount++;
			mRespawnElapsedTime -= RESPAWN_INTERVAL;
		}
	}
}

//CVector CGameScene::RandomPoint()
//{
//	int maxSize = POINT_DATA.size();
//	int minSize = maxSize - (maxSize - 1);
//	int randPoint = Math::Rand(minSize, maxSize);
//
//	//const CEnemyBase::PointData& data = (CEnemyBase::mpPointData
//
//	//return CVector();
//}
