#ifndef CENEMYA_H
#define CENEMYA_H
#include "CXCharacter.h"
#include "CCharaBase.h"
#include "CEnemyBase.h"
#include "CCollider.h"
#include "CModel.h"

// 視野範囲のデバッグ表示クラスの前宣言
class CDebugFieldOfView;
class CNavNode;

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemyA : public CEnemyBase
{

public:
	// コンストラクタ
	CEnemyA(std::vector<CVector> patrolPoints);
	~CEnemyA();
	// オブジェクト削除処理
	void DeleteObject(CObjectBase* obj) override;
	// 更新処理
	void Update() override;
	// 描画
	void Render() override;

	// 指定のステートか
	bool IsState(int state) override;
	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// ダメージを受ける
	void TakeDamage(int damage,float stan, CObjectBase* causer) override;
	// 死亡
	void Death() override;
	// 怯み処理
	void Stan() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,			// Tポーズ
		eIdle,			// 待機
		eWalk,			// 歩行
		eRun,			// 走り
		eDeath,			// 死亡
		eStan,			// 気絶アニメーション

		eRightAttackS,	// 右弱攻撃
		eRightAttackM,	// 右中攻撃
		eLeftAttackS,	// 左弱攻撃
		eLeftAttackM,	// 左中攻撃
		eUpAttackS,		// 上弱攻撃
		eUpAttackM,		// 上中攻撃
		eDownAttackS,	// 下弱攻撃
		eDownAttackM,	// 下中攻撃

		eHit1,			// 仰け反り1
		eHit2,			// 仰け反り2
		eHit3,			// 仰け反り3
		eChanceHit1,	// チャンス時の仰け反り1
		eChanceHit2,	// チャンス時の仰け反り2
		eChanceHit3,	// チャンス時の仰け反り3
		eChanceHit4,	// チャンス時の仰け反り4

		Num
	};

	// 敵の状態
	enum class EState
	{
		eIdle,			// 待機
		ePatrol,		// 巡回中
		eChase,			// 追跡中
		eLost,			// 見失う
		eAttack,		// 攻撃
		eGuardParry,	// 防御でパリー
		eAttackParry,	// 攻撃でパリー
		eStan,			// 怯む
		eDeath,			// 死亡
	};
	//状態切り替え
	void ChangeState(int state) override;

	enum class EAttackType
	{
		eRightAttack,	// 右攻撃
		eLeftAttack,	// 左攻撃
		eUpAttack,		// 上攻撃
		eDownAttack,	// 下攻撃

		//eLeftAttackS,		// 左弱攻撃
		//eLeftAttackM,		// 左中攻撃
		//eRightAttackS,	// 右弱攻撃
		//eRightAttackM,	// 右中攻撃
		//eUpAttackS,		// 上弱攻撃
		//eUpAttackM,		// 上中攻撃
		//eDownAttackS,		// 下弱攻撃
		//eDownAttackM,		// 下中攻撃
	};
	// 攻撃タイプ切り替え
	void ChangeAttackType(int attacktype) override;

	// プレイヤーが視野範囲内に入ったかどうか
	bool IsFoundPlayer() const;
	// 現在位置からプレイヤーが見えているかどうか
	bool IsLookPlayer() const;
	// プレイヤーを攻撃出来るかどうか
	bool CanAttackPlayer() const;
	// 攻撃時に移動する距離か
	bool AttackRangeMin();

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);
	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 次に巡回するポイントを変更
	void ChangePatrolPoint();

	// ↓ステートベースAI↓

	// 待機状態の更新処理
	void UpdateIdle();
	// 巡回中の更新処理
	void UpdatePatrol();
	// 追跡中の更新処理
	void UpdateChase();
	// 見失った時の更新処理
	void UpdateLost();
	// 攻撃時の更新処理
	void UpdateAttack();
	// 死亡時の更新処理
	void UpdateDeath();
	// 攻撃パリー
	void UpdateGuardParry();
	// 
	void UpdateAttackParry();
	// 怯み時の更新処理
	void UpdateStan();

	// 状態の文字列を取得
	std::string GetStateStr(int state) const;
	// 状態の色を取得
	CColor GetStateColor(int state) const;


	CDebugFieldOfView* mpDebugFov;  // 視野範囲のデバッグ表示
	float mFovAngle;                // 視野範囲の角度
	float mFovLength;                // 視野範囲の距離

	CNavNode* mpLostPlayerNode;  // プレイヤーを見失った位置のノード

	CVector mAttackStartPos; // 攻撃開始時の位置
	CVector mAttackEndPos;   // 攻撃終了時の位置

	// 巡回ポイントのリスト
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex; // 次に巡回するポイントの番号


	// 左手の球コライダ
	CColliderSphere* mpLAttackCol; 
	// 右手の球コライダ
	CColliderSphere* mpRAttackCol;

	std::vector<CNavNode*> mMoveRoute;	// 求めた最短経路記憶用
	int mNextMoveIndex;					// 次に移動するノードのインデックス値

	CObjectBase* mpBattleTarget;	// 戦闘相手
	bool mIsBattle;					// 戦闘状態か

	int mpDetectType;	// 攻撃タイプ
	bool mIsParry;	// スタンしているか
};
#endif
