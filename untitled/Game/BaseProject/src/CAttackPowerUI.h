#pragma once
#include "CImage.h"

class CImage;

class CAttackPowerUI : public CUIBase
{
public:
	// �R���X�g���N�^
	CAttackPowerUI();
	// �f�X�g���N�^
	virtual ~CAttackPowerUI();
	// ���݂̃p���[��ݒ�
	void SetCurrPower(int power);

	// �X�V
	void Update() override;
	// �`��
	void Render();

private:

	CImage* mpPowerSImg;	// ��U����UI�C���[�W
	CImage* mpPowerMImg;	// ���U����UI�C���[�W
	CImage* mpPowerLImg;	// ���U����UI�C���[�W
	CImage* mpPowerImg;		// �p���[��UI�C���[�W

	int mCurrPower; // ���ݒl
};