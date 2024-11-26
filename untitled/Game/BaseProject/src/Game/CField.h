#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CWall;

class CField : public CObjectBase
{
public:
	// フィールドのインスタンスを返す
	static CField* Instance();
	CField();
	~CField();

	/// <summary>
	/// レイとのフィールドオブジェクト衝突判定
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <returns>衝突していたら、trueを返す</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	/// <summary>
	/// レイとのフィールドオブジェクト衝突判定（経路探索用）
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <returns>衝突していたら、trueを返す</returns>
	bool NavCollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit);

	void Update();
	void Render();

	// カメラとの当たり判定取得処理
	CCollider* GetFieldCol()const;

private:
	// 壁を生成
	void CreateWalls();
	void CreateFieldObjects();
	// 経路探索用のノードを作成
	void CreateNavNodes();
	 
	// フィールドのインスタンス
	static CField* spInstance;

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// 壁オブジェクトのリスト
	std::list<CWall*> mWalls;
};