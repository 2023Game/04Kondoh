#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter {
public:

	CXPlayer();
	void Init(CModelX *model);
	void Update();

private:
	//コライダの宣言
	CCollider mColSphereBody;  //体
	CCollider mColSphereHead;  //頭
	CCollider mColSphereSword; //剣
	CInput mInput;

};

#endif // !CXPLAYER_H

