#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CLever;

class CGimmickDoor : public CObjectBase
{
	CGimmickDoor(CVector pos, CVector angle, CVector size);

	~CGimmickDoor();

	void AddSwitch();

	void Update() override;

	void Render() override;

private:
	CModel* mpDoorModel;
	CModel* mpRDoubleDoorsModel;
	CModel* mpLDoubleDoorsModel;
	CModel* mpRastDoorModel;

	CColliderMesh* mpDoorCol;
	CColliderMesh* mpRDoubleDoorsCol;
	CColliderMesh* mpLDoubleDoorsCol;
	CColliderMesh* mpRastDoorCol;

	// このドアを開くために押す必要のあるスイッチのリスト
	std::list<CLever*> mpLevers;
	bool mIsOpened;

	float mAnimTime;
	float mElapsedTime;
};
