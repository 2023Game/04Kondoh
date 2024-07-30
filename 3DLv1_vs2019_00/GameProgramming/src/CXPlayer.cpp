#include "CXPlayer.h"
#include "CApplication.h"
#include "CXCharacter.h"

void CXPlayer::Update() {

	if (mInput.Key('A')) {

		mRotation.Y(mRotation.Y() + 2.0);
	}
	if (mInput.Key('D')) {

		mRotation.Y(mRotation.Y() - 2.0);
	}
	if (mInput.Key('W')) {
		ChangeAnimation(1, true, 60);
		Position(Position() + mMatrixRotate.VectorZ() * 1.0);
	}
	else
	{
		ChangeAnimation(0, true, 60);
	}

	CXCharacter::Update();
}