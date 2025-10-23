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
	CEnemyA(const CVector& pos, std::vector<CVector> patrolPoints);
	~CEnemyA();

	// 必要なリソースを読み込み
	static void LoadResources();

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

	// 回避中かどうか
	bool IsAvoiding() const override;
	// 防御中かどうか
	bool IsGuarding() const override;

	// ダメージを受ける
	void TakeDamage(int damage,float stan, float knockback, CObjectBase* causer) override;

	// パリィをされた時の処理
	void Parry() override;
	// 仰け反り処理
	void Hit() override;
	// 怯み処理
	void Stun() override;
	// 死亡処理
	void Death() override;

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
		eBattleIdle,	// 戦闘待機
		eWalk,			// 歩行
		eRun,			// 走る
		eStun,			// 混乱
		eStunWait,		// 混乱待ち時間
		eDeath,			// 死亡

		eBlowL,			// 左薙ぎ払い
		eBlowR,			// 右薙ぎ払い
		eRoundKickL,	// 左回し蹴り
		eRoundKickR,	// 右回し蹴り
		eTackleStart,	// タックルの溜め
		eTackle,		// タックル
		eTackleEnd,		// タックルの待ち時間
		eHeadButt,		// 頭突き攻撃

		eBattleWalkL,	// 戦闘時の左歩行
		eBattleWalkR,	// 戦闘時の右歩行

		eGuardIdle,		// 防御待機状態
		eGuardHit,		// 防御時のヒット
		eLeftAvoid,		// 左回避
		eRightAvoid,	// 右回避

		eHit1,			// 仰け反り1
		eHit2,			// 仰け反り2
		eHit3,			// 仰け反り3
		eChanceHit1,	// チャンス時の仰け反り1
		eChanceHit2,	// チャンス時の仰け反り2
		eChanceHit3,	// チャンス時の仰け反り3
		eChanceHit4,	// チャンス時の仰け反り4

		Num
	};
	EAnimType mAnimType;

	// 敵の状態
	enum class EState
	{
		eIdle,			// 待機
		ePatrol,		// 巡回中

		eBattleIdle,	// 待機（戦闘）
		eChase,			// 追跡中
		eLost,			// 見失う

		eAttack,		// 攻撃

		eGuard,			// 防御
		eAvoid,			// 回避

		eKnockBack,		// ノックバック
		eHit,			// 仰け反り
		eStun,			// 混乱
		eStunWait,		// 混乱待ち時間
		eParried,		// パリィされた時
		eDeath,			// 死亡
	};
	//状態切り替え
	void ChangeState(int state) override;

	// 攻撃タイプ
	enum class EAttackType
	{
		eNone,			// 指定なし

		eBlowL,			// 左薙ぎ払い
		eBlowR,			// 右薙ぎ払い
		eRoundKickL,	// 左回し蹴り
		eRoundKickR,	// 右回し蹴り
		eTackleStart,	// タックルの溜め
		eTackle,		// タックル
		eTackleEnd,		// タックル終了時の予備動作
		eHeadButt,		// 頭突き攻撃
		eTripleAttack,	// 三連攻撃
	};
	// 攻撃タイプ切り替え
	void ChangeAttackType(int attacktype) override;

	// TODO:待機行動のクラスを作る

	// ダメージ計算
	void CalcDamage(CCharaBase* taker, int* outDamage, float* outStan, float* outKnockback) const;

	/// <summary>
	/// 攻撃出来るかどうか
	/// </summary>
	/// <param name="range">攻撃範囲を設定</param>
	/// <returns>攻撃範囲より内側：true</returns>
	bool CanAttackPlayer(float range) const;

	// 移動する攻撃範囲か？
	bool IsMoveAttackRange();

	// プレイヤーの攻撃を検知したか？
	bool IsPlayerAttackDetected() const;
	// プレイヤーの攻撃範囲内か？
	bool IsPlayerAttackRange() const;

	/// <summary>
	/// プレイヤーの攻撃を検知時の処理
	/// </summary>
	/// <returns>trueの場合は、状態が変わった</returns>
	bool DetectedPlayerAttack();

	// どの攻撃をするか判定する
	void AttackPickDetect();

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 頭の正面方向ベクトルを取得
//	CVector GetHeadForwardVec() const;

	// 次に巡回するポイントを変更
	bool ChangePatrolPoint();
	// 巡回ルートを更新する
	bool UpdatePatrolRoute();


	// ↓ステートベースAI↓

	// 待機状態の更新処理
	void UpdateIdle();
	// 巡回中の更新処理
	void UpdatePatrol();

	// 戦闘待機状態の更新処理
	void UpdateBattleIdle();
	// 追跡中の更新処理
	void UpdateChase();
	// 見失った時の更新処理
	void UpdateLost();

	// 攻撃時の更新処理
	void UpdateAttack();

	// 防御時の更新処理
	void UpdateGuard();
	// 回避時の更新処理
	void UpdateAvoid();


	// 仰け反り時の更新処理
	void UpdateHit();
	// 混乱時の更新処理
	void UpdateStun();
	// 混乱待ちの更新処理
	void UpdateStunWait();
	// パリィされた時の更新処理
	void UpdateParried();
	// 死亡時の更新処理
	void UpdateDeath();


	// ↓攻撃ごとの更新処理↓

	// 薙ぎ払い攻撃
	void UpdateBlowL();
	void UpdateBlowR();
	// 回し蹴り攻撃
	void UpdateRoundKickL();
	void UpdateRoundKickR();
	// タックル
	void UpdateTackleStart();
	void UpdateTackle();
	void UpdateTackleEnd
	();
	// 押し出し攻撃
	void UpdateHeadButt();
	// 三連攻撃
	void UpdateTripleAttack();


	// 待機行動の更新処理
	// ↓行動を自然にする為の更新処理↓

	// 横移動(horizontal)
	void UpdateHorizonMove();
	// 前進行動(forward)
	void UpdateForwardMove();
	// 後退行動(back)
	void UpdateBackMove();

	// 状態の文字列を取得
	std::string GetStateStr(int state) const;
	// 攻撃タイプの文字列取得
	std::string GetAttackTypeStr(int state) const;
	// 状態の色を取得
	CColor GetStateColor(int state) const;

	EAnimType GetAnimType() const;
	int mAvoidAnim;

	CDebugFieldOfView* mpDebugAttack;  // 攻撃範囲のデバッグ表示
	// プレイヤーの攻撃範囲の角度
	float mPlayerAttackAngle;
	// プレイヤーの攻撃範囲の距離
	float mPlayerAttackLength;

	CDebugFieldOfView* mpDebugFov;  // 視野範囲のデバッグ表示

	float mRandMoveAngle;	// ランダムの移動角度
	int mRandHitAnim;		// ランダムな仰け反りアニメーション

	CVector mMoveStartPos; // 攻撃開始時の位置
	CVector mMoveEndPos;   // 攻撃終了時の位置


	// 左手の球コライダ
	CColliderSphere* mpLHandCol; 
	// 右手の球コライダ
	CColliderSphere* mpRHandCol;
	// 左足の球コライダ
	CColliderSphere* mpLFootCol;
	// 右足の球コライダ
	CColliderSphere* mpRFootCol;
	// 攻撃用のコライダ４（頭の部分）
	CColliderSphere* mpHeadCol;

	// 戦闘相手
	CObjectBase* mpBattleTarget;

#if _DEBUG
	CMatrix mHeadForwardMtx;
#endif


	bool mIsBattle;			// 戦闘状態か
	bool mIsGuard;			// ガード状態か
	bool mIsAvoid;			// 回避状態か
	bool mIsTripleAttack;	// 三連攻撃状態か

	int mAttackCount;		// 今の攻撃の回数
	int mTackleCount;		// タックルが出来るまでのカウント

	// プレイヤーの攻撃を既に検知済みである
	bool mIsDetectedPlayerAttack;
	

	// プレイヤーを見失った位置のノード
	CNavNode* mpLostPlayerNode; 
	// 巡回ポイントのリスト
	std::vector<CNavNode*> mPatrolPoints;
	// 求めた最短経路記憶用
	std::vector<CNavNode*> mMoveRoute;
	// 次に巡回するポイントの番号
	int mNextPatrolIndex;
	// 次に移動するノードのインデックス値
	int mNextMoveIndex;		
};
#endif
