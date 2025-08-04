#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CFieldWall.h"
#include "CWall.h"
#include "CLever.h"
#include "CSingleDoor.h"
#include "CDoubleDoors.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include <assert.h>

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

	// ���̃��f��
	mpFieldModel = CResourceManager::Get<CModel>("Field");
	// ���̃R���C�_�[�ݒ�
	mpFieldCol = new CColliderMesh
	(
		this, ELayer::eField, mpFieldModel,
		true
	);
	//mpFieldCol->SetShow(true);

	// �w�i�̃��f��
	mpBackGroundModel = CResourceManager::Get<CModel>("BackGround");

	// �C���X�^���X�擾
	CNavManager* navManager = CNavManager::Instance();
	CFieldWall* fieldWall = CFieldWall::Instance();
	// �Օ����ɂȂ�R���C�_�[�ɒǉ�����
	navManager->AddCollider(mpFieldCol);
	navManager->AddCollider(fieldWall->GetFieldWallCol());

	// �ǂ��쐬
	CreateWalls();

	for (CWall* wall : mWalls)
	{
		navManager->AddCollider(wall->GetNavCol());
	}

	// �o�H�T���p�̃m�[�h���쐬
	CreateNavNodes();
	// �t�B�[���h�I�u�W�F�N�g���쐬
	CreateFieldObjects();

	CreateGimmick();
}

CField::~CField()
{
	spInstance = nullptr;
	if (mpFieldCol != nullptr)
	{
		delete mpFieldCol;
		mpFieldCol = nullptr;
	}
}

// �ǂ𐶐�
void CField::CreateWalls()
{
	// �ǂP�쐬
	CWall* wall = new CWall
	(
		CVector(-369.0f, 0.0f, 0.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(7.0f, 8.0f, 6.0f)

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


void CField::CreateFieldObjects()
{

	mpMoveFloor1Model = CResourceManager::Get<CModel>("MoveFloor1");
	mpMoveFloor2Model = CResourceManager::Get<CModel>("MoveFloor2");
	mpStaircaseModel = CResourceManager::Get<CModel>("Staircase");

	new CMoveFloor
	(
		mpStaircaseModel,
		CVector(0.0f, 5.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 0.0f, 0.0f), 1.0f
	);

	// 
	new CMoveFloor
	(
		mpMoveFloor1Model,
		CVector(0.0f, 30.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 50.0f, 0.0f), 10.0f
	);

	// 
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(0.0f, 25.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f,25.0f, 0.0f), 7.0f
	);
	// 
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(50.0f, 75.0f, 15.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 25.0f, 0.0f), 9.0f
	);
	// 
	new CMoveFloor
	(
		mpMoveFloor2Model,
		CVector(100.0f, 125.0f, 30.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 25.0f, 0.0f), 6.0f
	);

	/*
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
	*/
}

void CField::CreateGimmick()
{
	// �P�Ԗڂ̃��o�[�ƃh�A									// CVector(90.0f, 270.0f, 90.0f)
	CLever* lever1 = new CLever(CVector(-185.0f, 14.0f, -1.0f), CVector(0.0f, 0.0f, -90.0f));
	CSingleDoor* door1 = new CSingleDoor
	(
		CVector(-290.0f, 0.0f, 98.0f),
		CVector(0.0f, 72.0f, 0.0f),
		CVector(1.2f, 1.0f, 1.2f)
	);
	door1->SetAnimPos
	(
		CVector(-341.0f, 0.0f, 111.0f),
		CVector(-304.0f, 0.0f, 99.0f)
	);


	// �Q�Ԗڂ̃��o�[�ƃh�A
	// �ʒu(-270.0f, 175.0f, 90.0f)�A�p�x(90.0f, 163.0f, 0.0f)
	CLever* lever2 = new CLever(CVector(-270.0f, 175.0f, 90.0f), CVector(90.0f, 163.0f, 0.0f));
	CDoubleDoors* door2 = new CDoubleDoors
	(
		CVector(0.0f, 0.0f, 315.0f), CVector(0.0f, 0.0f, 262.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 0.8f, 0.7f)
	);
	door2->SetAnimPosL
	(
		CVector(0.0f, 0.0f, 345.0f),
		CVector(0.0f, 0.0f, 315.0f)
	);
	door2->SetAnimPosR
	(
		CVector(0.0f, 0.0f, 226.0f),
		CVector(0.0f, 0.0f, 262.0f)
	);

	// �R�Ԗڂ̃��o�[�ƃh�A
	CLever* lever3 = new CLever(CVector(105.0f, 15.0f, 344.0f), CVector(90.0f, 335.0f, 0.0f));
	CSingleDoor* door3 = new CSingleDoor
	(
		CVector(0.0f, 154.0f, 307.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door3->SetAnimPos
	(
		CVector(0.0f, 154.0f, 359.0f),
		CVector(8.0f, 154.0f, 310.0f)
	);

	// �S�Ԗڂ̃��o�[�ƃh�A
	CLever* lever4 = new CLever(CVector(105.0f, 162.0f, 344.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door4 = new CSingleDoor
	(
		CVector(154.0f, 0.0f, 209.0f),
		CVector(0.0f, 143.7f, 0.0f),
		CVector(0.9f, 1.0f, 0.9f)
	);
	door4->SetAnimPos
	(
		CVector(129.5f, 0.0f, 178.5f),
		CVector(153.0f, 0.0f, 209.0f)
	);

	// �T�Ԗڂ̃��o�[�ƃh�A
	CLever* lever5 = new CLever(CVector(158.0f, 14.0f, 103.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door5 = new CSingleDoor
	(
		CVector(152.0f, 154.0f, 208.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door5->SetAnimPos
	(
		CVector(152.0f, 154.0f, 208.0f),
		CVector(152.0f, 154.0f, 208.0f)
	);

	// �U�Ԗڂ̃��o�[�ƃh�A
	CLever* lever6 = new CLever(CVector(270.0f, 162.0f, 94.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door6 = new CSingleDoor
	(
		CVector(295.0f, 0.0f, 85.0f),
		CVector(0.0f, 105.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door6->SetAnimPos
	(
		CVector(291.0f, 0.0f, 91.0f),
		CVector(291.0f, 0.0f, 91.0f)
	);

	// �V�Ԗڂ̃��o�[�ƃh�A
	//CLever* lever7 = new CLever(CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f));
	//CDoubleDoors* door7 = new CDoubleDoors
	//(
	//	CVector(286.0f, 154.0f, -92.0f), CVector(248.0f, 154.0f, -81.0f),
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(1.0f, 1.0f, 1.0f)
	//);
	//door7->SetAnimPosL
	//(
	//	CVector(286.0f, 154.0f, -92.0f),
	//	CVector(286.0f, 154.0f, -92.0f)
	//);
	//door7->SetAnimPosR
	//(
	//	CVector(248.0f, 154.0f, -81.0f),
	//	CVector(248.0f, 154.0f, -81.0f)
	//);

	// �W�Ԗڂ̃��o�[�ƃh�A
	CLever* lever8 = new CLever(CVector(290.0f, 15.0f, -215.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door8 = new CSingleDoor
	(
		CVector(0.0f, 0.0f, -315.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door8->SetAnimPos
	(
		CVector(0.0f, 0.0f, -315.0f),
		CVector(0.0f, 0.0f, -315.0f)
	);

	// �X�Ԗڂ̃��o�[�ƃh�A
	CLever* lever9 = new CLever(CVector(-102.0f, 162.0f, -346.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door9 = new CSingleDoor
	(
		CVector(-174.0f, 0.0f, -240.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door9->SetAnimPos
	(
		CVector(-174.0f, 0.0f, -240.0f),
		CVector(-174.0f, 0.0f, -240.0f)
	);
	
	// �P�O�Ԗڂ̃��o�[�ƃh�A
	CLever* lever10 = new CLever(CVector(-293.0f, 15.0f, -214.0f), CVector(90.0f, 0.0f, 0.0f));
	CDoubleDoors* door10 = new CDoubleDoors
	(
		CVector(-150.0f, 154.0f, -206.0f), CVector(-180.0f, 154.0f, -247.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door10->SetAnimPosL
	(
		CVector(-150.0f, 154.0f, -206.0f),
		CVector(-150.0f, 154.0f, -206.0f)
	);
	door10->SetAnimPosR
	(
		CVector(-180.0f, 154.0f, -247.0f),
		CVector(-180.0f, 154.0f, -247.0f)
	);

	// �P�P�Ԗڂ̃��o�[�̃h�A
	CLever* lever11 = new CLever(CVector(-293.0f, 162.0f, -214.0f), CVector(90.0f, 0.0f, 0.0f));
	CSingleDoor* door11 = new CSingleDoor
	(
		CVector(-131.0f, 0.0f, -123.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door11->SetAnimPos
	(
		CVector(-131.0f, 0.0f, -123.0f),
		CVector(-131.0f, 0.0f, -123.0f)
	);

	// �e���o�[���e�h�A�ɑΉ�������
	door1->AddInputObjs(lever1);
	door2->AddInputObjs(lever2);
	door3->AddInputObjs(lever3);
	door4->AddInputObjs(lever4);
	door5->AddInputObjs(lever5);
	door6->AddInputObjs(lever6);
	//door7->AddInputObjs(lever7);
	door8->AddInputObjs(lever8);
	door9->AddInputObjs(lever9);
	door10->AddInputObjs(lever10);
	door11->AddInputObjs(lever11);


#if _DEBUG
	lever1->SetDebugName("Lever1");
	lever2->SetDebugName("Lever2");
	lever3->SetDebugName("Lever3");
	lever4->SetDebugName("Lever4");
	lever5->SetDebugName("Lever5");
	lever6->SetDebugName("Lever6");
	//lever7->SetDebugName("Lever7");
	lever8->SetDebugName("Lever8");
	lever9->SetDebugName("Lever9");
	lever10->SetDebugName("Lever10");
	lever11->SetDebugName("Lever11");
#endif

}


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

	CFieldWall* fieldWall = CFieldWall::Instance();

	//�@�t�B�[���h�̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpFieldCol, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}

	if (fieldWall->NavCollisionRay(start, end, &tHit))
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

	CFieldWall* fieldWall = CFieldWall::Instance();

	//�@�t�B�[���h�̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpFieldCol, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}

	if (fieldWall->NavCollisionRay(start, end, &tHit))
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
	mpFieldModel->Render(Matrix());
	mpBackGroundModel->Render(Matrix());
}


CColliderMesh* CField::GetFieldCol() const
{
	return mpFieldCol;
}


std::list<CWall*> CField::GetWalls() const
{
	return mWalls;
}
