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
	//衝突処理
	void Collision();
	//衝突処理
	//Collision(コライダ１,コライダ２)
	void Collision(CCollider* m, CCollider* o);
	//コンストラスタ
	//CEnemy(モデル、位置、回転、拡縮)
	CEnemy(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();

private:
	//コライダ
	CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;
};

#endif // !CENEMY_H
