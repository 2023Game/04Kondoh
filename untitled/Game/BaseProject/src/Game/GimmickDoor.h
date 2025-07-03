#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CLever;

class CGimmickDoor : public CObjectBase
{
	CGimmickDoor(CVector pos, CVector rot, CVector size);

	~CGimmickDoor();

	void AddSwitch();

	void Update() override;

	void Render() override;

private:
	CModel* mpDoorModel;
	CModel* mpRDoubleDoorsModel;
	CModel* mpLDoubleDoorsModel;
	CModel* mpRastDoor;

	CColliderMesh* mpColliderMesh;
	// ���̃h�A���J�����߂ɉ����K�v�̂���X�C�b�`�̃��X�g
	std::list<CLever*> mpLevers;
	bool mIsOpened;

	float mAnimTime;
	float mElapsedTime;
};
