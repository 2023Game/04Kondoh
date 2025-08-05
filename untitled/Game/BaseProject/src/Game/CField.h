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

	// コライダ取得処理
	CColliderMesh* GetFieldCol()const;
	// 生成した壁のリストを取得
	std::list<CWall*> GetWalls() const;

private:
	// 壁を生成
	void CreateWalls();
	// フィールドオブジェクトを作成
	void CreateFieldObjects();
	// ギミックを作成
	void CreateGimmick();
	// 経路探索用のノードを作成
	void CreateNavNodes();
	 
	// フィールドのインスタンス
	static CField* spInstance;

	// ステージ1の床のモデル
	CModel* mpFieldModel;
	CModel* mpBackGroundModel;
	// ステージ1の床のコリジョン
	CColliderMesh* mpFieldCol;

	// フィールドオブジェクトのモデル
	CModel* mpMoveFloor1Model;	// 螺旋階段上の動く床
	CModel* mpMoveFloor2Model;	// 動く床
	CModel* mpStaircaseModel;	// 螺旋階段

	TexAnimData mEffectAnimData;

	// 壁オブジェクトのリスト
	std::list<CWall*> mWalls;
};