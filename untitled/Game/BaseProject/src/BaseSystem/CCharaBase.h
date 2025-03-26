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
	virtual bool CheckParry(EAttackDir dir, EAttackPower power) const;

	/// <summary>
	/// ダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	/// <param name="stan">加算される怯み度</param>
	/// <param name="causer">ダメージを与えたもの</param>
	virtual void TakeDamage(int damage,float stan, CObjectBase* causer);

	// 死亡
	virtual void Death();
	// 死んでいるかどうか
	bool IsDeath() const;

	// 怯み処理
	virtual void Stan();

	// 防御処理
	//virtual void Defense();

	// 更新
	void Update()override;

protected:
	int mMaxHp;	// 最大HP
	int mHp;	// 現在HP

	float mStanPoints;		// 現在の怯み度
	float mStanThreshold;	// 怯み度のしきい値

	EAttackDir mAttackDir;
	EAttackPower mAttackPower;
};
