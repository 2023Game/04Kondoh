#pragma once
#include "CModel.h"
#include "CColliderMesh.h"

class CInteractObject;

class CLDoubleDoor : public CDoubleDoors
{
public:
	// コンストラクタ
	CLDoubleDoor(CVector pos, CVector angle, CVector size);
	// デストラクタ
	~CLDoubleDoor();

	// 接続するスイッチを追加
	void AddInputObjs(CInteractObject* sw);
	// 扉の開閉した時の各座標を設定
	void SetAnimPos(const CVector& openPos, const CVector& closePos);
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	//// スイッチを押して扉が開くかどうか
	//bool IsSwitchOn() const;

	CVector mOpenPos;	// 開いた時の位置
	CVector mClosePos;	// 閉まった時の位置

	float mAnimTime;
	float mElapsedTime;

	bool mIsOpened;
	bool mIsPlaying;

	// 入力系のオブジェクトのリスト
	std::list<CInteractObject*> mpInputObjs;

	CModel* mpLDoorModel;		// レフトドアのモデル
	CColliderMesh* mpLDoorCol;	// レフトドアのコライダー

};
