#include "CAttackPowerUI.h"
#include "CImage.h"

#define UI_SIZE CVector2(64.0f, 64.0f)

// �R���X�g���N�^
CAttackPowerUI::CAttackPowerUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpPowerSImg(nullptr)
	, mpPowerMImg(nullptr)
	, mpPowerLImg(nullptr)
	, mCurrPower(1)
{
	// ��U����UI�C���[�W
	mpPowerSImg = new CImage
	(
		"UI\\powerImg_S.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpPowerSImg->SetEnable(false);

	// ���U����UI�C���[�W
	mpPowerMImg = new CImage
	(
		"UI\\powerImg_M.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpPowerMImg->SetEnable(false);

	// ���U����UI�C���[�W
	mpPowerLImg = new CImage
	(
		"UI\\powerImg_L.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpPowerLImg->SetEnable(false);
}

// �f�X�g���N�^
CAttackPowerUI::~CAttackPowerUI()
{
	SAFE_DELETE(mpPowerSImg);
	SAFE_DELETE(mpPowerMImg);
	SAFE_DELETE(mpPowerLImg);
}

// ���݂̃p���[��ݒ�
void CAttackPowerUI::SetCurrPower(int power)
{
	mCurrPower = power;
}

// �X�V
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

// �`��
void CAttackPowerUI::Render()
{
	CVector2 pos = mPosition;
	// ��U����UI�`��
	mpPowerSImg->SetPos(pos);
	mpPowerSImg->SetSize(UI_SIZE);
	mpPowerSImg->Render();
	// ���U����UI�`��
	mpPowerMImg->SetPos(pos);
	mpPowerMImg->SetSize(UI_SIZE);
	mpPowerMImg->Render();
	// ���U����UI�`��
	mpPowerLImg->SetPos(pos);
	mpPowerLImg->SetSize(UI_SIZE);
	mpPowerLImg->Render();
}
