#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// ステージの壁表示クラス
class CFieldWall : public CObjectBase
{
public:
	// ステージ1の壁のインスタンス
	static CFieldWall* Instance();
	// コンストラクタ
	CFieldWall();
	// デストラクタ
	~CFieldWall();

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

	// コライダ取得処理
	CCollider* GetNavCol()const;

private:
	// ステージの壁のインスタンス
	static CFieldWall* spInstance;

	// ステージ1の壁のモデルデータ
	CModel* mpModel;
	// ステージ1の壁のコリジョンデータ
	CColliderMesh* mpColliderMesh;
};