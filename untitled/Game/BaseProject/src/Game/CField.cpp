#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CWall.h"
#include <assert.h>
#include "CNavManager.h"
#include "CNavNode.h"

// �t�B�[���h�̃C���X�^���X
CField* CField::spInstance = nullptr;

CField* CField::Instance()
{
	return spInstance;
}

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	assert(spInstance == nullptr);
	spInstance = this;

	mpModel = CResourceManager::Get<CModel>("Field");
//	CModel* colModel = CResourceManager::Get<CModel>("FieldCol");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);


	CreateWalls();
	//CreateFieldObjects();
	// �o�H�T���p�̃m�[�h���쐬
	CreateNavNodes();
}

CField::~CField()
{
	spInstance = nullptr;
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

// �ǂ𐶐�
void CField::CreateWalls()
{
	// �ǂP�쐬
	CWall* wall = new CWall
	(
		CVector(20.0f, 0.94f, 0.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(5.0f, 5.0f, 5.0f)

	);
	mWalls.push_back(wall);

	// �ǂQ����
	wall = new CWall
	(
		CVector(-50.0f, 0.94f, -50.0f),
		CVector(  0.0f,  0.0f,  0.0f),
		CVector(  5.0f,  5.0f,  5.0f)
	);
	mWalls.push_back(wall);


}

/*
void CField::CreateFieldObjects()
{
	mpCubeModel = CResourceManager::Get<CModel>("FieldCube");
	mpCylinderModel = CResourceManager::Get<CModel>("FieldCylinder");


	new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 10.0f, -50.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(50.0f, 0.0f, 0.0f), 10.0f
	);
	new CRotateFloor
	(
		mpCylinderModel,
		CVector(-40.0f, 15.0f, 20.0f), CVector(1.0f, 1.0f, 1.0f),
		1.0f
	);

	// �����Ȃ����@
	new CMoveFloor
	(
		mpCubeModel,
		CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// �������@
	new CMoveFloor
	(
		mpCubeModel,
		CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(20.0f, 0.0f, 0.0f), 5.0f
	);
	// �����Ȃ����A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// ��]���鏰�@
	new CRotateFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
		0.5f
	);
	// �����Ȃ����A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// �����Ȃ����A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 50.0f, 0.0f), 5.0f
	);
	// �����Ȃ����B�i�⓹�j
	CMoveFloor* mf = new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 20.0f, 200.5f), CVector(4.0f, 1.0f, 2.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	mf->Rotate(0.0f, 0.0f, 30.0f);

	// �d���G�t�F�N�g
	CLineEffect* le = new CLineEffect(ETag::eField);
	le->SetTexture("LightningBolt");
	le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	le->SetAnimData(&mEffectAnimData);

	CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	int div = 3;
	float width = 5.0f;
	le->AddPoint(startPos, width, width);
	for (int i = 0; i < div; i++)
	{
		float alpha = (float)(i + 1) / div;
		CVector pos = CVector::Lerp(startPos, endPos, alpha);
		le->AddPoint(pos, width, width);
	}
}
*/

// �o�H�T���p�̃m�[�h���쐬
void CField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// �ǂP�̎���̌o�H�T��
		new CNavNode(CVector(30.0f, 0.0f,  35.0f));
		new CNavNode(CVector(10.0f, 0.0f,  35.0f));
		new CNavNode(CVector(10.0f, 0.0f, -35.0f));
		new CNavNode(CVector(30.0f, 0.0f, -35.0f));

		// �ǂQ�̎���̌o�H�T��
		new CNavNode(CVector(-15.0f, 0.0f, -40.0f));
		new CNavNode(CVector(-15.0f, 0.0f, -60.0f));
		new CNavNode(CVector(-85.0f, 0.0f, -60.0f));
		new CNavNode(CVector(-85.0f, 0.0f, -40.0f));

	}

}

// ���C�Ƃ̃t�B�[���h�I�u�W�F�N�g�Փ˔���
bool CField::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����̃t���O
	bool isHit = false;

	//�@�t�B�[���h�̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}

	// �ǂƂ̏Փ˔���
	for (CWall* wall : mWalls)
	{
		if (wall->CollisionRay(start, end, &tHit))
		{
			// �܂����ɏՓ˂��Ă��Ȃ��ꍇ��
			// ���łɏՓ˂��Ă���R���C�_�[���߂��ꍇ��
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}

	return isHit;
}

// ���C�Ƃ̃t�B�[���h�I�u�W�F�N�g�Փ˔���i�o�H�T���p�j
bool CField::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����̃t���O
	bool isHit = false;

	//�@�t�B�[���h�̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}

	// �ǂƂ̏Փ˔���
	for (CWall* wall : mWalls)
	{
		if (wall->NavCollisionRay(start, end, &tHit))
		{
			// �܂����ɏՓ˂��Ă��Ȃ��ꍇ��
			// ���łɏՓ˂��Ă���R���C�_�[���߂��ꍇ��
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}

	return isHit;
}

void CField::Update()
{
}

void CField::Render()
{
	mpModel->Render(Matrix());
}


CCollider* CField::GetFieldCol() const
{
	return mpColliderMesh;
}


std::list<CWall*> CField::GetWalls() const
{
	return mWalls;
}
