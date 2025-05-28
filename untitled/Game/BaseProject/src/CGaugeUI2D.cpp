#include "CGaugeUI2D.h"
#include "CImage.h"
#include "Maths.h"

// コンストラクタ
CGaugeUI2D::CGaugeUI2D()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpHpGaugeImg(nullptr)
	, mpHpWhiteImg(nullptr)
	, mMaxPoint(100)
	, mCurrPoint(mMaxPoint)
	, mPercent(1.0f)
{
	//SetPos(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f);

	mpHpGaugeImg = new CImage
	(
		"UI\\gauge2.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mGaugeSize = mpHpGaugeImg->GetSize();
	CVector2 center = CVector2(-mGaugeSize.X() * 0.5f, 0.0f);
	mpHpGaugeImg->SetCenter(center);

	mpHpWhiteImg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
}

// デストラクタ
CGaugeUI2D::~CGaugeUI2D()
{
	SAFE_DELETE(mpHpGaugeImg);
	SAFE_DELETE(mpHpWhiteImg);
}

// 持ち主を設定
void CGaugeUI2D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ポイント残量を反映
void CGaugeUI2D::ApplyPoint()
{
	// 最大値が不正値でなければ
	if (mMaxPoint > 0)
	{
		// 現在値が最大値の何パーセントか求める
		mPercent = Math::Clamp01((float)mCurrPoint / mMaxPoint);
	}
	// 不正値ならが、100%固定
	else
	{
		mPercent = 1.0f;
	}
}

// 最大値を設定
void CGaugeUI2D::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// 現在地を設定
void CGaugeUI2D::SetCurrPoint(int point)
{
	mCurrPoint = point;
	ApplyPoint();
}

// ポイント残量の割合を設定（0.0～1.0）
void CGaugeUI2D::SetPercent(float per)
{
	mPercent = Math::Clamp01(per);
}

// 更新
void CGaugeUI2D::Update()
{
	mpHpGaugeImg->Update();
	mpHpWhiteImg->Update();
}

// 描画
void CGaugeUI2D::Rendar()
{
	CVector2 pos = mPosition;

	mpHpGaugeImg->SetPos(pos);
	mpHpGaugeImg->Render();


	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - mGaugeSize.X() * 0.5f);
	mpHpWhiteImg->SetPos(barPos);

	CVector2 barSize = mGaugeSize;
	barSize.X(barSize.X() * mPercent);
	mpHpWhiteImg->SetSize(barSize);

	mpHpWhiteImg->Render();

}


