#include "CDoubleDoors.h"
#include "CLDoubleDoor.h"
#include "CInteractObject.h"

// コンストラクタ
CLDoubleDoor::CLDoubleDoor(CVector pos)
	: mAnimTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsOpened(false)
	, mIsPlaying(false)
{
	Position(pos);

	mpLDoorModel = CResourceManager::Get<CModel>("LDuobleDoors");
	mpLDoorCol = new CColliderMesh(this, ELayer::eWall, mpLDoorModel);
}

// デストラクタ
CLDoubleDoor::~CLDoubleDoor()
{
	SAFE_DELETE(mpLDoorCol);
}

// 更新処理
void CLDoubleDoor::Update()
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
				CVector pos = CVector::Lerp(mClosePosL, mOpenPosL, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPosL);
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
				CVector pos = CVector::Lerp(mOpenPosL, mClosePosL, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePosL);
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

// 描画処理
void CLDoubleDoor::Render()
{
	mpLDoorModel->Render(Matrix());
}