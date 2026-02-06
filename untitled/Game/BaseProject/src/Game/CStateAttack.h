#pragma once
#include "CStateBase.h"

class CStateAttack : public CStateBase
{
public:
	// コンストラクタ
	CStateAttack(CEnemyBase* owner, const std::string& name = nullptr);

	void Enter() override;
	void Update() override;

	void SetRandAnimNum(int number);

protected:

	// アニメーション用ランダムナンバー
	int mRandAnimNum;

	bool mIsTripleAttack;	// 三連攻撃状態か

private:


};
