#ifndef CENEMY_H
#define CENEMY_H

//キャラクタークラスのインクルード
#include "CCharacter3.h"
#include "CCollider.h"
#include "CCollisionManager.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CCharacter3 {
public:  
	//コンストラスタ
	CEnemy();
	//CEnemy(位置、回転、拡縮)
	CEnemy(const CVector& position,
		const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	void Collision();
	//Collision(コライダ１,コライダ２)
	void Collision(CCollider* m, CCollider* o);

private:
	//コライダ
	CCollider mCollider;
	static CModel sModel;
};

#endif // !CENEMY_H
