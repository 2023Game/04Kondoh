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

	// 更新
	void Update();
	// 描画
	void Render();

	// コライダ取得処理
	CCollider* GetFieldCol()const;

private:
	// ステージの壁のインスタンス
	static CFieldWall* spInstance;

	// ステージ1の壁のモデルデータ
	CModel* mpModel;
	// ステージ1の壁のコリジョンデータ
	CColliderMesh* mpColliderMesh;
};