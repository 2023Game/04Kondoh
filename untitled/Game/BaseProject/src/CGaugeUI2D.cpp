#include "CGaugeUI2D.h"
#include "CImage.h"
#include "Maths.h"

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


