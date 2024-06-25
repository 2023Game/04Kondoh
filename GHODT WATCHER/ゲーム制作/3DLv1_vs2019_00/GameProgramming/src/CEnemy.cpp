#include "CEnemy.h"
#include "CEffect.h"

#define OBJ "res\\BoxMan_No3.obj"
#define MTL "res\\BoxMan_No3.mtl"
//移動速度
#define VELOCITY CVector(0.0f,0.0f,0.09f)

CModel CEnemy::sModel;

//デフォルトコンストラクタ
CEnemy::CEnemy()
	:CCharacter3(1)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
{
	//モデルが無いときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &sModel;
}

//CEnemy(位置、回転、拡縮)
CEnemy::CEnemy(const CVector& position,
	const CVector& rotation, const CVector& scale)
	:CEnemy()
{
	//位置、回転、拡縮
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale;       //拡縮の設定
	CTransform::Update(); //行列の設定
}

//更新処理
void CEnemy::Update() {
	//行列を更新
	CTransform::Update();
	//位置を移動
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}


//衝突処理
//CCollision(コライダ１,コライダ２)
void CEnemy::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::EType::ESPHERE: //球コライダの時
		//コライダのmとoが衝突しているか判定
		if (CCollider::Collision(m, o)) {
			//エフェクト生成
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//衝突している時は無効
			//mEnabled = false;
		}
		break;

	case CCollider::EType::ETRIANGLE: //三角コライダの時
		CVector adjust; //調整値
		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{   //衝突しない位置まで戻す
			mPosition = mPosition + adjust;
		}
		break;
	}
}

//衝突処理
void CEnemy::Collision()
{
	//コライダの優先度
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

