#include "CCamera.h"
#include "glut.h"
#include "CApplication.h"

#define MOUSE_X_LIN 1910  //マウス座標の範囲
#define MOUSE_Y_LIN 1070   //マウス座標の範囲
#define PRIORITY 0  //優先度


void CCamera::Start(double left, double right, double bottom, double top)
{
	//モデルビュー行列の退避
	glPushMatrix();
	//モデルビュー行列の初期化
	glLoadIdentity();
	//プロジェクション行列へ切り替え
	glMatrixMode(GL_PROJECTION);
	//プロジェクション行列の退避
	glPushMatrix();
	//プロジェクション行列の初期化
	glLoadIdentity();
	//表示エリアの設定
	gluOrtho2D(left, right, bottom, top);
	//Depthテストオフ
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
}

CCamera::CCamera()
	:ViewNumber(1)
	, VX(0.0f)
	, VY(0.0f)
	, CZ(10.0f)
	, mEye(1.0f, 2.0f, 3.0f)
	, mLine(this, &mMatrix, CVector(0.0f, -0.1f, 0.0f), CVector(0.0f, -0.1f, 10.0f))
	, mCollider(this, &mMatrix, CVector(), 1.5f)
{
	mCamera = ECamera::View1;
	CInput::GetMousePosD(&mMx, &mMy);
	//mScale = CVector(0.1f, 0.1f, 0.1f);
	//インスタンスの設定
	spInstance = this;
}

//CPlayer(位置、回転、スケール)
CCamera::CCamera(const CVector& pos, const CVector& rot, const CVector& scale)
	:CCamera()
{
	CTransform::Update(pos, rot, scale); //行列の更新
}

void CCamera::Update() {

	mpPlayer = CPlayer::Instance();
	mPosition = mpPlayer->Position() + CVector(0.0f, 1.1f, 0.0f);
	mRotation = CVector(VX, VY, 0.0f);


#pragma region [DisPlay]  
	int mx, my;  //マウスカーソル座標取得用
	//マウスカーソル座標の取得
	CInput::GetMousePosD(&mx, &my);

	//X軸方向の移動
	//前回より右であれば右へ移動
	if (mMx + 2 < mx)
	{
		VY -= 4.5f;
		mpPlayer->Rotation(CVector(0.0f, VY, 0.0f));
	}
	else if (mMx - 2 > mx)
	{
		VY += 4.5f;
		mpPlayer->Rotation(CVector(0.0f, VY, 0.0f));
	}

	//Y軸方向の移動
	//前回より下にあれば下へ移動
	if (mMy + 2 < my)
	{
		VX += 4.5f;
	}
	else if (mMy - 2 > my)
	{
		VX -= 4.5f;
	}

	//左右の範囲を出ると反対端へ移動させる
	if (mx >= MOUSE_X_LIN)
	{
		mx = 1;
	}
	else if (mx <= 0)
	{
		mx = MOUSE_X_LIN - 1;
	}

	//上下の範囲を出ると反対端へ移動させる
	if (my >= MOUSE_Y_LIN)
	{
		my = 1;
	}
	else if (my <= 0)
	{
		my = MOUSE_Y_LIN - 1;
	}

	//座標を退避する
	mMx = mx;
	mMy = my;

	//マウスカーソル座標の更新
	CInput::SetMousePosD(mx, my);

#pragma endregion 

	if (VX > 91)
	{
		VX = 90;
	}
	else if (VX < -46)
	{
		VX = -45;
	}


	if (ViewNumber == 1)
	{
		mCamera = ECamera::View1;
	}
	else if (ViewNumber == 2)
	{
		mCamera = ECamera::View2;
	}
	else if (ViewNumber == 3)
	{
		mCamera = ECamera::View3;
	}

	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	CTransform::Update();
}

CMatrix CCamera::mModelViewInverse;

const CMatrix& CCamera::ModelViewInverse()
{
	return mModelViewInverse;
}

CCamera* CCamera::spInstance = nullptr;

CCamera* CCamera::Instance() 
{ 
	return spInstance;
}


//衝突処理
//Collision(コライダ１,コライダ２)
void CCamera::Collision(CCollider* m, CCollider* o) {
	//自身のコライダタイプの判定
	switch (m->Type()) {
	case CCollider::EType::ELINE:
		//相手のコライダが三角形コライダの時
		if (o->Type() == CCollider::EType::ETRIANGLE) {
			CVector adjust; //調整用ベクトル
			//三角形と線分の衝突判定
			//if (CCollider::CollisionTriangleLine(o, m, &adjust))
			//{
			//}
		}
		break;
	case CCollider::EType::ESPHERE:
		if (o->Type() == CCollider::EType::ETRIANGLE) {
			CVector adjust; //調整用ベクトル
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
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
void CCamera::Collision()
{
	//線分コライダの優先度変更
	mLine.ChangePriority();
	//球コライダの優先度変更
	mCollider.ChangePriority();

	//線分コライダ衝突処理を実行
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	//球コライダー衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

void CCamera::Render()
{

#pragma region [mCamera]

	//カメラのパラメーターを作成する
	//e, c, u; //視点,注視点,上方向

	switch (mCamera)
	{
	case ECamera::View1:

		//視点移動（横）
		if (mInput.Key(VK_RIGHT))
		{
			VY = VY + 5.0f;
			mpPlayer->Rotation(CVector(0.0f, VY, 0.0f));
		}
		else if (mInput.Key(VK_LEFT))
		{
			VY = VY - 5.0f;
			mpPlayer->Rotation(CVector(0.0f, VY, 0.0f));
		}
		else if (mInput.Key(VK_DOWN))
		{
			VX = VX - 5.0f;
		}
		else if (mInput.Key(VK_UP))
		{
			VX = VX + 5.0f;
		}

		//視点を求める
		e = mPosition;
		//注視点を求める
		c = mPosition + CVector(0.0f, -1.0f, CZ) * CMatrix().RotateX(VX) * CMatrix().RotateY(VY);
		//上方向を求める
		u = CVector(0.0f, 1.0f, 0.0f);

		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

		if (mInput.Key('Y'))
		{
			ViewNumber = 2;
		}
		else if (mInput.Key('P'))
		{
			ViewNumber = 3;
		}

		break;

	case ECamera::View2:
		break;
	case ECamera::View3:

		//視点移動（横）
		if (mInput.Key(VK_RIGHT))
		{
			VY = VY + 5.0f;
		}
		else if (mInput.Key(VK_LEFT))
		{
			VY = VY - 5.0f;
		}
		else if (mInput.Key(VK_DOWN))
		{
			VX = VX - 5.0f;
		}
		else if (mInput.Key(VK_UP))
		{
			VX = VX + 5.0f;
		}

		//視点を求める
		e = mpPlayer->Position() + CVector(0.0f, 0.0f, -3.0f) * mpPlayer->MatrixRotate() * CMatrix().RotateX(VX) * CMatrix().RotateY(VY);
		//注視点を求める
		c = mpPlayer->Position();
		//上方向を求める
		u = CVector(0.0f, 1.0f, 0.0f) * mpPlayer->MatrixRotate();

		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

		if (mInput.Key('T'))
		{
			ViewNumber = 1;
		}
		else if (mInput.Key('Y'))
		{
			ViewNumber = 2;
		}
		break;
	}
#pragma endregion

	CTransform::Update();

}

CMatrix CCamera::CameraMatrix()
{
	return mMatrix;
}

