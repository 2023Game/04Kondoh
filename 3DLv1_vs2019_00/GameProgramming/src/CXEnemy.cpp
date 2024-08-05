#include "CXEnemy.h"


CXEnemy::CXEnemy()
	:mColSphereBody(this, nullptr, CVector(), 0.5f,CCollider::ETag::EBODY)
	, mColSphereHead(this, nullptr,
		CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereSword(this, nullptr,
		CVector(-10.0f, 10.0f, 50.0f), 0.5f,CCollider::ETag::ESWORD)
{}


void CXEnemy::Init(CModelX* model) {
	CXCharacter::Init(model);
	//‡¬s—ñ‚ÌÝ’è
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
	//“ª
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//Œ•
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
}

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
    if (m->Type() == CCollider::EType::ESPHERE)
    {
        if (o->Type() == CCollider::EType::ESPHERE)
        {
            if (o->Tag() == CCollider::ETag::ESWORD)
            {
                if (m->Tag() == CCollider::ETag::EBODY)
                {
                    if (CCollider::Collision(m, o))
                    {
                        ChangeAnimation(11, false, 30);
                    }
                }
            }
        }
    }
}