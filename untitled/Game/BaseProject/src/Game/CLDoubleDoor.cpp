#include "CDoubleDoors.h"
#include "CLDoubleDoor.h"
#include "CInteractObject.h"

// �R���X�g���N�^
CLDoubleDoor::CLDoubleDoor(CVector pos)
	: mAnimTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsOpened(false)
	, mIsPlaying(false)
{
	Position(pos);

	mpLDoorModel = CResourceManager::Get<CModel>("LDuobleDoors");
	mpLDoorCol = new CColliderMesh(this, ELayer::eWall, mpLDoorModel);
}

// �f�X�g���N�^
CLDoubleDoor::~CLDoubleDoor()
{
	SAFE_DELETE(mpLDoorCol);
}

// �X�V����
void CLDoubleDoor::Update()
{
	/// �J�A�j���[�V��������
	if (mIsPlaying)
	{
		// �����J���ꍇ
		if (mIsOpened)
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector pos = CVector::Lerp(mClosePosL, mOpenPosL, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPosL);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
		// �������ꍇ
		else
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector pos = CVector::Lerp(mOpenPosL, mClosePosL, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePosL);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
	}
	else
	{
		bool isSwitchOn = IsSwitchOn();

		// �X�C�b�`���I���̏�Ԃŕ����܂܂ł���΁A
		if (isSwitchOn && !mIsOpened)
		{
			// �J���A�j���[�V�������J�n
			mIsOpened = true;
			mIsPlaying = true;
		}
		// �X�C�b�`���I�t�̏�ԂŊJ�����܂܂ł���΁A
		else if (!isSwitchOn && mIsOpened)
		{
			// ����A�j���[�V�������J�n
			mIsOpened = false;
			mIsPlaying = true;
		}
	}
}

// �`�揈��
void CLDoubleDoor::Render()
{
	mpLDoorModel->Render(Matrix());
}