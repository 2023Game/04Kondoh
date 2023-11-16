#ifndef CENEMY_H
#define CENEMY_H
//キャラクタークラスのインクルード
#include "CCharacter3.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CCharacter3 {
public:
	//コンストラスタ
	//CEnemy(モデル、位置、回転、拡縮)
	CEnemy(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
};

#endif // !CENEMY_H
