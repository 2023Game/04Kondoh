#include "CApplication.h"
#include "CRectangle.h"
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include <math.h>

//クラスのstatic変数
CTexture CApplication::mTexture;
CCharacterManager CApplication::mCharacterManager;

#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル
//f14モデル
#define MODEL_OBJ "res\\BoxMan_No3.obj","res\\BoxMan_No3.mtl"
//敵輸送機モデル
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"
//背景モデルデータの指定
#define MODEL_BACKGROUND "res\\StageNo_09.obj","res\\StageNo_09.mtl"

#define MOS_POS_X 400  //マウス座標のX補正
#define MOS_POS_Y 300  //マウス座標のY補正
#define MOUSE_X_LIN 1910  //マウス座標の範囲
#define MOUSE_Y_LIN 1070   //マウス座標の範囲

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	/*
	mColliderTriangle2.Set(nullptr, nullptr
		, CVector(50.0f, 0.0f, 50.0f)
		, CVector(50.0f, 0.0f, -50.0f)
		, CVector(-50.0f, 0.0f, 50.0f));
	//三角コライダの作成
	mColliderTriangle.Set(nullptr, nullptr
		, CVector(-50.0f, 0.0f, -50.0f)
		, CVector(-50.0f, 0.0f, 50.0f)
		, CVector(50.0f, 0.0f, -50.0f));
　　*/

//	new CEnemy3(CVector(-5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
//	new CEnemy3(CVector(5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
//	new CEnemy(&mModelC5, CVector(30.0f, 10.0f, -130.0f),
//		CVector(), CVector(0.1f, 0.1f, 0.1f));
	//敵機のインスタンス作成
	new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -100.0f),
		CVector(), CVector(0.1f, 0.1f, 0.1f));
	//C5モデルの読み込み
	//mModelC5.Load(MODEL_C5);
	mEye = CVector(1.0f, 2.0f, 3.0f);
	mModel.Load(MODEL_OBJ);
	mModelBG.Load(MODEL_BACKGROUND);
	CMatrix matrix;
	matrix.Print();
	//mCharacter.Model(&mModel);
	//mCharacter.Scale(CVector(0.1f, 0.1f, 0.1f));
	mBackGround.Model(&mModelBG);
	mBackGround.Scale(CVector(2.5f, 2.5f, 2.5f));
	mPlayer.Model(&mModel);
	mPlayer.Position(CVector(11.0f, -3.5f, -53.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));
	mPlayer.Scale(CVector(0.2f, 0.2f, 0.2f));
	//ビルボードの生成
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//背景モデルから三角コライダを生成
    //親インスタンスと親行列
//	mColliderMesh.Set(nullptr, nullptr, &mModelBG);
	mBackGround.ColliderMesh();

	spUi = new CUi();  //UIクラスの生成

	mCamera = ECamera::View1;
	int ViewNumber = 1;
	VX = 0.0f;
	VY = 0.0f;

	CInput::GetMousePosD(&mMx, &mMy);
}

void CApplication::Update()
{
#pragma region [Window]  

	/*
	int mx, my;  //マウスカーソル座標取得用
	//マウスカーソル座標の取得
	CInput::GetMousePos(&mx, &my);
	

	//ゲーム画面中心からの座標へ変換
	mx -= MOS_POS_X;
	my = MOS_POS_Y;

	//距離が遠い方へ移動させる
	if (abs(mx) > abs(my))
	{
		//X軸で移動
		if (mx < 0)
		{   //左へ移動
			VY += 5.0f;
		}
		else
		{   //右へ移動
			VY -= 5.0f;
		}
	}
	else
	{
		//Y軸で移動
		if (my < 0)
		{   //下へ移動
			VX -= 5.0f;
		}
		else
		{   //上へ移動
			VX += 5.0f;
		}
	}
	*/

#pragma endregion 

#pragma region [DisPlay]  
	int mx, my;  //マウスカーソル座標取得用
	//マウスカーソル座標の取得
	CInput::GetMousePosD(&mx, &my);

	//X軸方向の移動
	//前回より右であれば右へ移動
	if (mMx + 2 < mx)
	{
		VY -= 4.5f;
		mPlayer.Rotation(CVector(0.0f, VY, 0.0f));
	}
	else if (mMx - 2 > mx)
	{
		VY += 4.5f;
		mPlayer.Rotation(CVector(0.0f, VY, 0.0f));
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




	//タスクマネージャーの更新
	CTaskManager::Instance()->Update();
	//コリジョンマネージャの衝突処理
//	CCollisionManager::Instance()->Collision();
	CTaskManager::Instance()->Collision();

	//カメラのパラメーターを作成する
	CVector e, c, u; //視点,注視点,上方向

#pragma region [mCamera]

	switch (mCamera)
	{
	case ECamera::View1:

		//視点移動（横）
		if (mInput.Key(VK_RIGHT))
		{
			VY = VY + 5.0f;
			mPlayer.Rotation(CVector(0.0f, VY, 0.0f));
		}
		else if (mInput.Key(VK_LEFT))
		{
			VY = VY - 5.0f;
			mPlayer.Rotation(CVector(0.0f, VY, 0.0f));
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
		e = mPlayer.Position() + CVector(0.0f, 0.7f, 0.0f);
		//注視点を求める
		c = mPlayer.Position() + CVector(0.0f, 3.0f, 10.0f) * CMatrix().RotateX(VX) * CMatrix().RotateY(VY);
		//上方向を求める
		u = CVector(0.0f, 1.0f, 0.0f);

		if (VX > 91)
		{
			VX = 90;
		}
		else if (VX < -46)
		{
			VX = -45;
		}

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
		//視点の設置
		//gluLookAt(視点X,視点Y,視点Z,中心X,中心Y,中心Z,上向X,上向Y,上向Z)
		gluLookAt(mEye.X(), mEye.Y(), mEye.Z(), mEye.X(), mEye.Y(), mEye.Z(), 0.0f, 1.0f, 0.0f);

		if (mInput.Key('J'))
		{
			mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
		}
		if (mInput.Key('L'))
		{
			mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
		}

		if (mInput.Key('I'))
		{
			mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
		}
		if (mInput.Key('K'))
		{
			mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
		}

		if (mInput.Key('M'))
		{
			mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
		}
		if (mInput.Key('O'))
		{
			mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
		}

		if (mInput.Key('T'))
		{
			ViewNumber = 1;
		}
		else if (mInput.Key('P'))
		{
			ViewNumber = 3;
		}
		break;
	case ECamera::View3:
		//視点を求める
		e = mPlayer.Position() + CVector(0.0f, 0.0f, -3.0f) * mPlayer.MatrixRotate();
		//注視点を求める
		c = mPlayer.Position();
		//上方向を求める
		u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
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

	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

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

#pragma endregion

	//mBackGround.Render();
	//タスクリストの削除
	CTaskManager::Instance()->Delete();
	//タスクマネージャーの描画
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();

	spUi->Render();   //UIの描画
}
/*
CTaskManager CApplication::mTaskManager;
CTaskManager* CApplication::TaskManager()
{
	return &mTaskManager;
}
*/

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi;   //インスタンスのポインタを返す
}

CApplication::~CApplication()
{
	delete spUi;   //インスタンスUiの削除
}