#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

class CFlamethrower;

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer : public CXCharacter
{
public:

	// プレイヤーの状態
	enum class EState
	{
		//eIdle,			// 待機
		eBattleIdle,	// 戦闘時の待機

		eAttack,	// 攻撃

		eGuard,   // 防御
		eAvoid,   // 回避

		eKnockBack,	// ノックバック
		eHit,		// 仰け反り
		eStun,		// 混乱
		eDeath,		// 死亡

		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
	};
	// 現在の状態を取得
	EState GetState();

	//インスタンスのポインタの取得
	static CPlayer* Instance();

	// コンストラクタ
	CPlayer();
	// デストラクタ
	~CPlayer();

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// ダメージを受ける
	void TakeDamage(int damage, float stan, CObjectBase* causer) override;

	// 防御処理
//	void Guard() override;
	// 回避処理
//	void Avoid() override;
	// 仰け反り処理
	void Hit() override;
	// 死亡処理
	void Death() override;

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render();

private:
	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec(bool force = false) const;
	/// <summary>
	/// ダメージ量を計算して返す
	/// </summary>
	/// <param name="taker">ダメージを受けるキャラ</param>
	/// <param name="outDamage">ダメージ量を返す用</param>
	/// <param name="outStan">怯み度を返す用</param>
	void CalcDamage(CCharaBase* taker, int* outDamage, float* outStan) const;

	// 非戦闘時の待機状態
	void UpdateIdle();
	// 戦闘時の待機状態
	void UpdateBattleIdle();
	// 攻撃処理
	void UpdateAttack();
	// 防御処理
	void UpdateGuard();
	// 回避処理
	void UpdateAvoid();

	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	// 移動の更新処理
	void UpdateMove();

	// 仰け反り処理
	void UpdateHit();
	// 死亡処理
	void UpdateDeath();


	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eBattleTPose,	// 戦闘時のTポーズ
		eBattleIdle,	// 戦闘時の待機アニメーション

		eBattleWalk,		// 歩行
		eBattleBackWalk,	// 後ろ方向への歩行
		eBattleLeftWalk,	// 左方向への歩行
		eBattleRightWalk,	// 右方向への歩行

		eUpAttackS,		// 弱上攻撃アニメーション
		eUpAttackM,		// 中上攻撃アニメーション
		eUpAttackL,		// 強上攻撃アニメーション

		eRightAttackS,	// 弱右攻撃アニメーション
		eRightAttackM,	// 中右攻撃アニメーション
		eRightAttackL,	// 強右攻撃アニメーション

		eLeftAttackS,	// 弱左攻撃アニメーション
		eLeftAttackM,	// 中左攻撃アニメーション
		eLeftAttackL,	// 強左攻撃アニメーション

		eGuard,		// 防御
		eAvoid,		// 回避

		eJumpStart,		// ジャンプ開始
		eJump,			// ジャンプ中
		eJumpEnd,		// ジャンプ終了
		eJumpAttack,	// ジャンプ攻撃

		eHit1,			// 仰け反り1
		eHit2,			// 仰け反り2
		eHit3,			// 仰け反り3

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type, bool restart = false);

	// 攻撃方向や、攻撃の威力によって
	// 切り替えるアニメーションの種類を取得
	EAnimType GetAttackAnimType() const;

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
		float speed;		// アニメーション速度（1.0で等倍）
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];


	EState mState;	// プレイヤーの状態
	//状態切り替え
	void ChangeState(EState state);
	// 回避状態へ切り替え
	void ChangeAvoid();
	// 攻撃状態へ切り替え
	void ChangeAttack();

	// 選択中の攻撃の強さ
	EAttackPower mSelectAttackPower;
	
	bool mIsBattleMode;		// バトルモードか
	bool mIsGuard;			// 防御しているか

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;	// 接地しているかどうか
	CVector mGroundNormal;	// 接地している地面の法線

	// 本体のコライダ
	CCollider* mpBodyCol;
	// 攻撃用のコライダ１（剣の刃の部分）
	CColliderCapsule* mpAttackCol1;
	// 攻撃用のコライダ２（剣の持ち手の部分）
	CColliderCapsule* mpAttackCol2;
	// 攻撃用のコライダ３（盾の部分）
	CColliderSphere* mpAttackCol3;

	CTransform* mpRideObject;

	CVector mAvoidDir;    // 回避方向

	int mStateStep;     // 状態内のステップ管理用
	float mElapsedTime; // 経過時間計測用

	int mRandHitAnim;	// ランダムな仰け反りアニメーション

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;

};
