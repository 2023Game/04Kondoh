//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CApplication.h"
#include "CCamera.h"
#include "CMatrix.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //Y軸回転速度
#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //X軸回転速度

#define RanZ CVector(0.0f,0.0f,0.2f)  //移動速度(前後)
#define RanX CVector(0.2f,0.0f,0.0f)  //移動速度(左右)
#define JRanZ CVector(0.0f,0.0f,0.1f)  //
#define JRanX CVector(0.1f,0.0f,0.0f)

#define VELOCITYY CVector(0.0f,0.2f,0.0f)  //ジャンプ初速
#define GRAVITY 0.01    //重力

CPlayer::CPlayer()
	: mPLine(this, &mMatrix, CVector(0.0f, 0.0f, 1.5f), CVector(0.0f, 0.0f, -1.5f))
	, mPLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -1.1f, 0.0f))
	, mPLine3(this, &mMatrix, CVector(2.0f, 0.5f, 0.0f), CVector(-2.0f, 0.5f, 0.0f))
	, mPLine4(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), CVector(0.0f, -1.1f, 0.0f))
	, JumpV(0)
	, ShootTime(30)
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

			mPosition = mPosition + RanX * mMatrixRotate;
		}
		//右キー入力で右に進む
		if (mInput.Key('D')) {

			mPosition = mPosition - RanX * mMatrixRotate;
		}

		//上キー入力で前進
		if (mInput.Key('W')) {

			mPosition = mPosition + RanZ * mMatrixRotate;
		}
		//下キー入力で後退
		if (mInput.Key('S'))
		{
			mPosition = mPosition - RanZ * mMatrixRotate;
		}

		
		//スペースキー入力でジャンプ
		if (mInput.Key(VK_SPACE)) {
			JumpV = 0.2;
			mPState = EPState::EJUMP;
		}

		if (mInput.Key(VK_LBUTTON))
		{
			if (ShootTime >= 30)
			{
				CBullet* bullet = new CBullet();
				bullet->Set(0.1f, 1.5f);
				bullet->Position(CVector(0.0f, 0.0f, 1.0f) * CCamera::Instance()->CameraMatrix());
				bullet->Rotation(CCamera::Instance()->Rotation());
				bullet->Update();
				ShootTime = 0;
			}
			ShootTime++;
		}
		else
		{
				ShootTime = 30;
		}

		mPosition = mPosition + CVector(0.0f, JumpV, 0.0f);

		break;

	case EPState::EJUMP:
		mPosition = mPosition + CVector(0.0f, JumpV, 0.0f);
		JumpV -= GRAVITY;
		break;

	case EPState::ESHOOT:
		mPState = EPState::EMOVE;

		break;

	}

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

			if (CCollider::CollisionTriangleLine(o, &mPLine2, &adjust) && JumpV != 0)
			{
				mPState = EPState::EMOVE;
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
		}
		break;
	}
}

//衝突処理
void CPlayer::Collision()
{
	//コライダの優先度変更
	mPLine4.ChangePriority();
	mPLine.ChangePriority();
	mPLine2.ChangePriority();
	mPLine3.ChangePriority();

	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mPLine4, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mPLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mPLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mPLine3, COLLISIONRANGE);

}

CPlayer* CPlayer::spInstance = nullptr;

CPlayer* CPlayer::Instance()
{
	return spInstance;
}
