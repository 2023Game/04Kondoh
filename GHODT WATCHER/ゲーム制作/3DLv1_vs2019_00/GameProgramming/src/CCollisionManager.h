#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H
//衝突処理範囲より離れているコライダは衝突処理しない
#define COLLISIONRANGE 200

#include "CTaskManager.h"

class CCollisionManager : public CTaskManager
{
public:
	//衝突処理
    //Collision(コライダ,範囲)
    //コライダの優先度＋範囲～優先度-範囲までのコライダと衝突判定を実行する
	void CCollisionManager::Collision(CCollider* c, int range);
	//衝突処理
	void Collision();
	//インスタンスの取得
	static CCollisionManager* Instance();

private:
	//マネージャのインスタンス
	static CCollisionManager* mpInstance;

};

#endif // !CCOLLISIONMSNSGER_H
