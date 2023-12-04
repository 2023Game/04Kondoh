#include "CEnemy.h"
#include "CEffect.h"

//移動速度
#define VELOCITY CVector(0.0f,0.0f,0.09f)

//コンストラクタ
//CEnemy(モデル、位置、回転、拡縮)
CEnemy::CEnemy(CModel* model, const CVector& position,
	const CVector& rotation, const CVector& scale)
	:mCollider1(this,&mMatrix,CVector(0.0f,5.0f,0.0f),0.8f)
	,mCollider2(this, &mMatrix, CVector(0.0f, 5.0f, 20.0f), 0.8f)
	,mCollider3(this, &mMatrix, CVector(0.0f, 5.0f, -20.0f), 0.8f)
{
	//モデル、位置、回転、拡縮
	mpModel = model;      //モデルの設定
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale;       //拡縮の設定
}

//更新処理
void CEnemy::Update() {
	//行列を更新
	CTransform::Update();
	//位置を移動
	mPosition = mPosition + VELOCITY * mMatrixRotate;
}

//衝突処理
//CCollision(コライダ１,コライダ２)
void CEnemy::Collision(CCollider* m, CCollider* o) {
	//コライダのmとoが衝突しているか判定
	if (CCollider::Collision(m, o)) {
		//エフェクト生成
		new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		//衝突している時は無効
		//mEnabled = false;
	}
}

