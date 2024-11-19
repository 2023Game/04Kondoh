#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include <assert.h>

CNavManager* CNavManager::spInstance = nullptr;

// 経路管理クラスのインスタンスを取得
CNavManager* CNavManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CNavManager::CNavManager()
	:CTask(ETaskPriority::eNavigation,0,ETaskPauseType::eGame)
	,mIsRender(false)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// デストラクタ
CNavManager::~CNavManager()
{
	spInstance = nullptr;

	// 探索ノードを全て削除
	auto itr = mNodes.begin();
	auto end = mNodes.end();
	while (itr != end)
	{
		CNavNode* del = *itr;
		itr = mNodes.erase(itr);
		delete del;
	}
}

// 経路探索用のノードを追加
void CNavManager::AddNode(CNavNode* node)
{
}

// 経路探索用のノードを取り除く
void CNavManager::RemoveNode(CNavNode* node)
{
}

// 全てのノードと経路を描画
void CNavManager::Render()
{
}
