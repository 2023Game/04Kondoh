#ifndef CBACKGROUND_H
#define CBACKGROUND_H
//キャラクタークラスインクルード
#include "CCharacter3.h"
#include "CColliderMesh.h"


/*
キャラクタークラス伝承
*/
class CBackGround : public CCharacter3 {
public:
	static CBackGround* Instance();

    CBackGround();
	//CBackGround(位置,回転,スケール)
	CBackGround(const CVector& pos, const CVector& rot, const CVector& scale);
	//更新処理
	void Update();
	void ColliderMesh();
private:
	static CBackGround* spInstance;
	CColliderMesh mColliderMesh;
};

#endif // !CBACKGROUND_H

