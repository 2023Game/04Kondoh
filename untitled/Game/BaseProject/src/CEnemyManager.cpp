#include "CEnemyManager.h"
#include "CEnemyBase.h"
#include "CField.h"
#include "Maths.h"

CEnemyManager* CEnemyManager::spInstance = nullptr;

// コンストラクタ
CEnemyManager::CEnemyManager()
	: mpCurrentEnemy(nullptr)
	, mpNextEnemy(nullptr)
{
}

// デストラクタ
CEnemyManager::~CEnemyManager()
{
}

// 敵のインスタンス
CEnemyManager* CEnemyManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CEnemyManager();
	}
	return spInstance;
}

// インスタンス削除
void CEnemyManager::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// 敵をリストに追加
void CEnemyManager::Add(CEnemyBase* enemy)
{
	mEnemies.push_back(enemy);
}

// 敵をリストから削除
void CEnemyManager::Remove(CEnemyBase* enemy)
{
	mEnemies.remove(enemy);
}

CEnemyBase* CEnemyManager::FindLockOnTarget(float angle, float length) const
{
	// メインカメラ取得
	CCamera* camera = CCamera::MainCamera();
	// メインカメラのZ座標取得
	CVector forward = camera->VectorZ();
	forward.Y();
	forward.Normalize();
	CVector side = CVector::Cross(CVector::up, forward);
	CVector camDir = CVector::Cross(CVector::up, side);

	float angleR = Math::DegreeToRadian(angle);

	float nearDot = 0.0f;
	// 近い敵
	CEnemyBase* nearEnemy = nullptr;

	for (CEnemyBase* enemy : mEnemies)
	{
		CVector vec = enemy->Position() - camera->Position();
		// 敵がカメラから離れすぎてないか
		float dist = vec.LengthSqr();
		if (dist > length * length) continue;

		// 扇形の外側かどうか
		CVector dir = vec.Normalized();
		float dot = CVector::Dot(camDir, dir);
		if (dot < cosf(angleR)) continue;
		// 
		if (nearEnemy == nullptr || nearDot < dot)
		{
			nearEnemy = enemy;
			nearDot = dot;
		}

	}

	return nearEnemy;
}

CEnemyBase* CEnemyManager::NextLockOnTarget()
{
	mpCurrentEnemy = FindLockOnTarget(45, 300);
	if (mpCurrentEnemy == nullptr) return;
	return nullptr;
}

bool CEnemyManager::IsFindTarget() const
{
	// 
	CEnemyManager* enemy = CEnemyManager::Instance();
	if (enemy == nullptr) return false;
	CFiled 
	return false;
}



