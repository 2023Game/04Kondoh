#include "CBackGround.h"

CBackGround::CBackGround()
{
	spInstance = this;
}

//CBackGround(位置,回転,スケール)
CBackGround::CBackGround(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale);  //行列更新
}

void CBackGround::Update()
{
	CTransform::Update();
}

void CBackGround::ColliderMesh()
{
	mColliderMesh.Set(this, &mMatrix, mpModel);
}



CBackGround* CBackGround::spInstance = nullptr;

CBackGround* CBackGround::Instance() {
	return spInstance;
}