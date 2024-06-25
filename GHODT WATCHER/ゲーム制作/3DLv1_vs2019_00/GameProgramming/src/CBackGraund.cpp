#include "CBackGround.h"

CBackGround::CBackGround()
{
	spInstance = this;
}

//CBackGround(�ʒu,��],�X�P�[��)
CBackGround::CBackGround(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale);  //�s��X�V
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