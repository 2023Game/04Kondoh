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
	//�R���C�_�̐錾
	CCollider mColSphereBody;  //��
	CCollider mColSphereHead;  //��
	CCollider mColSphereSword; //��
	CInput mInput;

};

#endif // !CXPLAYER_H

