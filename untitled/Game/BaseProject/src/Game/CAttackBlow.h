#pragma once
#include "CStateAttack.h"

class CAttackBlow : public CStateAttack
{
public:
	// コンストラクタ
	CAttackBlow(CEnemyBase* owner);

	// 開始処理
	void Enter() override;
	// 更新処理
	void Update() override;

private:

	int mStateStep;
	float mElapsedTime;

	bool mIsTripleAttack;	// 三連攻撃状態か

};