#include "CDoubleDoors.h"
#include "CRDoubleDoor.h"
#include "CInteractObject.h"

CRDoubleDoor::CRDoubleDoor(CVector pos)
	: mAnimTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsOpened(false)
	, mIsPlaying(false)
{
	Position(pos);

	mpRDoorModel = CResourceManager::Get<CModel>("RDuobleDoor");
	mpRDoorCol = new CColliderMesh(this, ELayer::eWall, mpRDoorModel);
}

CRDoubleDoor::~CRDoubleDoor()
{
	SAFE_DELETE(mpRDoorCol);
}

void CRDoubleDoor::Update()
{
	/// �J�A�j���[�V��������
	if (mIsPlaying)
	{
		// �����J���ꍇ
		if (mIsOpened)
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector pos = CVector::Lerp(mClosePosR, mOpenPosR, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPosR);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
		// �������ꍇ
		else
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector pos = CVector::Lerp(mOpenPosR, mClosePosR, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePosR);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
	}
	else
	{
		bool isSwitchOn = IsSwitchOn();

		// �X�C�b�`���I���̏�Ԃŕ����܂܂ł���΁A
		if (isSwitchOn && !mIsOpened)
		{
			// �J���A�j���[�V�������J�n
			mIsOpened = true;
			mIsPlaying = true;
		}
		// �X�C�b�`���I�t�̏�ԂŊJ�����܂܂ł���΁A
		else if (!isSwitchOn && mIsOpened)
		{
			// ����A�j���[�V�������J�n
			mIsOpened = false;
			mIsPlaying = true;
		}
	}
}

void CRDoubleDoor::Render()
{
	mpRDoorModel->Render(Matrix());
}