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
	mpFieldCol->SetShow(true);

	// 背景のモデル
	mpBackGroundModel = CResourceManager::Get<CModel>("BackGround");

	// インスタンス取得
	CNavManager* navManager = CNavManager::Instance();
	CFieldWall* fieldWall = CFieldWall::Instance();
	// 遮蔽物になるコライダーに追加する
	navManager->AddCollider(mpFieldCol);
	navManager->AddCollider(fieldWall->GetFieldWallCol());

	// 壁を作成
	CreateWalls();

	for (CWall* wall : mWalls)
	{
		navManager->AddCollider(wall->GetNavCol());
	}

	// 経路探索用のノードを作成
	CreateNavNodes();
	// フィールドオブジェクトを作成
	CreateFieldObjects();

	CreateGimmick();
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
		CVector(20.0f, 0.94f, 0.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(5.0f, 5.0f, 5.0f)

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

	// 
	new CMoveFloor
	(
		mpMoveFloor1Model,
		CVector(0.0f, 30.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 50.0f, 0.0f), 10.0f
	);

	// 
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(0.0f, 25.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f,25.0f, 0.0f), 7.0f
	);
	// 
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(50.0f, 75.0f, 15.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 25.0f, 0.0f), 9.0f
	);
	// 
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(100.0f, 125.0f, 30.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 25.0f, 0.0f), 6.0f
	);

	/*
	// 動かない床�A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 50.0f, 0.0f), 5.0f
	);
	// 動かない床�B（坂道）
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
//	// レバー
	CLever* lever1 = new CLever(CVector(-270.0f, 175.0f, 90.0f), CVector(90.0f, 163.0f, 0.0f));
#if _DEBUG
	lever1->SetDebugName("Lever1");
#endif

	CSingleDoor* singleDoor1 = new CSingleDoor
	(
		CVector(0.0f, 10.0f, 20.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	singleDoor1->SetAnimPos
	(
		CVector(30.0f, 0.0f, 50.0f),
		CVector(30.0f, 0.0f, 0.0f)
	);
	singleDoor1->AddInputObjs(lever1);

	// レバー
	CLever* lever2 = new CLever(CVector(0.0f, 0.0f, 90.0f), CVector(0.0f, 0.0f, 0.0f));
#if _DEBUG
	lever2->SetDebugName("Lever2");
#endif
	// 
	CDoubleDoors* doubleDoors1 = new CDoubleDoors();
	doubleDoors1->Position(-50.0f, 0.0f, 0.0f); 
	doubleDoors1->SetAnimPosL
	(
		CVector(-80.0f, 0.0f, 0.0f),
		CVector(-50.0f, 0.0f, 0.0f)
	);
	doubleDoors1->SetAnimPosR
	(
		CVector(-80.0f, 0.0f, 0.0f),
		CVector(-110.0f, 0.0f, 0.0f)
	);
	doubleDoors1->AddInputObjs(lever2);
}


// 経路探索用のノードを作成
void CField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 壁１の周りの経路探索
		new CNavNode(CVector(30.0f, 0.0f,  35.0f));
		new CNavNode(CVector(10.0f, 0.0f,  35.0f));
		new CNavNode(CVector(10.0f, 0.0f, -35.0f));
		new CNavNode(CVector(30.0f, 0.0f, -35.0f));

		// 壁２の周りの経路探索
		new CNavNode(CVector(-15.0f, 0.0f, -40.0f));
		new CNavNode(CVector(-15.0f, 0.0f, -60.0f));
		new CNavNode(CVector(-85.0f, 0.0f, -60.0f));
		new CNavNode(CVector(-85.0f, 0.0f, -40.0f));

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
