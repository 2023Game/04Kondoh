#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMatrix.h"
#include "CCharacter3.h"

class CXCharacter : public CCharacter3 {
public:
	//デフォルトコンストラクタ
	CXCharacter();
	//初期化処理
	void Init(CModelX* model);
	//アニメーションの変更
	void ChangeAnimation(int index, bool loop, float framesize);
	//更新処理
	void Update();
	//更新処理
	//materix: 移動、回転、拡大縮小の行列
	void Update(CMatrix& m);
	//描画処理
	void Render();
	//アニメーションの再生終了判定
	//ture:終了　false:再生中
	bool IsAnimationFinished();
	int AnimationIndex();  //アニメーション番号の取得

protected:
	CModelX* mpModel;     //モデルデータ
	int mAnimationIndex;    //アニメーション
	bool mAnimationLoopFlg;   //true:アニメーション
	float mAnimationFrame;   //アニメーションの再生フレーム
	float mAnimationFrameSize;  //アニメーションの再生フレーム数

};

#endif // !CXCHARACTER_H

