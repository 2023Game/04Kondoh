#pragma once
#include "CInteractObject.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CLever;

class CGimmickDoor : public CObjectBase
{
	// コンストラクタ
	CGimmickDoor(CVector pos, CVector angle, CVector size);

	// デストラクタ
	~CGimmickDoor();


	// 接続するスイッチを追加
	void AddInputObjs(CInteractObject* sw);
	//アニメーションするときの移動位置
	void SetAnimPos(const CVector& openPos, const CVector& closePos);


	// 更新処理
	void Update() override;

	// 描画処理
	void Render() override;

private:
	// 扉を開くかどうか
	bool IsSwitchOn() const;

	bool mIsOpened;	// 開いているかどうか

	CVector mOpenPos;
	CVector mClosePos;

	float mAnimTime;
	float mElapsedTime;

	// 各ドアのモデルデータ

	CModel* mpRDoubleDoorsModel;
	CModel* mpLDoubleDoorsModel;
	CModel* mpRastDoorModel;
	// 各ドアのコライダー

	CColliderMesh* mpRDoubleDoorsCol;
	CColliderMesh* mpLDoubleDoorsCol;
	CColliderMesh* mpRastDoorCol;
};
