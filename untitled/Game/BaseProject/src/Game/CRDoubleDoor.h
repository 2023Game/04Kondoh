#pragma once
#include "CDoubleDoors.h"

class CInteractObject;

class CRDoubleDoor : public CDoubleDoors
{
public:
	// コンストラクタ
	CRDoubleDoor(CVector pos, CVector angle, CVector size);
	// デストラクタ
	~CRDoubleDoor();

	// 接続するスイッチを追加
	void AddInputObjs(CInteractObject* sw);
	// 扉の開閉した時の各座標を設定
	void SetAnimPos(const CVector& openPos, const CVector& clpsePos);
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	// スイッチを押して扉が開くかどうか
	bool IsSwitchOn() const;

	CVector mOpenPos;	// 開いた時の位置
	CVector mClosePos;	// 閉まった時の位置

	float mAnimTime;
	float mElapsedTime;

};
