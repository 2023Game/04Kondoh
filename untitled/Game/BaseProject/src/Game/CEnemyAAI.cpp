#include "CEnemyA.h"
#include "CStateIdle.h"
#include "CStatePatrol.h"
#include "CStateChase.h"
#include "CStateLost.h"

#define IDLE_TIME 5.0f          // 待機状態の時間

// このキャラクターの行動AIを設定
void CEnemyA::SetupStateAI()
{
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
			[this]() {return !IsFoundPlayer(); });
	}

	// 見失った状態
	{
		CStateLost* state = new CStateLost(this);
		mStateMachine.RegisterState((int)EState::eLost, state);

		// プレイヤーが視界に入ったら、追跡状態へ遷移
		state->AddTransition((int)EState::eChase, false,
			[this]() {return IsFoundPlayer(); });
		// 見失った状態が指定時間まで続くと待機状態へ遷移
		//state->AddTransition((int)EState::eIdle);
	}

	// 攻撃状態
	{
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
}
