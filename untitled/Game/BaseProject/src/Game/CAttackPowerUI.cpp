#include "CAttackPowerUI.h"
#include "CImage.h"

#define UI_SIZE 100.0f

// コンストラクタ
CAttackPowerUI::CAttackPowerUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpPowerSImg(nullptr)
	, mpPowerMImg(nullptr)
	, mpPowerLImg(nullptr)
	, mCurrPower(1)
{
	// 弱攻撃のUIイメージ
	mpPowerSImg = new CImage
	(
		"UI\\powerImg_S.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);

	// 中攻撃のUIイメージ
	mpPowerMImg = new CImage
	(
		"UI\\powerImg_M.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);

	// 強攻撃のUIイメージ
	mpPowerLImg = new CImage
	(
		"UI\\powerImg_L.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);

	// パワーのUIイメージ
	mpPowerImg = new CImage
	(
		"UI\\powerImg.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
}

// デストラクタ
CAttackPowerUI::~CAttackPowerUI()
{
	SAFE_DELETE(mpPowerSImg);
	SAFE_DELETE(mpPowerMImg);
	SAFE_DELETE(mpPowerLImg);
}

// 現在のパワーを設定
void CAttackPowerUI::SetCurrPower(int power)
{
	mCurrPower = power;
}

// 更新
void CAttackPowerUI::Update()
{
	mpPowerSImg->SetShow(mCurrPower < 1);
	mpPowerMImg->SetShow(mCurrPower == 1);
	mpPowerLImg->SetShow(mCurrPower > 1);

	mpPowerSImg->Update();
	mpPowerMImg->Update();
	mpPowerLImg->Update();
}

// 描画
void CAttackPowerUI::Render()
{
	CVector2 pos = mPosition;

	mpPowerImg->SetPos(pos - CVector2(UI_SIZE, 0.0f));
	mpPowerImg->SetSize(CVector2(UI_SIZE, UI_SIZE));
	mpPowerImg->Render();

	// 弱攻撃のUI描画
	if (mpPowerSImg->IsShow())
	{
		mpPowerSImg->SetPos(pos);
		mpPowerSImg->SetSize(CVector2(UI_SIZE, UI_SIZE));
		mpPowerSImg->Render();
	}
	// 中攻撃のUI描画
	if (mpPowerMImg->IsShow())
	{
		mpPowerMImg->SetPos(pos);
		mpPowerMImg->SetSize(CVector2(UI_SIZE, UI_SIZE));
		mpPowerMImg->Render();
	}
	// 強攻撃のUI描画
	if (mpPowerLImg->IsShow())
	{
		mpPowerLImg->SetPos(pos);
		mpPowerLImg->SetSize(CVector2(UI_SIZE, UI_SIZE));
		mpPowerLImg->Render();
	}
}
