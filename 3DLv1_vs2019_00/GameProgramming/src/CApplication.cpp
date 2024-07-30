#include "CApplication.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CCamera.h"


void CApplication::Start()
{
	//3Dファイルの読み込み
	mModelX.Load(MODEL_FILE);
	//キャラクターにモデルを設定
	mXPlayer.Init(&mModelX);
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{
	////アニメーションに切り替える
	//if (mCharacter.IsAnimationFinished()) {
	//	int ai = mCharacter.AnimationIndex() + 1;
	//	ai %= mModelX.AnimationSet().size();
	//	mCharacter.ChangeAnimation(ai, true, 60);
	//}
	////キャラクタークラスの更新
	//mCharacter.Update(CMatrix());

	mXPlayer.Update();

	//カメラのパラメーターを作成する
	CVector e, c, u; //視点、注視点、上方向
	//視点を求める
	e = CVector(1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);
	//カメラの設定
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//X軸＋回転
	if (mInput.Key('K')) {
		mMatrix = mMatrix * CMatrix().RotateX(1);
	}
	//X軸－回転
	if (mInput.Key('I')) {
		mMatrix = mMatrix * CMatrix().RotateX(-1);
	}
	//Y軸＋回転
	if (mInput.Key('L')) {
		mMatrix = mMatrix * CMatrix().RotateY(1);
	}
	//Y軸－回転
	if (mInput.Key('J')) {
		mMatrix = mMatrix * CMatrix().RotateY(-1);
	}
	//行列設定
	glMultMatrixf(mMatrix.M());

	//頂点にアニメーションを適用する
	mModelX.AnimateVertex();
	//モデル描画
//	mModelX.Render();
	mXPlayer.Render();

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D RPOGRAMMING");

	//2Dの描画終了
	CCamera::End();
}

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