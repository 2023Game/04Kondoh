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
//背景モデルデータの指定
#define MODEL_BACKGROUND "res\\StageNo_09.obj","res\\StageNo_09.mtl"

#define MOS_POS_X 400  //マウス座標のX補正
#define MOS_POS_Y 300  //マウス座標のY補正


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

	new CEnemy(CVector(11.0f, -2.0f, -50.0), CVector(), CVector(0.2f, 0.2f, 0.2f));
	mModel.Load(MODEL_OBJ);
	mModelBG.Load(MODEL_BACKGROUND);
	CMatrix matrix;
	matrix.Print();
	mBackGround.Model(&mModelBG);
	mBackGround.Scale(CVector(2.5f, 2.5f, 2.5f));
	mPlayer.Model(&mModel);
	mPlayer.Position(CVector(11.0f, -3.0f, -53.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));
	mPlayer.Scale(CVector(0.2f, 0.2f, 0.2f));
	//ビルボードの生成
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//背景モデルから三角コライダを生成
    //親インスタンスと親行列
	mBackGround.ColliderMesh();

	spUi = new CUi();  //UIクラスの生成

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



	//タスクマネージャーの更新
	CTaskManager::Instance()->Update();
	//コリジョンマネージャの衝突処理
//	CCollisionManager::Instance()->Collision();
	CTaskManager::Instance()->Collision();


	//タスクリストの削除
	CTaskManager::Instance()->Delete();
	//タスクマネージャーの描画
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();

	//spUi->Render();   //UIの描画
}
/*
CTaskManager CApplication::mTaskManager;
CTaskManager* CApplication::TaskManager()
{
	return &mTaskManager;
}
*/


CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi;   //インスタンスのポインタを返す
}

CApplication::~CApplication()
{
	delete spUi;   //インスタンスUiの削除
}