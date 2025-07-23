#include "CDoubleDoors.h"
#include "CLDoubleDoor.h"
#include "CInteractObject.h"

// コンストラクタ
CLDoubleDoor::CLDoubleDoor(CVector pos, CVector angle, CVector size)
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
{
	Position(pos);
	Rotation(angle);
	Scale(size);

	mpLDoorModel = CResourceManager::Get<CModel>("LDuobleDoors");
	mpLDoorCol = new CColliderMesh(this, ELayer::eWall, mpLDoorModel);
}

// デストラクタ
CLDoubleDoor::~CLDoubleDoor()
{
	SAFE_DELETE(mpLDoorCol);
}

// 接続するスイッチを追加
void CLDoubleDoor::AddInputObjs(CInteractObject* sw)
{
	mpInputObjs.push_back(sw);
}

// 扉の開閉した時の各座標を設定
void CLDoubleDoor::SetAnimPos(const CVector& openPos, const CVector& closePos)
{
	mOpenPos = openPos;
	mClosePos = closePos;
	Position(mIsOpened ? mOpenPos : mClosePos);
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
		bool isSwitchOn = CDoubleDoors::IsSwitchOn();

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