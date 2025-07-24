#pragma once
#include "CModel.h"
#include "CColliderMesh.h"

class CInteractObject;

class CSingleDoor : public CObjectBase
{
public:
	// コンストラクタ
	CSingleDoor(CVector pos, CVector angle, CVector size);
	// デストラクタ
	~CSingleDoor();

	// 接続するスイッチを追加
	void AddInputObjs(CInteractObject* sw);
	// 扉の開閉した時の各座標を設定
	void SetAnimPos(const CVector& openPos, const CVector& closePos);
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	// スイッチを押して扉が開くかどうか
	bool IsSwitchOn() const;

	CVector mOpenPosR; // 開いた時の位置
	CVector mClosePosR;// 閉まった時の位置

	float mAnimTime;
	float mElapsedTime;

	bool mIsOpened;  // 開いたか？
	bool mIsPlaying;

	// 入力系のオブジェクトのリスト
	std::list<CInteractObject*> mpInputObjs;

	CModel* mpDoorModel; // シングルドアのモデル
	CColliderMesh* mpDoorCol; // シングルドアのコライダー
};
