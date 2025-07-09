#pragma once
#include "CObjectBase.h"

// ���ׂ�I�u�W�F�N�g�̃x�[�X�N���X�i���ۃN���X�j
class CInteractObject : public CObjectBase
{
public:
	// �R���X�g���N�^
	CInteractObject(
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);

	// �f�X�g���N�^
	virtual ~CInteractObject();

	// ���ׂ����Ԃ��ǂ���
	virtual bool CanInteract() const;

	// ���ׂ�i��΁I�I�p����Ŏ����j
	// �������z�֐� (�֐�() = 0;)
	virtual void Interact() = 0;
	// �C���^���N�g�I�u�W�F�N�g���I���ɂȂ�����
	virtual bool IsOnInteractObj() const;

	// ���ׂ���e�̃e�L�X�g��Ԃ�
	std::string GetInteractStr() const;



#if _DEBUG	
	// �f�o�b�O�\���p�̖��O���擾
	std::string GetDebugName() const;
	// �f�o�b�O�\���p�̖��O��ݒ�
	void SetDebugName(std::string name);
#endif

protected:

	bool mIsPlaying;	// �A�j���[�V�����Đ�����
	bool mIsOnIntaractObj;	// �I���ɂȂ��Ă��邩

	// ���̓I�u�W�F�N�g�̃��X�g
	std::list<CInteractObject*> mpInputObjs;

	std::string mInteractStr;	// ���ׂ���e�̃e�L�X�g



#if _DEBUG
	std::string mDebugName;	// �f�o�b�O�\���p
#endif
};
