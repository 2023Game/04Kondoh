#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include <assert.h>

CNavManager* CNavManager::spInstance = nullptr;

// �o�H�Ǘ��N���X�̃C���X�^���X���擾
CNavManager* CNavManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CNavManager::CNavManager()
	:CTask(ETaskPriority::eNavigation,0,ETaskPauseType::eGame)
	,mIsRender(false)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// �f�X�g���N�^
CNavManager::~CNavManager()
{
	spInstance = nullptr;

	// �T���m�[�h��S�č폜
	auto itr = mNodes.begin();
	auto end = mNodes.end();
	while (itr != end)
	{
		CNavNode* del = *itr;
		itr = mNodes.erase(itr);
		delete del;
	}
}

// �o�H�T���p�̃m�[�h��ǉ�
void CNavManager::AddNode(CNavNode* node)
{
}

// �o�H�T���p�̃m�[�h����菜��
void CNavManager::RemoveNode(CNavNode* node)
{
}

// �S�Ẵm�[�h�ƌo�H��`��
void CNavManager::Render()
{
}