#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CFieldWall.h"
#include "CWall.h"
#include "CLever.h"
#include "CSingleDoor.h"
#include "CDoubleDoors.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include <assert.h>

// フィールドのインスタンス
CField* CField::spInstance = nullptr;

CField* CField::Instance()
{
	return spInstance;
}

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	assert(spInstance == nullptr);
	spInstance = this;

	// 床のモデル
	mpFieldModel = CResourceManager::Get<CModel>("Field");
	// 床のコライダー設定
	mpFieldCol = new CColliderMesh
	(
		this, ELayer::eField, mpFieldModel,
		true
	);
	//mpFieldCol->SetShow(true);

	// 背景のモデル
	mpBackGroundModel = CResourceManager::Get<CModel>("BackGround");

	// インスタンス取得
	CNavManager* navManager = CNavManager::Instance();
	CFieldWall* fieldWall = CFieldWall::Instance();
	// 遮蔽物になるコライダーに追加する
	navManager->AddCollider(mpFieldCol);
	navManager->AddCollider(fieldWall->GetFieldWallCol());

	// 壁を作成
	//CreateWalls();

	for (CWall* wall : mWalls)
	{
		navManager->AddCollider(wall->GetNavCol());
	}

	// フィールドオブジェクトを作成
	//CreateFieldObjects();
	// ギミックオブジェクトを作成
	//CreateGimmick();

	// 経路探索用のノードを作成
	CreateNavNodes();
}

CField::~CField()
{
	spInstance = nullptr;
	if (mpFieldCol != nullptr)
	{
		delete mpFieldCol;
		mpFieldCol = nullptr;
	}
}

// 壁を生成
void CField::CreateWalls()
{
	// 壁１作成
	CWall* wall = new CWall
	(
		CVector(-369.0f, 0.0f, 0.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(7.0f, 8.0f, 6.0f)

	);
	mWalls.push_back(wall);

	// 壁２生成
	wall = new CWall
	(
		CVector(-50.0f, 0.94f, -50.0f),
		CVector(  0.0f,  0.0f,  0.0f),
		CVector(  5.0f,  5.0f,  5.0f)
	);
	mWalls.push_back(wall);
}


void CField::CreateFieldObjects()
{

	mpMoveFloor1Model = CResourceManager::Get<CModel>("MoveFloor1");
	mpMoveFloor2Model = CResourceManager::Get<CModel>("MoveFloor2");
	mpStaircaseModel = CResourceManager::Get<CModel>("Staircase");

	// 螺旋階段
	new CMoveFloor
	(
		mpStaircaseModel,
		CVector(0.0f, 5.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 0.0f, 0.0f), 1.0f
	);

	// 螺旋階段の上の動く床
	new CMoveFloor
	(
		mpMoveFloor1Model,
		CVector(0.0f, 40.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 40.0f, 0.0f), 10.0f
	);

	// 動く床１
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(0.0f, 25.0f, 0.0f), 
		CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 25.0f, 0.0f), 7.0f
	);
	// 動く床２
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(50.0f, 75.0f, 15.0f),
		CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 25.0f, 0.0f), 9.0f
	);
	// 動く床３
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(100.0f, 125.0f, 30.0f),
		CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 25.0f, 0.0f), 6.0f
	);

	// 
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(0.0f, 16.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 0.0f, 0.0f), 6.0f
	);

	/*
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 50.0f, 0.0f), 5.0f
	);
	// 動かない床③（坂道）
	CMoveFloor* mf = new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 20.0f, 200.5f), CVector(4.0f, 1.0f, 2.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	mf->Rotate(0.0f, 0.0f, 30.0f);

	// 電撃エフェクト
	CLineEffect* le = new CLineEffect(ETag::eField);
	le->SetTexture("LightningBolt");
	le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	le->SetAnimData(&mEffectAnimData);

	CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	int div = 3;
	float width = 5.0f;
	le->AddPoint(startPos, width, width);
	for (int i = 0; i < div; i++)
	{
		float alpha = (float)(i + 1) / div;
		CVector pos = CVector::Lerp(startPos, endPos, alpha);
		le->AddPoint(pos, width, width);
	}
	*/
}

void CField::CreateGimmick()
{
	// １番目のレバーとドア									// CVector(90.0f, 270.0f, 90.0f)
	CLever* lever1 = new CLever(CVector(-185.0f, 14.0f, -1.0f), CVector(0.0f, 0.0f, -90.0f));
	CSingleDoor* door1 = new CSingleDoor
	(
		CVector(-290.0f, 0.0f, 98.0f),
		CVector(0.0f, 72.0f, 0.0f),
		CVector(1.2f, 1.0f, 1.2f)
	);
	door1->SetAnimPos
	(
		CVector(-341.0f, 0.0f, 111.0f),
		CVector(-304.0f, 0.0f, 99.0f)
	);


	// ２番目のレバーとドア
	// 位置(-270.0f, 175.0f, 90.0f)、角度(90.0f, 163.0f, 0.0f)
	CLever* lever2 = new CLever(CVector(-270.0f, 175.0f, 90.0f), CVector(90.0f, 163.0f, 0.0f));
	CDoubleDoors* door2 = new CDoubleDoors
	(
		CVector(0.0f, 0.0f, 315.0f), CVector(0.0f, 0.0f, 262.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 0.8f, 0.7f)
	);
	door2->SetAnimPosL
	(
		CVector(0.0f, 0.0f, 345.0f),
		CVector(0.0f, 0.0f, 315.0f)
	);
	door2->SetAnimPosR
	(
		CVector(0.0f, 0.0f, 226.0f),
		CVector(0.0f, 0.0f, 262.0f)
	);

	// ３番目のレバーとドア
	CLever* lever3 = new CLever(CVector(105.0f, 15.0f, 344.0f), CVector(90.0f, 335.0f, 0.0f));
	CSingleDoor* door3 = new CSingleDoor
	(
		CVector(0.0f, 154.0f, 307.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door3->SetAnimPos
	(
		CVector(0.0f, 154.0f, 359.0f),
		CVector(8.0f, 154.0f, 310.0f)
	);

	// ４番目のレバーとドア
	CLever* lever4 = new CLever(CVector(105.0f, 162.0f, 344.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door4 = new CSingleDoor
	(
		CVector(154.0f, 0.0f, 209.0f),
		CVector(0.0f, 143.7f, 0.0f),
		CVector(0.9f, 1.0f, 0.9f)
	);
	door4->SetAnimPos
	(
		CVector(129.5f, 0.0f, 178.5f),
		CVector(153.0f, 0.0f, 209.0f)
	);

	// ５番目のレバーとドア
	CLever* lever5 = new CLever(CVector(158.0f, 14.0f, 103.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door5 = new CSingleDoor
	(
		CVector(152.0f, 154.0f, 208.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door5->SetAnimPos
	(
		CVector(152.0f, 154.0f, 208.0f),
		CVector(152.0f, 154.0f, 208.0f)
	);

	// ６番目のレバーとドア
	CLever* lever6 = new CLever(CVector(270.0f, 162.0f, 94.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door6 = new CSingleDoor
	(
		CVector(295.0f, 0.0f, 85.0f),
		CVector(0.0f, 105.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door6->SetAnimPos
	(
		CVector(291.0f, 0.0f, 91.0f),
		CVector(291.0f, 0.0f, 91.0f)
	);

	// ７番目のレバーとドア
	//CLever* lever7 = new CLever(CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f));
	//CDoubleDoors* door7 = new CDoubleDoors
	//(
	//	CVector(286.0f, 154.0f, -92.0f), CVector(248.0f, 154.0f, -81.0f),
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(1.0f, 1.0f, 1.0f)
	//);
	//door7->SetAnimPosL
	//(
	//	CVector(286.0f, 154.0f, -92.0f),
	//	CVector(286.0f, 154.0f, -92.0f)
	//);
	//door7->SetAnimPosR
	//(
	//	CVector(248.0f, 154.0f, -81.0f),
	//	CVector(248.0f, 154.0f, -81.0f)
	//);

	// ８番目のレバーとドア
	CLever* lever8 = new CLever(CVector(290.0f, 15.0f, -215.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door8 = new CSingleDoor
	(
		CVector(0.5f, 0.0f, -309.0f),
		CVector(0.0f, 179.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door8->SetAnimPos
	(
		CVector(-0.7f, 0.0f, -359.0f),
		CVector(-0.5f, 0.0f, -309.0f)
	);

	// ９番目のレバーとドア
	CLever* lever9 = new CLever(CVector(-102.0f, 162.0f, -346.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door9 = new CSingleDoor
	(
		CVector(-174.0f, 0.0f, -240.0f),
		CVector(0.0f, 144.5f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door9->SetAnimPos
	(
		CVector(-174.0f, 0.0f, -240.0f),
		CVector(-174.0f, 0.0f, -240.0f)
	);
	
	// １０番目のレバーとドア
	CLever* lever10 = new CLever(CVector(-293.0f, 15.0f, -214.0f), CVector(90.0f, 0.0f, 0.0f));
	CDoubleDoors* door10 = new CDoubleDoors
	(
		CVector(-150.0f, 154.0f, -206.0f), CVector(-180.0f, 154.0f, -247.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door10->SetAnimPosL
	(
		CVector(-150.0f, 154.0f, -206.0f),
		CVector(-150.0f, 154.0f, -206.0f)
	);
	door10->SetAnimPosR
	(
		CVector(-180.0f, 154.0f, -247.0f),
		CVector(-180.0f, 154.0f, -247.0f)
	);

	// １１番目のレバーのドア
	CLever* lever11 = new CLever(CVector(-293.0f, 162.0f, -214.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door11 = new CSingleDoor
	(
		CVector(-131.0f, 0.0f, -123.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door11->SetAnimPos
	(
		CVector(-131.0f, 0.0f, -123.0f),
		CVector(-131.0f, 0.0f, -123.0f)
	);

	// 各レバーを各ドアに対応させる
	door1->AddInputObjs(lever1);
	door2->AddInputObjs(lever2);
	door3->AddInputObjs(lever3);
	door4->AddInputObjs(lever4);
	door5->AddInputObjs(lever5);
	door6->AddInputObjs(lever6);
	//door7->AddInputObjs(lever7);
	door8->AddInputObjs(lever8);
	door9->AddInputObjs(lever9);
	door10->AddInputObjs(lever10);
	door11->AddInputObjs(lever11);


#if _DEBUG
	lever1->SetDebugName("Lever1");
	lever2->SetDebugName("Lever2");
	lever3->SetDebugName("Lever3");
	lever4->SetDebugName("Lever4");
	lever5->SetDebugName("Lever5");
	lever6->SetDebugName("Lever6");
	//lever7->SetDebugName("Lever7");
	lever8->SetDebugName("Lever8");
	lever9->SetDebugName("Lever9");
	lever10->SetDebugName("Lever10");
	lever11->SetDebugName("Lever11");
#endif

}


// 経路探索用のノードを作成
void CField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		//// 壁１の周りの経路探索
		//new CNavNode(CVector(30.0f, 0.0f,  35.0f));
		//new CNavNode(CVector(10.0f, 0.0f,  35.0f));
		//new CNavNode(CVector(10.0f, 0.0f, -35.0f));
		//new CNavNode(CVector(30.0f, 0.0f, -35.0f));

		//// 壁２の周りの経路探索
		//new CNavNode(CVector(-15.0f, 0.0f, -40.0f));
		//new CNavNode(CVector(-15.0f, 0.0f, -60.0f));
		//new CNavNode(CVector(-85.0f, 0.0f, -60.0f));
		//new CNavNode(CVector(-85.0f, 0.0f, -40.0f));

		new CNavNode(CVector(105.0f, 0.0f, -5.0f));
		new CNavNode(CVector( 60.0f, 0.0f, -5.0f));

	}

}

// レイとのフィールドオブジェクト衝突判定
bool CField::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかのフラグ
	bool isHit = false;

	CFieldWall* fieldWall = CFieldWall::Instance();

	//　フィールドのオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpFieldCol, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}

	if (fieldWall->NavCollisionRay(start, end, &tHit))
	{
		// まだ他に衝突していない場合か
			// すでに衝突しているコライダーより近い場合は
		if (!isHit || tHit.dist < hit->dist)
		{
			// 衝突情報を更新
			*hit = tHit;
			isHit = true;
		}
	}

	// 壁との衝突判定
	for (CWall* wall : mWalls)
	{
		if (wall->CollisionRay(start, end, &tHit))
		{
			// まだ他に衝突していない場合か
			// すでに衝突しているコライダーより近い場合は
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}

	return isHit;
}

// レイとのフィールドオブジェクト衝突判定（経路探索用）
bool CField::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかのフラグ
	bool isHit = false;

	CFieldWall* fieldWall = CFieldWall::Instance();

	//　フィールドのオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpFieldCol, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}

	if (fieldWall->NavCollisionRay(start, end, &tHit))
	{
		// まだ他に衝突していない場合か
			// すでに衝突しているコライダーより近い場合は
		if (!isHit || tHit.dist < hit->dist)
		{
			// 衝突情報を更新
			*hit = tHit;
			isHit = true;
		}
	}

	// 壁との衝突判定
	for (CWall* wall : mWalls)
	{
		if (wall->NavCollisionRay(start, end, &tHit))
		{
			// まだ他に衝突していない場合か
			// すでに衝突しているコライダーより近い場合は
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}

	return isHit;
}

void CField::Update()
{
}

void CField::Render()
{
	mpFieldModel->Render(Matrix());
	mpBackGroundModel->Render(Matrix());
}


CColliderMesh* CField::GetFieldCol() const
{
	return mpFieldCol;
}


std::list<CWall*> CField::GetWalls() const
{
	return mWalls;
}
