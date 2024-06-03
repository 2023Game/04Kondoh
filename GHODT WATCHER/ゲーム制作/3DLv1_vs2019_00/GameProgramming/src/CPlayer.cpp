//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CApplication.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //Y軸回転速度
#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //X軸回転速度
#define VELOCITYZ CVector(0.0f,0.0f,0.2f)  //移動速度(前後)
#define VELOCITYX CVector(0.2f,0.0f,0.0f)  //移動速度(左右)
#define VELOCITYY CVector(0.0f,0.2f,0.0f)  //ジャンプ初速
#define GRAVITY 0.05    //重力

CPlayer::CPlayer()
	:mLine(this, &mMatrix, CVector(0.0f, 0.0f, 1.0f), CVector(0.0f, 0.0f, -1.0f))
	, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -1.3f, 0.0f))
	, mLine3(this, &mMatrix, CVector(1.5f, 0.0f, 0.0f), CVector(-1.5f, 0.0f, 0.0f))
	, mLine4(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, -1.2f, 0.0f))
	,JumpV(0)
{
	//インスタンスの設定
	spInstance = this;
	//プレイヤー行動ステート::待機
	mPState = EPState::EMOVE;
}

//CPlayer(位置、回転、スケール)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale); //行列の更新
}

//更新処理
void CPlayer::Update() {

	switch (mPState) {
	case EPState::EIDEL:

		break;

	case EPState::EMOVE:

		//左キー入力で左に進む
		if (mInput.Key('A')) {
			//Y軸の回転値を減少
			mPosition = mPosition + VELOCITYX * mMatrixRotate;
		}
		//右キー入力で右に進む
		if (mInput.Key('D')) {
			//Y軸の回転値を増加
			mPosition = mPosition - VELOCITYX * mMatrixRotate;
		}

		//上キー入力で前進
		if (mInput.Key('W')) {
			//Z軸方向の値を回転させ移動させる
			mPosition = mPosition + VELOCITYZ * mMatrixRotate;
		}
		//下キー入力で後退
		if (mInput.Key('S'))
		{
			mPosition = mPosition - VELOCITYZ * mMatrixRotate;
		}
		
		//スペースキー入力でジャンプ
		if (mInput.Key(VK_SPACE)) {
			JumpV = 0.5;
			mPState = EPState::EJUMP;
		}

		break;

	case EPState::EJUMP:
		mPosition = mPosition + CVector(0.0f, JumpV, 0.0f);
		JumpV -= GRAVITY;

	}
	//JumpV -= GRAVITY;
	//mPosition = mPosition + CVector(0.0f, JumpV, 0.0f);

	//変換行列の更新
	CTransform::Update();
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
				mPState = EPState::EMOVE;
				JumpV = 0;
				//行列更新
				CTransform::Update();
			}
			if (CCollider::CollisionTriangleLine(o, &mLine4, &adjust))
			{
				//位置更新
				mPosition = mPosition + adjust;
				mPState = EPState::EMOVE;
				JumpV = 0;
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
	mLine4.ChangePriority();
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();

	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mLine4, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}

CPlayer* CPlayer::spInstance = nullptr;

CPlayer* CPlayer::Instance()
{
	return spInstance;
}
