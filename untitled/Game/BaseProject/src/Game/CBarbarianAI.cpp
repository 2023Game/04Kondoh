#include "CBarbarian.h"
#include "CStateIdle.h"
#include "CStatePatrol.h"
#include "CStateChase.h"
#include "CStateLost.h"
#include "CStateAttack.h"
#include "CAttackBlow.h"

#include "Maths.h"

#define IDLE_TIME 5.0f          // 待機状態の時間

#define CLOSE_RANGE		10.0f	// 頭突きの攻撃範囲
#define NEAR_RANGE		45.0f	// 薙ぎ払いと回し蹴り、三連攻撃の攻撃範囲
#define MIDDLE_RANGE	0.0f	// 中距離攻撃（今の所無し）
#define LONG_RANGE		150.0f	// タックルの攻撃範囲
#define DETECT_RANGE	30.0f	// 判定用の攻撃範囲

//// 攻撃範囲データのテーブル
//const std::vector<CEnemyBase::AttackRangeData> RANGE_DATA =
//{
//	{ CLOSE_RENGE, NEAR_RANGE, MIDDLE_RANGE, CLOSE_RENGE }
//};

// このキャラクターの行動AIを設定
void CBarbarian::SetupStateAI()
{
	AddAttackRange(CLOSE_RANGE, NEAR_RANGE, MIDDLE_RANGE, LONG_RANGE, DETECT_RANGE);

	// 待機状態
	{
		// 待機状態の生成とステートマシンへの登録
		CStateIdle* state = new CStateIdle(this, IDLE_TIME);
		mStateMachine.RegisterState((int)EState::eIdle, state);

		// 待機状態が終了したら、巡回状態へ遷移
		state->AddTransition((int)EState::ePatrol);
		// プレイヤーが視界に入ったら、追跡状態へ遷移
		state->AddTransition((int)EState::eChase, false, 
			[this]() {return IsFoundPlayer(); });
	}

	// 巡回状態
	{
		// 巡回状態の生成とステートマシンへの登録
		CStatePatrol* state = new CStatePatrol(this);
		mStateMachine.RegisterState((int)EState::ePatrol, state);

		// 巡回が終われば、待機状態へ遷移
		state->AddTransition((int)EState::eIdle);
		// プレイヤーが視界に入ったら、追跡状態へ遷移
		state->AddTransition((int)EState::eChase, false,
			[this]() {return IsFoundPlayer(); });

		// 巡回ポイントのリストを設定
		state->SetPatrolPoints(mPatrolPoints);
	}

	// 戦闘待機状態 
	{
	}

	// 追跡状態
	{
		// 待機状態の生成とステートマシンへの登録
		CStateChase* state = new CStateChase(this);
		mStateMachine.RegisterState((int)EState::eChase, state);

		// プレイヤーが視界外に出たら、見失い状態へ遷移
		state->AddTransition((int)EState::eLost, false,
			[this]() {return !IsLookPlayer(); });
		// 
		state->AddTransition((int)EState::eAttack, false,
			[this]() {return CanAttackPlayer(CLOSE_RANGE); });
	}

	// 見失った状態
	{
		CStateLost* state = new CStateLost(this);
		mStateMachine.RegisterState((int)EState::eLost, state);

		// プレイヤーが視界に入ったら、追跡状態へ遷移
		state->AddTransition((int)EState::eChase, false,
			[this]() {return IsFoundPlayer(); });
		// 見失った状態が指定時間まで続くと待機状態へ遷移
		state->AddTransition((int)EState::eIdle);
	}

	// 攻撃状態
	{
		CStateAttack* state = new CStateAttack(this);
		mStateMachine.RegisterState((int)EState::eAttack, state);
		mStateMachine.RegisterAttackState((int)EAttackType::eNone, state);

		
	}

	// 防御状態
	{
	}

	// 回避状態
	{
	}

	// 仰け反り状態
	{
	}

	// スタン状態
	{
	}

	// スタン待ち状態
	{
	}

	// パリィされた時のスタン状態
	{
	}

	// 死亡状態
	{
	}


	// ↓攻撃の種類↓

	// 薙ぎ払い
	{
		CAttackBlow* attack = new CAttackBlow(this);
		mStateMachine.RegisterAttackState((int)EAttackType::eBlowL, attack);
		mStateMachine.RegisterAttackState((int)EAttackType::eBlowR, attack);

		if ((int)EAttackType::eBlowL) return mpAttackState->SetRandAnimNum(1);
		else if ((int)EAttackType::eBlowR) return mpAttackState->SetRandAnimNum(2);
		else return mpAttackState->SetRandAnimNum(Math::Rand(1, 2));
	}

	// 回し蹴り
	{
	}

	// タックル
	{
	}

	// 押し出し
	{
	}

	// 三連攻撃
	{
	}

}
