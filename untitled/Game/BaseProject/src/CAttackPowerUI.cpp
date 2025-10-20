#include "CAttackPowerUI.h"
#include "CImage.h"

#define UI_SIZE CVector2(64.0f, 64.0f)

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
	mpPowerSImg->SetEnable(false);

	// 中攻撃のUIイメージ
	mpPowerMImg = new CImage
	(
		"UI\\powerImg_M.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpPowerMImg->SetEnable(false);

	// 強攻撃のUIイメージ
	mpPowerLImg = new CImage
	(
		"UI\\powerImg_L.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpPowerLImg->SetEnable(false);
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
	if (mCurrPower < 1)
	{
		mpPowerSImg->SetEnable(true);
		mpPowerMImg->SetEnable(false);
		mpPowerLImg->SetEnable(false);
	}
	else if (mCurrPower > 1)
	{
		mpPowerSImg->SetEnable(false);
		mpPowerMImg->SetEnable(false);
		mpPowerLImg->SetEnable(true);
	}
	else
	{
		mpPowerSImg->SetEnable(false);
		mpPowerMImg->SetEnable(true);
		mpPowerLImg->SetEnable(false);
	}

	mpPowerSImg->Update();
	mpPowerMImg->Update();
	mpPowerLImg->Update();
}

// 描画
void CAttackPowerUI::Render()
{
	CVector2 pos = mPosition;
	// 弱攻撃のUI描画
	mpPowerSImg->SetPos(pos);
	mpPowerSImg->SetSize(UI_SIZE);
	mpPowerSImg->Render();
	// 中攻撃のUI描画
	mpPowerMImg->SetPos(pos);
	mpPowerMImg->SetSize(UI_SIZE);
	mpPowerMImg->Render();
	// 強攻撃のUI描画
	mpPowerLImg->SetPos(pos);
	mpPowerLImg->SetSize(UI_SIZE);
	mpPowerLImg->Render();
}
