#include "CTaskManager.h"
//�f�t�H���g�R���X�g���N�^
CTaskManager::CTaskManager()
{
	mHead.mpNext = &mTail;
	mTail.mpPrev = &mHead;
}
CTaskManager::~CTaskManager() {
}
//���X�g�ɒǉ�
//Add(�^�X�N�̃|�C���^)
void CTaskManager::Add(CTask* addTask) 
{
	//mTail�̑O�ɂɒǉ�
	CTask* task = &mTail;
	//addTask�̎���task
	addTask->mpNext = task;
	//addTask�̑O�̎���addTask��
	addTask->mpPrev = task->mpPrev;
	//addTask�̑O�̎���addTask��
	addTask->mpPrev->mpNext = addTask;
	//task�̑O��addTask��
	task->mpPrev = addTask;
}
//�X�V
void CTaskManager::Update() {
	//�擪����ŏI�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//�X�V�������Ă�
		task->Update();
		//����
		task = task->mpNext;
	}
}
//�`��
void CTaskManager::Render() {
	//�擪����ŏI�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//�X�V�������Ă�
		task->Render();
		//����
		task = task->mpNext;
	}
}

void CTaskManager::Remove(CTask* task) {
	//�^�X�N�̑O�̎����A�^�X�N�̎��ɂ���
	task->mpPrev->mpNext = task->mpNext;
	//�^�X�N�̎��̑O���A�^�X�N�̑O�ɂ���
	task->mpNext->mpPrev = task->mpPrev;
}

void CTaskManager::Delete() {
	//�擪����Ō�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		CTask* del = task;
		//����
		task = task->mpNext;
		//mEnabled��false�Ȃ�폜
		if (del->mEnabled == false) {
			delete del;
		}
	}
}
