//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CApplication.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //Y軸回転速度
#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //X軸回転速度
#define VELOCITY CVector(0.0f,0.0f,0.1f) //移動速度

CPlayer::CPlayer()
	:mLine(this, &mMatrix, CVector(0.0f, 0.0f, -14.0f), CVector(0.0f, 0.0f, 17.0f))
	,mLine2(this, &mMatrix, CVector(0.0f, 5.0f, -8.0f), CVector(0.0f, -3.0f, -8.0f))
	,mLine3(this, &mMatrix, CVector(9.0f, 0.0f, -8.0f), CVector(-9.0f, 0.0f, -8.0f))
{
	//インスタンスの設定
	spInstance = this;
}

//CPlayer(位置、回転、スケール)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale); //行列の更新
}

//更新処理
void CPlayer::Update() {
	//Dキー入力で回転
	if (mInput.Key('D')) {
		//Y軸の回転値を減少
		mRotation = mRotation - ROTATION_YV;
	}
	//Aキー入力で回転
	if (mInput.Key('A')) {
		//Y軸の回転値を増加
		mRotation = mRotation + ROTATION_YV;
	}

	//Sキー入力で上向き
	if (mInput.Key('S')) {
		//X軸の回転値を減少
		mRotation = mRotation - ROTATION_XV;
	}
	//Wキー入力で下向き
	if (mInput.Key('W')) {
		//X軸の回転値を加算
		mRotation = mRotation + ROTATION_XV;
	}

	//上キー入力で前進
	if (mInput.Key(VK_UP)) {
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}

	//スペースキー入力で弾発射
	if (mInput.Key(VK_SPACE)) {
		CBullet* bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet->Rotation(mRotation);
		bullet->Update();
		//CApplication::TaskManager()->Add(bullet);
	}

	//変換行列の更新
	CTransform::Update();

	//UI設定
	CApplication::Ui()->PosY(mPosition.Y());
	CApplication::Ui()->RotX(mRotation.X());
	CApplication::Ui()->RotY(mRotation.Y());
}

void CPlayer::Collision(CCollider* m, CCollider* o) {
	//自身のコライダタイプの判定
	switch (m->Type()) {
	case CCollider::EType::ELINE:
		//相手のコライダが三角形コライダの時
		if (o->Type() == CCollider::EType::ETRIANGLE) {
			CVector adjust; //調整用ベクトル
			//三角形と線分の衝突判定
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//位置更新
				mPosition = mPosition + adjust;
				//行列更新
				CTransform::Update();
			}
		}
		break;
	}
}

//衝突処理
void CPlayer::Collision()
{
	//コライダの優先度変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}

CPlayer* CPlayer::spInstance = nullptr;

CPlayer* CPlayer::Instance()
{
	return spInstance;
}
