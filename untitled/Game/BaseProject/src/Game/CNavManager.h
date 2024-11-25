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

	/// <summary>
	/// 指定したノードに接続できるノードを検索して設定
	/// （間に遮蔽物があるノードには接続できない）
	/// </summary>
	/// <param name="node">接続できるノードを検索するノード</param>
	/// <param name="distance">接続できる距離の限界値</param>
	/// <returns>接続したノードの数</returns>
	int FindConnectNavNode(CNavNode* node, float distance);

	/// <summary>
	/// 指定したノードから目的地までの最短経路を求める
	/// </summary>
	/// <param name="start">開始ノード</param>
	/// <param name="goal">目的地ノード</param>
	/// <param name="route">最短経路返却用</param>
	/// <returns>経路が繋がっている場合は、trueを返す</returns>
	bool Navigate(CNavNode* start, CNavNode* goal, std::vector<CNavNode*>& route);

	// 全てのノードと経路を描画
	void Render() override;

private:
	// 最短経路計算用のデータをリセット
	void ResetCalcData();
	/// <summary>
	/// 指定したノードから次のノードへの移動コストを計算
	/// </summary>
	/// <param name="node">移動コストを計算する開始ノード</param>
	/// <param name="goal">目的地ノード</param>
	void CalcNextMoveCost(CNavNode* node, CNavNode* goal);

	// 経路管理クラスのインスタンスへのポインタ
	static CNavManager* spInstance;

	// 経路探索用ノードのリスト
	std::list<CNavNode*> mNodes;

	// 経路探索のデバッグ表示を行うかどうか
	bool mIsRender;
	// 最後に計算した最短経路のデバッグ表示
	std::vector<CNavNode*> mLastCalcRoute;
};