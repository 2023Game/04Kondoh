#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CMiss.h"
#include <vector>
#include "CCharacterManager.h"

#include "CGame.h"
#include "CSound.h"
#include "CVector.h"
#include "CModel.h"
#include "CCharacter3.h"
#include "CPlayer.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CEnemy3.h"
#include "CBackGround.h"

class CApplication
{
private:
	enum class ECamera
	{
		View1,  //一人称
		View2,  //二人称
		View3,  //三人称
	};
	ECamera mCamera;
	float VX;  //縦視点移動の値
	float VY;  //横視点移動の値

	//前回のマウス座標
	int mMx, mMy;

	int ViewNumber;
	static CUi* spUi;    //UIクラスのポインタ
	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	//三角コライダ２作成
	// CColliderTriangle mColliderTriangle2;
	//三角コライダ作成
	// CColliderTriangle mColliderTriangle;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	//C5モデル
	CModel mModelC5;  //C5のモデル
	CModel mModel;    //プレイヤーのモデル
	CModel mModelBG;  //ステージのモデル
	CBackGround mBackGround; //背景モデル
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	CGame* mpGame;
	static CCharacterManager mCharacterManager;
	enum class EGame
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EGame mGame;
//  CCharacter mRectangle;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
//  CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	CMiss* mpMiss;
    //CCharacterのポインタの可変長配列
//  std::vector<CCharacter*> mCharacters;
	CVector mEye;
//	CCharacter3 mCharacter;
	CPlayer mPlayer;

public:
	~CApplication();
	static CUi* Ui();   //UIクラスのインスタンスを取得
	static const CMatrix& ModelViewInverse();
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};