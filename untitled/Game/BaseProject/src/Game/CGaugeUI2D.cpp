#include "CGaugeUI2D.h"
#include "CImage.h"
#include "Maths.h"

#define BAR_SET_POS 8.0f
#define BAR_SET_SIZE 16.0f

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
		"UI\\gauge_2D.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mGaugeSize = mpHpGaugeImg->GetSize();

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
void CGaugeUI2D::Render()
{
	CVector2 pos = mPosition;

	CVector2 barPos = mPosition + CVector2(BAR_SET_POS, BAR_SET_POS);
	mpHpWhiteImg->SetPos(barPos);

	CVector2 barSize = mGaugeSize - CVector2(BAR_SET_SIZE, BAR_SET_SIZE);
	barSize.X(barSize.X() * mPercent);
	mpHpWhiteImg->SetSize(barSize);

	CColor barColor = CColor::green;
	if (mPercent <= 0.2f) barColor = CColor::red;
	else if (mPercent <= 0.5f) barColor = CColor::yellow;
	mpHpWhiteImg->SetColor(barColor);

	mpHpWhiteImg->Render();

	// ゲージ描画
	mpHpGaugeImg->SetPos(pos);
	mpHpGaugeImg->Render();

}


