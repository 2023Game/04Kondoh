#include "CLever.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
#include "Maths.h"

#define ROT_ANGLE 45.0f
#define ROT_TIME 1.0f

CLever::CLever(const CVector& pos)
	: mpLeverBaseModel(nullptr)
	, mpLeverModel(nullptr)
	, mIsPlaying(false)
	, mIsOn(false)
	, mLeverAngle(ROT_ANGLE)
	, mStartAngle(0)
	, mEndAngle(0)
	, mElapsedTime(0.0f)
{
	// 位置と向きとサイズを設定
	Position(pos);

	mpLeverBaseModel = CResourceManager::Get<CModel>("LeverBase");
	mpLeverModel = CResourceManager::Get<CModel>("Lever");

	mpLeverCol = new CColliderMesh(this, ELayer::eInteractObj,mpLeverModel);
	mpLeverCol->SetCollisionTags({ ETag::ePlayer });
	mpLeverCol->SetCollisionLayers({ ELayer::ePlayer, ELayer::eInteractSearch, ELayer::eAttackCol });
	mpLeverCol->SetShow(true);

	mpLeverBaseCol = new CColliderMesh(this, ELayer::eInteractObj,mpLeverBaseModel);
	mpLeverBaseCol->SetCollisionTags({ ETag::ePlayer });
	mpLeverBaseCol->SetCollisionLayers({ ELayer::ePlayer, ELayer::eAttackCol });
	mpLeverBaseCol->SetShow(true);

	mInteractStr = "オンにする";
}

CLever::~CLever()
{
	SAFE_DELETE(mpLeverBaseCol);
	SAFE_DELETE(mpLeverCol);
}

void CLever::Interact()
{
	if (mIsPlaying) return;

	mIsOn = !mIsOn;
	mInteractStr = mIsOn ? "オフにする" : "オンにする";
	// レバーの角度を変える
	mStartAngle = mLeverAngle;
	mEndAngle = mIsOn ? ROT_ANGLE : -ROT_ANGLE;
	mElapsedTime = 0.0f;
	mIsPlaying = true;
}

bool CLever::IsOn() const
{
	return false;
}

void CLever::Update()
{
	if (!mIsPlaying) return;

	if (mElapsedTime < ROT_TIME)
	{
		float percent = mElapsedTime / ROT_TIME;
		mLeverAngle = Math::Lerp(mStartAngle, mEndAngle, percent);

		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		mLeverAngle = mEndAngle;
		mIsPlaying = false;
	}
}

void CLever::Render()
{
	CMatrix m = Matrix();
	mpLeverBaseModel->Render(m);

	CMatrix rotM;
	rotM.RotateX(mLeverAngle);
	mpLeverModel->Render(rotM * m);
}

void CLever::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpLeverCol || self == mpLeverBaseCol)
	{
		if (other->Tag() == ETag::ePlayer && other->Layer() == ELayer::eInteractSearch)
		{

		}
	}
}
