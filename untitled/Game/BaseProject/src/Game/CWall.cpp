#include "CWall.h"

// コンストラクタ
CWall::CWall(const CVector& pos, const CVector& angle, const CVector& size)
{
	// 壁のモデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Wall");

	CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, colModel, true);

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
}

// レイとの壁との衝突判定
bool CWall::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 壁のコライダーが存在しなければ、衝突していない
	if (mpColliderMesh == nullptr) return false;
	return CCollider::CollisionRay(mpColliderMesh, start, end, hit);
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
