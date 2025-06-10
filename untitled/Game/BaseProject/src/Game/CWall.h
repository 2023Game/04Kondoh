#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// 壁表示クラス
class CWall : public CObjectBase
{
public:
	// コンストラクタ
	CWall(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CWall();

	/// <summary>
	/// レイとの壁との衝突判定
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <returns>衝突していたら、trueを返す</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	/// <summary>
	/// レイとの壁との衝突判定（経路探索用）
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <returns>衝突していたら、trueを返す</returns>
	bool NavCollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit);

	// 更新
	void Update();
	// 描画
	void Render();

	// カメラとの当たり判定取得処理
	CCollider* GetFieldCol()const;

	// 経路探索用当たり判定取得処理
	CCollider* GetNavCol() const;

private:
	CModel* mpModel;		// 壁のモデルデータ
	CColliderMesh* mpColliderMesh;      // 壁のコリジョンデータ
	CColliderMesh* mpNavColliderMesh;   // 壁の経路探索用のコリジョンデータ

};
