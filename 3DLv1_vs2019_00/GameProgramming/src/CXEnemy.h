#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter {
public:

	CXEnemy();
	void Init(CModelX* model);
	//Õ“Ëˆ—
	void Collision(CCollider* m, CCollider* o);

private:
	//ƒRƒ‰ƒCƒ_éŒ¾
	CCollider mColSphereBody;  //‘Ì
	CCollider mColSphereHead;  //“ª
	CCollider mColSphereSword0; //Œ•
	CCollider mColSphereSword1; //Œ•
	CCollider mColSphereSword2; //Œ•
	
};


#endif // !CXENEMY_H

