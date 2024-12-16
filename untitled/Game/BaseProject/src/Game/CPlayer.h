#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
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
	//インスタンスのポインタの取得
	static CPlayer* Instance();

	// コンストラクタ
	CPlayer();
	// デストラクタ
	~CPlayer();

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
	CVector CalcMoveVec() const;

	// 非戦闘時の待機状態
	void UpdateIdle();

	// 戦闘時の待機状態
	void UpdateAttackIdle();
	// 攻撃
	void UpdateAttack();
	// 攻撃終了待ち
	void UpdateAttackWait();

	// 回避
	void UpdateEvasion();

	// 防御
	void UpdateDefense();

	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();

	// 移動の更新処理
	void UpdateMove();

	// モーションブラーの更新処理
	void UpdateMotionBlur();

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eAttackTPose,    // 戦闘時のTポーズ
		eAttackIdle,     // 戦闘時の待機アニメーション

		eAttackWalk,     // 歩行
		eAttackBackWalk, // 後ろ方向への歩行
		eAttackLeftWalk,     // 左方向への歩行
		eAttackRightWalk,    // 右方向への歩行

		eUpAttackS,	   // 弱上攻撃アニメーション
		eUpAttackM,	   // 中上攻撃アニメーション
		eUpAttackL,	   // 強上攻撃アニメーション

		eDwonAttackS,  // 弱下攻撃アニメーション
		eDwonAttackM,  // 中下攻撃アニメーション
		eDwonAttackL,  // 強下攻撃アニメーション

		eRightAttackS, // 弱右攻撃アニメーション
		eRightAttackM, // 中右攻撃アニメーション
		eRightAttackL, // 強右攻撃アニメーション

		eLeftAttackS,  // 弱左攻撃アニメーション
		eLeftAttackM,  // 中左攻撃アニメーション
		eLeftAttackL,  // 強左攻撃アニメーション

		eDefense,      // 防御
		eEvasion,      // 回避

		eJumpStart,	   // ジャンプ開始
		eJump,		   // ジャンプ中
		eJumpEnd,	   // ジャンプ終了

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機

		eAttack,	// 攻撃
		eAttackWait,// 攻撃終了待ち

		eDefense,   // 防御
		eEvasion,   // 回避

		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
	};
	EState mState;	// プレイヤーの状態

	// 攻撃方向
	enum class EAttackWay
	{
		eIdle,         // 待機

		eUpAttackS,	   // 弱上攻撃
		eUpAttackM,	   // 中上攻撃
		eUpAttackL,	   // 強上攻撃

		eDwonAttackS,  // 弱下攻撃
		eDwonAttackM,  // 中下攻撃
		eDwonAttackL,  // 強下攻撃

		eRightAttackS, // 弱右攻撃
		eRightAttackM, // 中右攻撃
		eRightAttackL, // 強右攻撃

		eLeftAttackS,  // 弱左攻撃
		eLeftAttackM,  // 中左攻撃
		eLeftAttackL,  // 強左攻撃,
	};
	EAttackWay mAttackWay; // 攻撃方向

	// 攻撃の強さ
	enum class EAttackPower
	{
		eAttackS,   // 弱攻撃
		eAttackM,   // 中攻撃
		eAttackL,   // 強攻撃

		Num,        // 攻撃の強さの数
	};
	EAttackPower mAttackPower; // 攻撃の強さ

	// モード選択
	enum class EMode
	{
		eNotBattle, // 非戦闘モード
		eBattle,    // 戦闘モード
	};
	EMode mMode;  //モード選択

	bool IsBattleMode;

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;	// 接地しているかどうか
	CVector mGroundNormal;	// 接地している地面の法線

	CColliderLine* mpColliderLine;  // 縦方向の線分コライダー
	CColliderLine* mpColliderLineX; // 横方向（X軸）の線分コライダー
	CColliderLine* mpColliderLineZ; // 横方向（Z軸）の線分コライダー
	CColliderCapsule* mpColliderCapsule;  //カプセルコライダー
	CTransform* mpRideObject;

	CVector mEvaStartPos; // 回避開始時の位置
	CVector mEvaEndPos;   // 回避終了時の位置

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	// 火炎放射エフェクト
	CFlamethrower* mpFlamethrower;

	// モーションブラーを掛ける残り時間
	float mMotionBlurRemainTime;
};
