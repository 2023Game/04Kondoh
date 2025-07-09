#include "GimmickDoor.h"

// コンストラクタ
CGimmickDoor::CGimmickDoor(CVector pos, CVector angle, CVector size)
{
	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);

	mpRDoubleDoorsModel = CResourceManager::Get<CModel>("RDuobleDoors");
	mpRDoubleDoorsCol = new CColliderMesh(this, ELayer::eWall, mpRDoubleDoorsModel);

	mpLDoubleDoorsModel = CResourceManager::Get<CModel>("LDuobleDoors");
	mpLDoubleDoorsCol = new CColliderMesh(this, ELayer::eWall, mpLDoubleDoorsModel);

	mpRastDoorModel = CResourceManager::Get<CModel>("RastDoor");
	mpRastDoorCol = new CColliderMesh(this, ELayer::eWall, mpRastDoorModel);
}

// デストラクタ
CGimmickDoor::~CGimmickDoor()
{
	SAFE_DELETE(mpRDoubleDoorsCol);
	SAFE_DELETE(mpLDoubleDoorsCol);
	SAFE_DELETE(mpRastDoorCol);
}

/*
// 接続するスイッチを設定
void CGimmickDoor::AddInputObjs(CInteractObject* sw)
{
	mpInputObjs.push_back(sw);
}

void CGimmickDoor::SetAnimPos(const CVector& openPos, const CVector& closePos)
{
	mOpenPos = openPos;
	mClosePos = closePos;
}

bool CGimmickDoor::IsSwitchOn() const
{
	for (CInteractObject* sw : mpInputObjs)
	{
		if (!sw->IsOnInteractObj()) return false;
	}
	return true;
}

// 更新処理
void CGimmickDoor::Update()
{
	// 開閉アニメーション中か
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

// 描画処理
void CGimmickDoor::Render()
{
}
*/


