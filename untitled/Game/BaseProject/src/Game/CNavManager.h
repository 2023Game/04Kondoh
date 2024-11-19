#pragma once
#include <list>
#include <vector>
#include "CTask.h"

class CNavNode;

// 経路探索処理の管理クラス
class CNavManager : public CTask
{
public:
	// 経路管理クラスのインスタンスを取得
	static CNavManager* Instance();

	// コンストラクタ
	CNavManager();
	// デストラクタ
	~CNavManager();

	// 経路探索用のノードを追加
	void AddNode(CNavNode* node);
	// 経路探索用のノードを取り除く
	void RemoveNode(CNavNode* node);

	// 全てのノードと経路を描画
	void Render() override;

private:
	// 経路管理クラスのインスタンスへのポインタ
	static CNavManager* spInstance;

	// 経路探索用ノードのリスト
	std::list<CNavNode*> mNodes;

	// 経路探索のデバッグ表示を行うかどうか
	bool mIsRender;
};