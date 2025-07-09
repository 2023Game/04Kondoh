#include "GimmickDoor.h"

// �R���X�g���N�^
CGimmickDoor::CGimmickDoor(CVector pos, CVector angle, CVector size)
{
	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

	mpRDoubleDoorsModel = CResourceManager::Get<CModel>("RDuobleDoors");
	mpRDoubleDoorsCol = new CColliderMesh(this, ELayer::eWall, mpRDoubleDoorsModel);

	mpLDoubleDoorsModel = CResourceManager::Get<CModel>("LDuobleDoors");
	mpLDoubleDoorsCol = new CColliderMesh(this, ELayer::eWall, mpLDoubleDoorsModel);

	mpRastDoorModel = CResourceManager::Get<CModel>("RastDoor");
	mpRastDoorCol = new CColliderMesh(this, ELayer::eWall, mpRastDoorModel);
}

// �f�X�g���N�^
CGimmickDoor::~CGimmickDoor()
{
	SAFE_DELETE(mpRDoubleDoorsCol);
	SAFE_DELETE(mpLDoubleDoorsCol);
	SAFE_DELETE(mpRastDoorCol);
}

/*
// �ڑ�����X�C�b�`��ݒ�
void CGimmickDoor::AddInputObjs(CInteractObject* sw)
{
	mpInputObjs.push_back(sw);
}

void CGimmickDoor::SetAnimPos(const CVector& openPos, const CVector& closePos)
{
	mOpenPos = openPos;
	mClosePos = closePos;
}

bool CGimmickDoor::IsSwitchOn() const
{
	for (CInteractObject* sw : mpInputObjs)
	{
		if (!sw->IsOnInteractObj()) return false;
	}
	return true;
}

// �X�V����
void CGimmickDoor::Update()
{
	// �J�A�j���[�V��������
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
void CGimmickDoor::Render()
{
}
*/


