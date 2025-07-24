#pragma once
#include "CModel.h"
#include "CColliderMesh.h"

class CInteractObject;

class CLDoubleDoor : public CDoubleDoors
{
public:
	// コンストラクタ
	CLDoubleDoor(CVector pos);
	// デストラクタ
	~CLDoubleDoor();

	// 扉の開閉した時の各座標を設定
//	void SetAnimPosL(const CVector& openPos, const CVector& closePos) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:

	//CVector mOpenPosL;	// 開いた時の位置
	//CVector mClosePosL;	// 閉まった時の位置

	float mAnimTime;
	float mElapsedTime;

	bool mIsOpened;
	bool mIsPlaying;

	CModel* mpLDoorModel;		// 左扉のモデル
	CColliderMesh* mpLDoorCol;	// 右扉のコライダー

};
