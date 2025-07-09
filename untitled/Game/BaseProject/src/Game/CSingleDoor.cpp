#include "CSingleDoor.h"

CSingleDoor::CSingleDoor(CVector pos, CVector angle, CVector size)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
{
	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);

	mpDoorModel = CResourceManager::Get<CModel>("Door");
	mpDoorCol = new CColliderMesh(this, ELayer::eWall, mpDoorModel);
}

CSingleDoor::~CSingleDoor()
{
	SAFE_DELETE(mpDoorCol);
}

// 接続するスイッチを設定
void CSingleDoor::AddInputObjs(CInteractObject* sw)
{
	mpInputObjs.push_back(sw);
}

void CSingleDoor::SetAnimPos(const CVector& openPos, const CVector& closePos)
{
	mOpenPos = openPos;
	mClosePos = closePos;
	Position(mIsOpened ? mOpenPos : mClosePos);
}

void CSingleDoor::Update()
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
				CVector pos = CVector::Lerp(mClosePos, mOpenPos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPos);
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
				CVector pos = CVector::Lerp(mOpenPos, mClosePos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePos);
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

void CSingleDoor::Render()
{
	mpDoorModel->Render(Matrix());
}
