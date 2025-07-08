#include "GimmickDoor.h"

CGimmickDoor::CGimmickDoor(CVector pos, CVector angle, CVector size)
	: mAnimTime(0.0f)
	, mElapsedTime(0.0f)
{
	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);

	mpDoorModel = CResourceManager::Get<CModel>("Door");
	mpDoorCol = new CColliderMesh(this, ELayer::eWall, mpDoorModel);

	mpRDoubleDoorsModel = CResourceManager::Get<CModel>("RDuobleDoors");
	mpRDoubleDoorsCol = new CColliderMesh(this, ELayer::eWall, mpRDoubleDoorsModel);

	mpLDoubleDoorsModel = CResourceManager::Get<CModel>("LDuobleDoors");
	mpLDoubleDoorsCol = new CColliderMesh(this, ELayer::eWall, mpLDoubleDoorsModel);

	mpRastDoorModel = CResourceManager::Get<CModel>("RastDoor");
	mpRastDoorCol = new CColliderMesh(this, ELayer::eWall, mpRastDoorModel);
}

CGimmickDoor::~CGimmickDoor()
{
	SAFE_DELETE(mpDoorCol);
	SAFE_DELETE(mpRDoubleDoorsCol);
	SAFE_DELETE(mpLDoubleDoorsCol);
	SAFE_DELETE(mpRastDoorCol);
}

void CGimmickDoor::AddSwitch()
{
}

void CGimmickDoor::Update()
{
}

void CGimmickDoor::Render()
{
}
