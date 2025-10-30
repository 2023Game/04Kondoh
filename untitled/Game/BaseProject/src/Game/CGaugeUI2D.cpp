#include "CGaugeUI2D.h"
#include "CImage.h"
#include "Maths.h"

#define BAR_SET_POS 8.0f
#define BAR_SET_SIZE 16.0f

// �R���X�g���N�^
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

// �f�X�g���N�^
CGaugeUI2D::~CGaugeUI2D()
{
	SAFE_DELETE(mpHpGaugeImg);
	SAFE_DELETE(mpHpWhiteImg);
}

// �������ݒ�
void CGaugeUI2D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �|�C���g�c�ʂ𔽉f
void CGaugeUI2D::ApplyPoint()
{
	// �ő�l���s���l�łȂ����
	if (mMaxPoint > 0)
	{
		// ���ݒl���ő�l�̉��p�[�Z���g�����߂�
		mPercent = Math::Clamp01((float)mCurrPoint / mMaxPoint);
	}
	// �s���l�Ȃ炪�A100%�Œ�
	else
	{
		mPercent = 1.0f;
	}
}

// �ő�l��ݒ�
void CGaugeUI2D::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// ���ݒn��ݒ�
void CGaugeUI2D::SetCurrPoint(int point)
{
	mCurrPoint = point;
	ApplyPoint();
}

// �|�C���g�c�ʂ̊�����ݒ�i0.0�`1.0�j
void CGaugeUI2D::SetPercent(float per)
{
	mPercent = Math::Clamp01(per);
}

// �X�V
void CGaugeUI2D::Update()
{
	mpHpGaugeImg->Update();
	mpHpWhiteImg->Update();
}

// �`��
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

	// �Q�[�W�`��
	mpHpGaugeImg->SetPos(pos);
	mpHpGaugeImg->Render();

}


