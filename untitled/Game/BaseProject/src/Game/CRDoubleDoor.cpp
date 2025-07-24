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
	/// 開閉アニメーション中か
	if (mIsPlaying)
	{
		// 扉を開く場合
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
		// 扉を閉じる場合
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

		// スイッチがオンの状態で閉じたままであれば、
		if (isSwitchOn && !mIsOpened)
		{
			// 開くアニメーションを開始
			mIsOpened = true;
			mIsPlaying = true;
		}
		// スイッチがオフの状態で開いたままであれば、
		else if (!isSwitchOn && mIsOpened)
		{
			// 閉じるアニメーションを開始
			mIsOpened = false;
			mIsPlaying = true;
		}
	}
}

void CRDoubleDoor::Render()
{
	mpRDoorModel->Render(Matrix());
}