#include "CMoveFloor.h"
#include "Maths.h"

CMoveFloor::CMoveFloor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& move, float moveTime)
	: mpModel(model)
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
{
	Position(mDefaultPos);
	Scale(scale);

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

}

CMoveFloor::~CMoveFloor()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}


void CMoveFloor::Update()
{
	float per = mElapsedTime / mMoveTime;
	Position(mDefaultPos + mMoveVec * sinf(M_PI * 2.0f * per));

	mElapsedTime += 1.0f / 60.0f;
	if (mElapsedTime >= mMoveTime)
	{
		mElapsedTime -= mMoveTime;
	}
}

void CMoveFloor::Render()
{
	mpModel->Render(Matrix());
}
