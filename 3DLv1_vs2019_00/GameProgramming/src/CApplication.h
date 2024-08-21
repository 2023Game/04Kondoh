#pragma once
#include "CTexture.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>

#include "CSound.h"
#include "CVector.h"
#include "CModel.h"
#include "CCharacter3.h"
#include "CPlayer.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CEnemy3.h"
#include "CUi.h"
#include "CModelX.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"

class CApplication
{
private:
	CModelX mKnight;
	//敵のインスタンス
	CXEnemy mXEnemy;
	//キャラクタのインスタンス
	CXPlayer mXPlayer;
	CXCharacter mCharacter;
	CMatrix mMatrix;
	CModelX mModelX;
	static CUi* spUi;    //UIクラスのポインタ
	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	//C5モデル
	CModel mModelC5;
	CModel mBackGround; //背景モデル
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
	CInput mInput;
	CFont mFont;
    //CCharacterのポインタの可変長配列
    //std::vector<CCharacter*> mCharacters;
	CVector mEye;
	CModel mModel;
	CPlayer mPlayer;

public:
	~CApplication();
	static CUi* Ui();   //UIクラスのインスタンスを取得
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};