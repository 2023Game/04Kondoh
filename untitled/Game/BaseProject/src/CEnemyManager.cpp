#include "CEnemyManager.h"
#include "CEnemyBase.h"
#include "CField.h"
#include "CPlayer.h"
#include "Maths.h"

CEnemyManager* CEnemyManager::spInstance = nullptr;

// �R���X�g���N�^
CEnemyManager::CEnemyManager()
{
}

// �f�X�g���N�^
CEnemyManager::~CEnemyManager()
{
}

// �G�̃C���X�^���X
CEnemyManager* CEnemyManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CEnemyManager();
	}
	return spInstance;
}

// �C���X�^���X�폜
void CEnemyManager::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// �G�����X�g�ɒǉ�
void CEnemyManager::Add(CEnemyBase* enemy)
{
	mEnemies.push_back(enemy);
}

// �G�����X�g����폜
void CEnemyManager::Remove(CEnemyBase* enemy)
{
	mEnemies.remove(enemy);
}



//CEnemyBase* CEnemyManager::FindLockOnTarget(float angle, float length) const
//{
//	// ���C���J�����擾
//	CCamera* camera = CCamera::MainCamera();
//	// ���C���J������Z���W�擾
//	CVector forward = camera->VectorZ();
//	forward.Y();
//	forward.Normalize();
//	CVector side = CVector::Cross(CVector::up, forward);
//	CVector camDir = CVector::Cross(CVector::up, side);
//
//	float angleR = Math::DegreeToRadian(angle);
//
//	float nearDot = 0.0f;
//	// �߂��G
//	CEnemyBase* nearEnemy = nullptr;
//
//	for (CEnemyBase* enemy : mEnemies)
//	{
//		CVector vec = enemy->Position() - camera->Position();
//		// �G���J�������痣�ꂷ���ĂȂ���
//		float dist = vec.LengthSqr();
//		if (dist > length * length) continue;
//
//		// ��`�̊O�����ǂ���
//		CVector dir = vec.Normalized();
//		float dot = CVector::Dot(camDir, dir);
//		if (dot < cosf(angleR)) continue;
//
//		// TODO�F�Օ��������鎞�A���b�N�I���o���Ȃ�
//
//
//		if (nearEnemy == nullptr || nearDot < dot)
//		{
//			nearEnemy = enemy;
//			nearDot = dot;
//		}
//
//	}
//
//	return nearEnemy;
//}


//bool CEnemyManager::IsFindTarget() const
//{
//	CPlayer* player = CPlayer::Instance();
//	// �G�����݂��Ȃ��ꍇ
//	CEnemyManager* enemy = CEnemyManager::Instance();
//	if (enemy == nullptr) return false;
//	// �t�B�[���h�����݂��Ȃ��ꍇ
//	CField* field = CField::Instance();
//	if (field == nullptr) return false;
//
//	CVector enemyPos = Instance()->Position();
//	//
//	CVector playerPos = player->Position();
//
//	CHitInfo hit;
//	//
//	if (field->CollisionRay(playerPos, enemyPos, &hit)) return false;
//
//	return true;
//}

//CEnemyBase* CEnemyManager::NextLockOnTarget()
//{
//	mpCurrentEnemy = FindLockOnTarget(45, 300);
//	if (mpCurrentEnemy == nullptr) return;
//	return nullptr;
//}




