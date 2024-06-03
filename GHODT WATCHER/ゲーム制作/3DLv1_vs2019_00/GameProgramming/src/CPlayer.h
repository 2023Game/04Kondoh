#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
#include "CInput.h"
#include "CBullet.h"
#include "CColliderLine.h"
#include "CCollisionManager.h"

/*
プレイヤークラス
キャラクタクラスを継承
*/

class CPlayer : public CCharacter3 {
public:

	enum class EPState  //プレイヤー行動ステート
	{
		EMOVE,  //移動
		EJUMP,  //ジャンプ
		EIDEL,  //待機
	};
	EPState mPState;
	float JumpV;  //ジャンプ変数

	//インスタンスのポイントの取得
	static CPlayer* Instance();
	//衝突処理
	void Collision();
	//衝突判定
	void Collision(CCollider* m, CCollider* o);
	//CBullet bullet;
	CPlayer();
	//CPlayer(位置、回転、スケール)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//更新処理
	void Update();
private:
	//プレイヤーのインスタンス
	static CPlayer* spInstance; 
	CColliderLine mLine;  //線分コライダ
	CColliderLine mLine2;
	CColliderLine mLine3;
	CColliderLine mLine4;
	CInput mInput;
};

#endif // !CPLAYER_H
