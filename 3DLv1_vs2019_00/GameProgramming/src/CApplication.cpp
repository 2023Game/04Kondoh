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
	mKnight.Load(MODEL_KNIGHT);
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 1:移動
	mKnight.SeparateAnimationSet(0, 1530, 1830, "idle1"); // 2:待機
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 3:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 4:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 5:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 6:ダミー
	mKnight.SeparateAnimationSet(0, 440, 520, "attack1"); // 7:Attack1
	mKnight.SeparateAnimationSet(0, 520, 615, "attack2"); // 8:Attack2
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 9:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 10:ダミー
	mKnight.SeparateAnimationSet(0, 1160, 1260, "death1"); // 11:ダウン


	//キャラクターにモデルを設定
	mXPlayer.Init(&mModelX);
	mFont.Load("FontG.png", 1, 4096 / 64);
	//敵の初期設定
	mXEnemy.Init(&mKnight);
	//敵の配置
	mXEnemy.Position(CVector(7.0f, 0.0f, 0.0f));

	mXEnemy.ChangeAnimation(2, true, 200);
}

void CApplication::Update()
{

	//キャラクタークラスの更新
	mXPlayer.Update();
	//敵の更新
	mXEnemy.Update();

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
	//X軸−回転
	if (mInput.Key('I')) {
		mMatrix = mMatrix * CMatrix().RotateX(-1);
	}
	//Y軸＋回転
	if (mInput.Key('L')) {
		mMatrix = mMatrix * CMatrix().RotateY(1);
	}
	//Y軸−回転
	if (mInput.Key('J')) {
		mMatrix = mMatrix * CMatrix().RotateY(-1);
	}
	//行列設定
	glMultMatrixf(mMatrix.M());

	//頂点にアニメーションを適用する
	mModelX.AnimateVertex();

	//モデル描画
	mXPlayer.Render();
	                  //描画はカメラ設定の後で！！！！！//
	//コライダの描画
	CCollisionManager::Instance()->Render();
	//敵描画
	mXEnemy.Render();

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D RPOGRAMMING");

	//2Dの描画終了
	CCamera::End();

	//衝突処理
	CCollisionManager::Instance()->Collision();

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

