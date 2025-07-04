#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CFieldWall.h"
#include "CWall.h"
#include "CLever.h"
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

	mpModel = CResourceManager::Get<CModel>("Field");
	// コライダー設定
	mpColliderMesh = new CColliderMesh
	(
		this, ELayer::eField, mpModel,
		true
	);
	mpColliderMesh->SetShow(true);

	CNavManager* navManager = CNavManager::Instance();
	CFieldWall* fieldWall = CFieldWall::Instance();

	navManager->AddCollider(mpColliderMesh);
	navManager->AddCollider(fieldWall->GetFieldWallCol());


	CLever* lever1 = new CLever(CVector(0.0f, 5.0f, 0.0f));

#if _DEBUG
	lever1->SetDebugName("Lever1");
#endif

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
}

CField::~CField()
{
	spInstance = nullptr;
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
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
	new CRotateFloor
	(
		mpCylinderModel,
		CVector(-40.0f, 15.0f, 20.0f), CVector(1.0f, 1.0f, 1.0f),
		1.0f
	);

	// 動かない床①
	new CMoveFloor
	(
		mpCubeModel,
		CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 動く床①
	new CMoveFloor
	(
		mpCubeModel,
		CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(20.0f, 0.0f, 0.0f), 5.0f
	);
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 回転する床①
	new CRotateFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
		0.5f
	);
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
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
	if (CCollider::CollisionRay(mpColliderMesh, start, end, &tHit))
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
	if (CCollider::CollisionRay(mpColliderMesh, start, end, &tHit))
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
	mpModel->Render(Matrix());
}


CColliderMesh* CField::GetFieldCol() const
{
	return mpColliderMesh;
}


std::list<CWall*> CField::GetWalls() const
{
	return mWalls;
}
