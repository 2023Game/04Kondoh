#pragma once
#include "CModel.h"
#include "CColliderMesh.h"

class CInteractObject;

class CRDoubleDoor : public CDoubleDoors
{
public:
	// コンストラクタ
	CRDoubleDoor(CVector pos);
	// デストラクタ
	~CRDoubleDoor();

	// 扉の開閉した時の各座標を設定
//	void SetAnimPosR(const CVector& openPos, const CVector& clpsePos) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:

	//CVector mOpenPosR;	// 開いた時の位置
	//CVector mClosePosR;	// 閉まった時の位置

	float mAnimTime;
	float mElapsedTime;


	bool mIsPlaying;

	CModel* mpRDoorModel;		// 右扉のモデル
	CColliderMesh* mpRDoorCol;	// 右扉のコライダー

};
