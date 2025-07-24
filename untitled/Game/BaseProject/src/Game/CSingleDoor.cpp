#include "CSingleDoor.h"
#include "CInteractObject.h"

CSingleDoor::CSingleDoor(CVector pos, CVector angle, CVector size)
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
	, mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
{
	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

	mpDoorModel = CResourceManager::Get<CModel>("SingleDoor");
	mpDoorCol = new CColliderMesh(this, ELayer::eWall, mpDoorModel);
}

CSingleDoor::~CSingleDoor()
{
	SAFE_DELETE(mpDoorCol);
}

// �ڑ�����X�C�b�`��ݒ�
void CSingleDoor::AddInputObjs(CInteractObject* sw)
{
	mpInputObjs.push_back(sw);
}

void CSingleDoor::SetAnimPos(const CVector& openPos, const CVector& closePos)
{
	mOpenPosR = openPos;
	mClosePosR = closePos;
	Position(mIsOpened ? mOpenPosR : mClosePosR);
}



bool CSingleDoor::IsSwitchOn() const
{
	for (CInteractObject* sw : mpInputObjs)
	{
		if (!sw->IsOnInteractObj()) return false;
	}
	return true;
}


void CSingleDoor::Update()
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
				CVector pos = CVector::Lerp(mClosePosR, mOpenPosR, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPosR);
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
				CVector pos = CVector::Lerp(mOpenPosR, mClosePosR, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePosR);
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

void CSingleDoor::Render()
{
	mpDoorModel->Render(Matrix());
}

