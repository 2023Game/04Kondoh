#include "CSingleDoor.h"
#include "CInteractObject.h"

// �R���X�g���N�^
CSingleDoor::CSingleDoor(CVector pos, CVector angle, CVector size)
	: CObjectBase(ETag::eDoor, ETaskPriority::eBackground)
	, mIsOpened(true)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
{
	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

	mpDoorModel = CResourceManager::Get<CModel>("SingleDoor");
	mpDoorCol = new CColliderMesh(this, ELayer::eWall, mpDoorModel, true);
	mpDoorCol->SetShow(true);
}

// �f�X�g���N�^
CSingleDoor::~CSingleDoor()
{
	SAFE_DELETE(mpDoorCol);
}

// �ڑ�����X�C�b�`��ݒ�
void CSingleDoor::AddInputObjs(CInteractObject* sw)
{
	mpInputObjs.push_back(sw);
}

// ���̊J�������̊e���W��ݒ�
void CSingleDoor::SetAnimPos(const CVector& openPos, const CVector& closePos)
{
	mOpenPos = openPos;
	mClosePos = closePos;
	Position(mIsOpened ? mClosePos : mOpenPos);
}

// �X�C�b�`�������Ĕ����J�����ǂ���
bool CSingleDoor::IsSwitchOn() const
{
	for (CInteractObject* sw : mpInputObjs)
	{
		if (!sw->IsOnInteractObj()) return false;
	}
	return true;
}

// �X�V����
void CSingleDoor::Update()
{
	/// �J�A�j���[�V��������
	if (mIsPlaying)
	{
		// �����J���ꍇ
		if (!mIsOpened)
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
void CSingleDoor::Render()
{
	mpDoorModel->Render(Matrix());
}

