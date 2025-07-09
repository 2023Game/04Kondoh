#pragma once
#include "CObjectBase.h"
#include "CInteractObject.h"
#include "CModel.h"

class CLever : public CInteractObject
{
public:
	CLever(const CVector& pos);
	~CLever();

	// 調べる
	void Interact() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	CModel* mpLeverBaseModel;	// レバーの土台のモデル
	CModel* mpLeverModel;		// レバーのモデル

	CColliderMesh* mpLeverBaseCol;// レバーの土台コライダー
	CColliderMesh* mpLeverCol;	// レバーのコライダー

	CTransform* mpParentObj;




	float mLeverAngle;	// レバーの現在の角度
	float mStartAngle;	// レバー回転開始時の角度
	float mEndAngle;	// レバー回転終了時の角度
	float mElapsedTime;

};