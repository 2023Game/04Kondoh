#ifndef CENEMY_H
#define CENEMY_H

//キャラクタークラスのインクルード
#include "CCharacter3.h"
#include "CCollider.h"
#include "CColliderLine.h"
#include "CColliderMesh.h"
#include "CCollisionManager.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CCharacter3 {
public:  
	enum class EneState  //プレイヤー行動ステート
	{
		EIDEL,   //待機
		EWANING, //警戒
		ECHASE,  //追跡
		ERETUM   //戻る
	};
	EneState mEneState;
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

	float JumpV;  //ジャンプ変数
//	bool 

private:

	CCollider mSphere;  //球コライダ

	CColliderLine mELine;  //線分コライダ
	CColliderLine mELine2;
	CColliderLine mELine3;
	CColliderLine mELine4;

	static CModel sModel;
};

#endif // !CENEMY_H
