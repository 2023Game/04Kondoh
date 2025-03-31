#include "CWall.h"

// コンストラクタ
CWall::CWall(const CVector& pos, const CVector& angle, const CVector& size)
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
{
	// 壁のモデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Wall");

	// 壁のコライダーを作成
	CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// 壁の経路探索用のコライダーを作成
	CModel* navColModel = CResourceManager::Get<CModel>("WallNavCol");
	mpNavColliderMesh = new CColliderMesh(this, ELayer::eWall, navColModel, true);

	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// デストラクタ
CWall::~CWall()
{
	// コライダーを削除
	SAFE_DELETE(mpColliderMesh);
	SAFE_DELETE(mpNavColliderMesh);
}

// レイとの壁との衝突判定
bool CWall::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 壁のコライダーが存在しなければ、衝突していない
	if (mpColliderMesh == nullptr) return false;
	return CCollider::CollisionRay(mpColliderMesh, start, end, hit);
}

// レイとの壁との衝突判定（経路探索用）
bool CWall::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 壁のコライダーが存在しなければ、衝突していない
	if (mpNavColliderMesh == nullptr) return false;
	return CCollider::CollisionRay(mpNavColliderMesh, start, end, hit);
}
// 更新
void CWall::Update()
{
}

// 描画
void CWall::Render()
{
	mpModel->Render(Matrix());
}

CCollider* CWall::GetFieldCol() const
{
	return mpColliderMesh;
}
