#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//変数行列クラスのインクルード
#include "CTransform.h"
//モデルクラスのインクルード
#include "CModel.h"
/*
キャラクタークラス
ゲームキャラクタの基本的な機能を定義する
*/

class CCharacter3 : public CTransform {
public:
	//モデル設定
	//Model(モデルクラスのポイント)
	void Model(CModel* m);
	//描画処理
	void Render();
protected:
	CModel* mpModel; //モデルのポインタ

};

#endif // !CCHARACTER3_H

