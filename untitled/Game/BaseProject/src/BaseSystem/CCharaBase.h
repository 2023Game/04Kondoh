#pragma once
#include "CObjectBase.h"

// 攻撃方向
enum class EAttackDir
{
	eNone,	// 方向指定なし
	eUp,    // 上攻撃
	eDown,  // 下攻撃
	eRight, // 右攻撃
	eLeft,  // 左攻撃
};

// 攻撃の強さ
enum class EAttackPower
{
	eAttackS,   // 弱攻撃
	eAttackM,   // 中攻撃
	eAttackL,   // 強攻撃

	Num,        // 攻撃の強さの数
};

/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	// コンストラクタ
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	virtual ~CCharaBase();

	// 最大HPを取得
	int GetMaxHp() const;
	// 現在HPを取得
	int GetHp() const;

	// 指定の攻撃タイプか
	bool IsAttackType(EAttackPower power, EAttackDir dir);
	// 攻撃方向を取得
	EAttackDir GetAttackDir() const;
	// 攻撃の方向の文字列を取得
	std::string GetAttackDirStr() const;
	// 攻撃の強さを取得
	EAttackPower GetAttackPower() const;
	// 攻撃の強さの文字列を取得
	std::string GetAttackPowerStr() const;


	/// <summary>
	/// 指定した攻撃の方向と強さで怯むかどうか
	/// </summary>
	/// <param name="dir">方向</param>
	/// <param name="power">強さ</param>
	/// <returns>trueであれば怯む</returns>
	virtual bool CheckAttackParry(EAttackDir dir, EAttackPower power) const;

	// 防御時のパリィ
	virtual bool CheckGuardParry() const;

	/// <summary>
	/// キャラがダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	/// <param name="stan">加算される怯み度</param>
	/// <param name="causer">ダメージを与えたキャラ</param>
	virtual void TakeDamage(int damage,float stun, float knockback, CCharaBase* causer);

	// 防御処理
	virtual void Guard();
	// 回避処理
	virtual void Avoid();
	// ノックバック
	virtual void KnockBack();
	// 仰け反り処理
	virtual void Hit();
	// 怯み処理
	virtual void Stun();
	// 死亡
	virtual void Death();

	// 回避中か
	virtual bool IsAvoiding() const;
	// 防御中か
	virtual bool IsGuarding() const;
	// 防御パリィ中か
	virtual bool IsGuardParry() const;
	// 仰け反り中か
	virtual bool IsHiting() const;
	// 死んでいるかどうか
	bool IsDeath() const;

	// 更新
	void Update()override;

protected:
	int mMaxHp;	// 最大HP
	int mHp;	// 現在HP

	float mStunPoints;		// 現在の怯み度
	float mStunThreshold;	// 怯み度のしきい値

	float mKnockBack;

	EAttackDir mAttackDir;
	EAttackPower mAttackPower;
};
