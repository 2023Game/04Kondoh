#ifndef CTASKMANAGER_H
#define CTASKMANAGER_H
//タスククラスのインクルード
#include "CTask.h"
#include "CCollider.h"

/*
タスクマネージャ
タスクリストの管理
*/
class CTaskManager {
public:
	//衝突処理
	void Collision();
	//インスタンスの取得
	static CTaskManager* Instance();
	//タスクの削除
	void Delete();
	//リストから削除
	//Remove(タスクポイント)
	void Remove(CTask* task);
	//デストラクタ
	virtual ~CTaskManager();
	//リスト追加
	//Add(タスクのポイント)
	void Add(CTask* addTask);
	//更新
	void Update();
	//描画
	void Render();

protected:
	//デフォルトコンストラクタ
	CTaskManager();
	CTask mHead; //先頭タスク
	CTask mTail; //最終タスク

private:
	//タスクマネージャのインスタンス
	static CTaskManager* mpInstance;
};


#endif // !CTASKMANAGER_H

