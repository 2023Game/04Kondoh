#include "CAttackPowerUI.h"
#include "CImage.h"

#define UI_SIZE 100.0f

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

	// ���U����UI�C���[�W
	mpPowerMImg = new CImage
	(
		"UI\\powerImg_M.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);

	// ���U����UI�C���[�W
	mpPowerLImg = new CImage
	(
		"UI\\powerImg_L.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);

	// �p���[��UI�C���[�W
	mpPowerImg = new CImage
	(
		"UI\\powerImg.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
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
	mpPowerSImg->SetShow(mCurrPower < 1);
	mpPowerMImg->SetShow(mCurrPower == 1);
	mpPowerLImg->SetShow(mCurrPower > 1);

	mpPowerSImg->Update();
	mpPowerMImg->Update();
	mpPowerLImg->Update();
}

// �`��
void CAttackPowerUI::Render()
{
	CVector2 pos = mPosition;

	mpPowerImg->SetPos(pos - CVector2(UI_SIZE, 0.0f));
	mpPowerImg->SetSize(CVector2(UI_SIZE, UI_SIZE));
	mpPowerImg->Render();

	// ��U����UI�`��
	if (mpPowerSImg->IsShow())
	{
		mpPowerSImg->SetPos(pos);
		mpPowerSImg->SetSize(CVector2(UI_SIZE, UI_SIZE));
		mpPowerSImg->Render();
	}
	// ���U����UI�`��
	if (mpPowerMImg->IsShow())
	{
		mpPowerMImg->SetPos(pos);
		mpPowerMImg->SetSize(CVector2(UI_SIZE, UI_SIZE));
		mpPowerMImg->Render();
	}
	// ���U����UI�`��
	if (mpPowerLImg->IsShow())
	{
		mpPowerLImg->SetPos(pos);
		mpPowerLImg->SetSize(CVector2(UI_SIZE, UI_SIZE));
		mpPowerLImg->Render();
	}
}
