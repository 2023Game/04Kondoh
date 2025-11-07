#pragma once

class CEnemyBase;

class CStateBase {
public:

	// コンストラクタ
	explicit CStateBase(CEnemyBase* owner);
	// デストラクタ
	virtual ~CStateBase() = default;

	// ステートが始まる時に一度だけ呼ばれる関数
	virtual void OnStart() = 0;
	// ステートが更新される時に呼ばれる関数
	virtual void OnUpdate() = 0;
	// ステートが終了する時に一度だけ呼ばれる関数
	virtual void OnEnd() = 0;

private:

	CEnemyBase* mOwner;
};