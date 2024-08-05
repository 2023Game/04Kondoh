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
	//ƒRƒ‰ƒCƒ_‚ÌéŒ¾
	CCollider mColSphereBody;  //‘Ì
	CCollider mColSphereHead;  //“ª
	CCollider mColSphereSword; //Œ•
	CInput mInput;

};

#endif // !CXPLAYER_H

