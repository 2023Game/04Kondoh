#include "CEnemy.h"
#include "CEffect.h"
#include "CPlayer.h"

#define OBJ "res\\BoxMan_No3.obj"
#define MTL "res\\BoxMan_No3.mtl"
//移動速度
#define VELOCITY CVector(0.0f,0.0f,0.09f)

CModel CEnemy::sModel;

//デフォルトコンストラクタ
CEnemy::CEnemy()
	:CCharacter3(1)
	, mELine(this, &mMatrix, CVector(0.0f, 0.0f, 1.5f), CVector(0.0f, 0.0f, -1.5f))
	, mELine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -1.1f, 0.0f))
	, mELine3(this, &mMatrix, CVector(2.0f, 0.5f, 0.0f), CVector(-2.0f, 0.5f, 0.0f))
	, mELine4(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), CVector(0.0f, -1.1f, 0.0f))
	, mSphere(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), 0.5)
	, JumpV(0.0)
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
	switch (mEneState)
	{
	case EneState::EIDEL:


		Rotation(CVector(0.0f, 0.0f, 0.0f));

		break;
	}

}


//衝突処理
//CCollision(コライダ１,コライダ２)
void CEnemy::Collision(CCollider* m, CCollider* o) {

	//自身のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::EType::ESPHERE: //線コライダの時
		//相手のコライダが三角形コライダの時
		if (o->Type() == CCollider::EType::ELINE) {
			CVector adjust; //調整用ベクトル

			//コライダのmとoが衝突しているか判定
			if (CCollider::Collision(&mSphere, &CPlayer::Instance()->mPLine)) {
				mPosition = mPosition + CVector(0.0f, 0.0f, 10.0f);
			}
			/*
			if (CCollider::CollisionTriangleSphere(&CPlayer::Instance()->mPLine, m, &adjust) && JumpV != 0)
			{
				JumpV = 0;
				//CTransform::Update();
			}

			
			//三角形と線分の衝突判定
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//位置更新
				mPosition = mPosition + adjust;
				//行列更新
				CTransform::Update();
			}
			*/
			
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

	//コライダの優先度変更
	mELine4.ChangePriority();
	mELine.ChangePriority();
	mELine2.ChangePriority();
	mELine3.ChangePriority();
	mSphere.ChangePriority();

	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mELine4, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mELine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mELine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mELine3, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mSphere, COLLISIONRANGE);

	////コライダの優先度
	//mCollider.ChangePriority();
	////衝突処理を実行
	//CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

