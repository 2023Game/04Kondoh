#ifndef CBULLET_H
#define CBULLET_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
//三角形クラスのインクルード
#include "CTriangle.h"
#include "CModel.h"
#include "CCollider.h"
#include "CCollisionManager.h"

/*
弾クラス
三角形を飛ばす
*/

class CBullet : public CCharacter3 {
public:
	float JumpV;
	//衝突処理
	void Collision();
	//衝突処理
	//Collision(コライダ１,コライダ２)
	void Collision(CCollider* m, CCollider* o);
	CBullet();
	//幅と奥行きの設定
	//Set(幅,奥行)
	void Set(float w, float d);
	//更新
	void Update();
	//描画
	void CBullet::Render();

private:
	//生存時間
	int mLife;
	//三角形
	CTriangle mT;
	CCollider mCollider;

};

#endif // !CBULLET_H
