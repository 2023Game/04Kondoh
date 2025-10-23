#pragma once
#include "CImage.h"

class CImage;

class CAttackPowerUI : public CUIBase
{
public:
	// コンストラクタ
	CAttackPowerUI();
	// デストラクタ
	virtual ~CAttackPowerUI();
	// 現在のパワーを設定
	void SetCurrPower(int power);

	// 更新
	void Update() override;
	// 描画
	void Render();

private:

	CImage* mpPowerSImg;	// 弱攻撃のUIイメージ
	CImage* mpPowerMImg;	// 中攻撃のUIイメージ
	CImage* mpPowerLImg;	// 強攻撃のUIイメージ
	CImage* mpPowerImg;		// パワーのUIイメージ

	int mCurrPower; // 現在値
};