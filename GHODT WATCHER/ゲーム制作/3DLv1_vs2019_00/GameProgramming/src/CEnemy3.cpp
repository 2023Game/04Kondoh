#include "CEnemy3.h"
#include "CEffect.h"
#include "CPlayer.h"

#define OBJ "res\\BoxMan_No3.obj"
#define MTL "res\\BoxMan_No3.mtl"
#define HP 3 //耐久値
#define VELOCITY 0.11f  //速度

CModel CEnemy3::sModel;

//デフォルトコンストラクタ
CEnemy3::CEnemy3()
	: CCharacter3(1)
//	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
	,mHp(HP)
{
	//モデルが無いときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &sModel;
	//目標地点の設定
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
}

//コンストラクタ
//CEnemy(位置、回転、拡縮)
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CEnemy3()
{
	//位置、回転、拡縮を設定する
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale;       //拡縮の設定
	CTransform::Update(); //行列の設定
}

//更新処理
void CEnemy3::Update()
{
	//プレイヤーのポインタが０以外の時
	CPlayer *player = CPlayer::Instance();
	if (player != nullptr)
	{
		//プレイヤーまでのベクトルを求める
		CVector vp = player->Position() - mPosition;
		//左ベクトルとの内積を求める
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//上ベクトルとの内積を求める
		float dy = vp.Dot(mMatrixRotate.VectorY());
		//前ベクトルとの内積を求める
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		//X軸のズレが2.0以下
		if (-2.0f < dx && dx < 2.0f)
		{
			//Y軸のズレが2.0以下
			if (-2.0f < dy && dy < 2.0f)
			{
				if (0.0f < dz && dz < 30.0f)
				{
					//弾を発射します
					CBullet* bullet = new CBullet();
					bullet->Set(0.1f, 1.5f);
					bullet->Position(
						CVector(0.0f, 0.0f, 10.0f) * mMatrix);
					bullet->Rotation(mRotation);
					bullet->Update();
				}
			}
		}
	}
	//HPが0以下の時　撃破
	if (mHp <= 0)
	{
		mHp--;
		//HPが0以下の時
		if (mHp % 15 == 0)
		{
			//15フレーム毎にエフェクト
			new CEffect(mPosition, 1.0f, 1.0f, "exe.tga", 4, 4, 2);
		}
		//降下させる
		mPosition = mPosition - CVector(0.0f, 0.03f, 0.0f);
		CTransform::Update();
		return;
	}

	//目標地点までのベクトルを求める
	CVector vp = mPoint - mPosition;
	//左ベクトルとの内積を求める
	float dx = vp.Dot(mMatrixRotate.VectorX());
	//上ベクトルとの内積を求める
	float dy = vp.Dot(mMatrixRotate.VectorY());
	const float margin = 0.1f;
	//左右方向へ回転
	if (dx > margin)
	{
		//左へ回転
		mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);
	}
	else if (dx < -margin)
	{
		//右へ回転
		mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f);
	}
	//上下方向へ回転
	if (dy > margin)
	{
		//上へ回転
		mRotation = mRotation + CVector(-1.0f, 0.0f, 0.0f);
	}
	else if (dy < -margin)
	{
		//下へ回転
		mRotation = mRotation + CVector(1.0f, 0.0f, 0.0f);
	}

	//機体前方へ移動する
	mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY;
	CTransform::Update(); //行列更新

	//およそ3秒毎に目標地点を更新
	int r = rand() % 180; //rand()は整数の乱数を返す
	                      //％ 180 は180で割った余りを求める
	if (r == 0)
	{
		if (player != nullptr)
		{
			mPoint = player->Position();
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}
}

/*

//衝突処理
//Collision(コライダ１、コライダ２)
void CEnemy3::Collision(CCollider* m, CCollider* o)
{
	switch (o->Type())
	{
	case CCollider::EType::ESPHERE: //球コライダの時
		if (CCollider::Collision(m, o)) {

			mHp--; //ヒットポイントの減算
			//エフェクト生成
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//衝突している時は無効にする
			//mEnabled = false;
		}
		break;

	case CCollider::EType::ETRIANGLE: //三角コライダの時
		CVector adjust; //調整値
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//撃破で地面に衝突すると無効
			if (mHp <= 0)
			{
				mEnabled = false;
			}
			//衝突しない位置まで戻す
			mPosition = mPosition + adjust;
		}
		break;

	}
}

void CEnemy3::Collision()
{
	mCollider.ChangePriority();

	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}
*/





