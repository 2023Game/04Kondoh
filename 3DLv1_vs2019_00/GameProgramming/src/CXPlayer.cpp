#include "CXPlayer.h"
#include "CApplication.h"
#include "CXCharacter.h"

CXPlayer::CXPlayer()
	:mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereHead(this, nullptr,
		CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereSword(this, nullptr,
		CVector(-10.0f, 10.0f, 50.0f), 0.3f,CCollider::ETag::ESWORD)
{
}

void CXPlayer::Init(CModelX *model) {
	CXCharacter::Init(model);
	//çáê¨çsóÒÇÃê›íË
	mColSphereBody.Matrix(&mpCombinedMatrix[9]);
	//ì™
	mColSphereHead.Matrix(&mpCombinedMatrix[12]);
	//åï
	mColSphereSword.Matrix(&mpCombinedMatrix[22]);
}

void CXPlayer::Update() {

	switch (AnimationIndex())
	{
	case 3:
		if (IsAnimationFinished())
		{
			ChangeAnimation(4, false, 30);
		}
		break;
	case 4:
		if (IsAnimationFinished())
		{
			ChangeAnimation(0, true, 60);
		}
		break;
	default:

		if (mInput.Key('A')) {

			mRotation.Y(mRotation.Y() + 2.0);
		}
		if (mInput.Key('D')) {

			mRotation.Y(mRotation.Y() - 2.0);
		}
		if (mInput.Key('W')) {
			ChangeAnimation(1, true, 60);
			Position(Position() + mMatrixRotate.VectorZ() * 0.1);
		}
		else
		{
			ChangeAnimation(0, true, 60);
		}
		if (mInput.Key(' ')) {
			ChangeAnimation(3, false, 30);
		}
	}

	CXCharacter::Update();
}