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

	// ダメージ計算
	void CalcDamage(CCharaBase* taker, int* outDamage, float* outStan) const;

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,			// Tポーズ
		eIdle,			// 待機
		eBattleIdle,	// 戦闘待機
		eWalk,			// 歩行
		eRun,			// 走り
		eDeath,			// 死亡
		eStan,			// 気絶アニメーション

		eBlowL,			// 左薙ぎ払い
		eBlowR,			// 右薙ぎ払い
		eRoundKickL,	// 左回し蹴り
		eRoundKickR,	// 右回し蹴り
		eTackle,		// 竜巻旋風脚タックル
		eHeadButt,		// 押し出し攻撃

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

		eBattleIdle,	// 待機（戦闘）
		eChase,			// 追跡中
		eLost,			// 見失う

		eAttack,		// 攻撃

		eGuard,			// 防御
		eAvoid,			// 回避

		eStan,			// 怯む
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
		eTackle,		// 竜巻旋風脚タックル
		eHeadButt,		// 頭突き攻撃
		eTripleAttack,	// 三連攻撃
	};
	// 攻撃タイプ切り替え
	void ChangeAttackType(int attacktype) override;

	// プレイヤーが視野範囲内に入ったかどうか
	bool IsFoundPlayer() const;
	// 現在位置からプレイヤーが見えているかどうか
	bool IsLookPlayer() const;

	/// <summary>
	/// 攻撃出来るかどうか
	/// </summary>
	/// <param name="range">攻撃範囲を設定</param>
	/// <returns>攻撃範囲より内側：true</returns>
	bool CanAttackPlayer(float range) const;

	// 攻撃時に移動する距離か
	bool AttackRangeMin();

	// プレイヤーの攻撃を検知したか？
	bool IsPlayerAttackDetected() const;

	/// <summary>
	/// プレイヤーの攻撃を検知時の処理
	/// </summary>
	/// <returns>trueの場合は、状態が変わった</returns>
	bool DetectedPlayerAttack();

	// どの攻撃をするか判定する
	void AttackPickDetect();

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);
	// ランダムで移動
	bool RandMove(float speed);

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 次に巡回するポイントを変更
	void ChangePatrolPoint();

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

	// 死亡時の更新処理
	void UpdateDeath();
	// 怯み時の更新処理
	void UpdateStan();

	// ↓攻撃ごとの更新処理↓

	// 薙ぎ払い攻撃
	void UpdateBlowL();
	void UpdateBlowR();
	// 回し蹴り攻撃
	void UpdateRoundKickL();
	void UpdateRoundKickR();
	// 竜巻旋風脚タックル
	void UpdateTackle();
	// 押し出し攻撃
	void UpdateHeadButt();
	// 三連攻撃
	void UpdataTripleAttack();


	// 状態の文字列を取得
	std::string GetStateStr(int state) const;
	// 攻撃タイプの文字列取得
	std::string GetAttackTypeStr(int state) const;
	// 状態の色を取得
	CColor GetStateColor(int state) const;

	CDebugFieldOfView* mpDebugAttack;  // 攻撃範囲のデバッグ表示
	// プレイヤーの攻撃範囲の角度
	float mPlayerAttackAngle;
	// プレイヤーの攻撃範囲の距離
	float mPlayerAttackLength;

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

	int mAttackCount;		// 攻撃の回数
	bool mIsTripleAttack;	// 三連攻撃状態か

	/*
	int mpDetectType;	// 攻撃タイプ
	bool mIsParry;	// スタンしているか
	*/

	// プレイヤーの攻撃を既に検知済みである
	bool mIsDetectedPlayerAttack;
};
#endif
