#ifndef CCAMERA_H
#define CCAMERA_H
#include "CCharacter3.h"
#include "CColliderLine.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CPlayer.h"
#include "CModel.h"
#include "CMatrix.h"
#include <vector>

/*
* カメラクラス
* 画面に表示するエリアを設定する
*/
class CCamera : public CCharacter3 {
public:
	CMatrix CameraMatrix();
	void Render();
	static void Start(double left, double right, double bottom, double top);
	void Update();
	////CBillBoard(位置,幅,高さ,優先度)
	//CCamera(CVector pos, float w, float h, int priority);
	//CCamera(位置、回転、スケール)
	CCamera(const CVector& pos, const CVector& rot, const CVector& scale);
	CCamera();

	static const CMatrix& ModelViewInverse();
	//インスタンスのポイントの取得
	static CCamera* Instance();

	//衝突処理
	void Collision();
	//衝突処理
	//Collision(コライダ１,コライダ２)
	void Collision(CCollider* m, CCollider* o);

	CVector e, c, u; //視点,注視点,上方向
	float VX;  //縦視点移動の値
	float VY;  //横視点移動の値
	float CZ;
	float ShootTime;

private:
	enum class ECamera
	{
		View1,  //一人称
		View2,  //二人称
		View3,  //三人称
	};
	ECamera mCamera;
	int ViewNumber;
	CVector mEye;
	
	//前回のマウス座標
	int mMx, mMy;

	CInput mInput;
	CPlayer *mpPlayer;

	static CMatrix mModelViewInverse;
	//カメラのインスタンス
	static CCamera* spInstance;
	CColliderLine mLine;
	CCollider mCollider;
};

#endif // !CCAMERA_H
