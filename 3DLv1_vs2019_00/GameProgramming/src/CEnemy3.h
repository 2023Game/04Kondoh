#ifndef CENEMY3_H
#define CENEMY3_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
//コライダクラスのインクルード
#include "CCollider.h"
#include "CCollisionManager.h"
/*
エネミークラス
キャラクタクラスを伝承
*/
class CEnemy3 : public CCharacter3 {
public:
	//コンストラクタ
	CEnemy3();
	//CEnemy3(位置、回転、拡縮)
	CEnemy3(const CVector& position, const CVector& rotation,
		const CVector& scale);
	//更新
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２)
	void Collision(CCollider* m, CCollider* o);
	void Collision();
private:
	int mHp;  //ヒットポイント
	//モデルデータ
	static CModel sModel;
	//コライダ
	CCollider mCollider;

};

#endif // !CENEMY3_H

