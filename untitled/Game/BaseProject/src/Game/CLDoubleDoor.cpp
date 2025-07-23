#include "CDoubleDoors.h"
#include "CLDoubleDoor.h"
#include "CInteractObject.h"

// �R���X�g���N�^
CLDoubleDoor::CLDoubleDoor(CVector pos, CVector angle, CVector size)
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
{
	Position(pos);
	Rotation(angle);
	Scale(size);

	mpLDoorModel = CResourceManager::Get<CModel>("LDuobleDoors");
	mpLDoorCol = new CColliderMesh(this, ELayer::eWall, mpLDoorModel);
}

// �f�X�g���N�^
CLDoubleDoor::~CLDoubleDoor()
{
	SAFE_DELETE(mpLDoorCol);
}

// �ڑ�����X�C�b�`��ǉ�
void CLDoubleDoor::AddInputObjs(CInteractObject* sw)
{
	mpInputObjs.push_back(sw);
}

// ���̊J�������̊e���W��ݒ�
void CLDoubleDoor::SetAnimPos(const CVector& openPos, const CVector& closePos)
{
	mOpenPos = openPos;
	mClosePos = closePos;
	Position(mIsOpened ? mOpenPos : mClosePos);
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
				CVector pos = CVector::Lerp(mClosePos, mOpenPos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPos);
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
				CVector pos = CVector::Lerp(mOpenPos, mClosePos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePos);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
	}
	else
	{
		bool isSwitchOn = CDoubleDoors::IsSwitchOn();

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