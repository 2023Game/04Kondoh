#include "CInteractObject.h"

// �R���X�g���N�^
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, sortOrder, pause)
	, mInteractStr("���ׂ�")
	, mIsPlaying(false)
	, mIsOnIntaractObj(false)

#if _DEBUG	
	, mDebugName("InteractObj")
#endif
{
}

// �f�X�g���N�^
CInteractObject::~CInteractObject()
{
}

// ���ׂ����Ԃ��ǂ���
bool CInteractObject::CanInteract() const
{
	return true;
}

bool CInteractObject::IsOnInteractObj() const
{
	return mIsOnIntaractObj;
}


// ���ׂ���e�̃e�L�X�g��Ԃ�
std::string CInteractObject::GetInteractStr() const
{
	return mInteractStr;
}


#if _DEBUG	

// �f�o�b�O�\���p�̖��O���擾
std::string CInteractObject::GetDebugName() const
{
	return mDebugName;
}

// �f�o�b�O�\���p�̖��O��ݒ�
void CInteractObject::SetDebugName(std::string name)
{
	mDebugName = name;
}

#endif
