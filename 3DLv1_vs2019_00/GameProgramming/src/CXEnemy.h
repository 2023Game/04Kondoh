#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter {
public:

	CXEnemy();
	void Init(CModelX* model);
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);

private:
	//�R���C�_�錾
	CCollider mColSphereBody;  //��
	CCollider mColSphereHead;  //��
	CCollider mColSphereSword; //��
	
};


#endif // !CXENEMY_H
