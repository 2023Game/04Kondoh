#include "CFieldWall.h"
#include <assert.h>
#include "CNavManager.h"
#include "CNavNode.h"

CFieldWall* CFieldWall::spInstance = nullptr;

CFieldWall* CFieldWall::Instance()
{
	return spInstance;
}

// コンストラクタ
CFieldWall::CFieldWall()
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
{
	assert(spInstance == nullptr);
	spInstance = this;

	// ステージ1の壁のモデルデータ取得
	mpModel = CResourceManager::Get<CModel>("FieldWall");
	// ステージ壁のコライダを作成
	CModel* fieldWallColMesh = CResourceManager::Get<CModel>("FieldWallCol");

	mpColliderMesh = new CColliderMesh
	(
		this, ELayer::eWall, fieldWallColMesh,
		true, 1.0f,
		8, 2, 8
	);
	//mpColliderMesh->SetShow(true);
}

// デストラクタ
CFieldWall::~CFieldWall()
{
	spInstance = nullptr;
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

// レイとの壁との衝突判定（経路探索用）
bool CFieldWall::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 壁のコライダーが存在しなければ、衝突していない
	if (mpColliderMesh == nullptr) return false;
	return CCollider::CollisionRay(mpColliderMesh, start, end, hit);
}


// 更新
void CFieldWall::Update()
{
}

// 描画
void CFieldWall::Render()
{
	mpModel->Render(Matrix());
}

CCollider* CFieldWall::GetFieldWallCol() const
{
	return mpColliderMesh;
}
