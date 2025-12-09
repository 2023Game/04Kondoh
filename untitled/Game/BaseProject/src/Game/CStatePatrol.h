#pragma once
#include "CStateBase.h"

class CStatePatrol : public CStateBase
{
public:
	CStatePatrol(CEnemyBase* owner);

	// 巡回するポイントのリストを設定
	void SetPatrolPoints(std::vector<CVector> patrolPoints);

	void Enter() override;
	void Update() override;

private:
	// 巡回ポイントを切り替え
	bool ChangePatrolPoint();

	int mStateStep;	// ステートステップ
	float mElapsedTime;	// 経過時間計測用

	// 巡回ポイントのリスト
	std::vector<CVector> mPatrolPoints;
	int mNextPatrolIndex;	// 次の巡回ポイントのインデクス
};