#pragma once
#include "CUIBase.h"

class CImage;

class CGaugeUI2D : public CUIBase
{
public:
	// �R���X�g���N�^
	CGaugeUI2D();
	// �f�X�g���N�^
	virtual ~CGaugeUI2D();

	// �������ݒ�
	void SetOwner(CObjectBase* owner);

	// �ő�l��ݒ�
	void SetMaxPoint(int point);
	// ���ݒn��ݒ�
	void SetCurrPoint(int point);
	// �|�C���g�c�ʂ̊�����ݒ�i0.0�`1.0�j
	void SetPercent(float per);

	// �X�V
	void Update() override;
	// �`��
	void Render() ;

private:

	// �|�C���g�c�ʂ𔽉f
	void ApplyPoint();

	CObjectBase* mpOwner;	// HP�Q�[�W�̎�����

	CImage* mpHpGaugeImg;	// HP�Q�[�W�̃C���[�W
	CImage* mpHpWhiteImg;	// HP�o�[�̔��C���[�W

	CVector2 mGaugeSize;	// �Q�[�W�̃C���[�W�̃T�C�Y
	int mMaxPoint;			// �ő�l
	int mCurrPoint;			// ���ݒl
	float mPercent;			// �|�C���g�c�ʂ̊���


};
