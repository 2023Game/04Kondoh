#ifndef CENEMYA_H
#define CENEMYA_H
#include "CXCharacter.h"
#include "CCharaBase.h"
#include "CCollider.h"
#include "CModel.h"

// 視野範囲のデバッグ表示クラスの前宣言
class CDebugFieldOfView;
class CNavNode;

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemyA : public CXCharacter
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

		eTPose,		     // Tポーズ
		eIdle,		     // 待機
		eWalk,		     // 歩行
		eRun,		     // 走り
		eRightAttack,    // 攻撃

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type, bool restart = false);

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// 敵の状態
	enum class EState
	{
		eIdle,		// 待機
		ePatrol,    // 巡回中
		eChase,     // 追跡中
		eLost,      // 見失う
		eAttack     // 攻撃
	};
	//状態切り替え
	void ChangeState(EState state);

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

	// 状態の文字列を取得
	std::string GetStateStr(EState state)const;
	// 状態の色を取得
	CColor GetStateColor(EState state) const;

	EState mState;	// 敵の状態
	int mStateStep; // 状態内のステップ管理用
	float mElapsedTime; // 経過時間計測用

	CDebugFieldOfView* mpDebugFov;  // 視野範囲のデバッグ表示
	float mFovAngle;                // 視野範囲の角度
	float mFovLength;                // 視野範囲の距離

	CNavNode* mpLostPlayerNode;  // プレイヤーを見失った位置のノード

	CVector mAttackStartPos; // 攻撃開始時の位置
	CVector mAttackEndPos;   // 攻撃終了時の位置

	// 巡回ポイントのリスト
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex; // 次に巡回するポイントの番号

	//CColliderLine* mpColliderLine;  // 縦方向の線分コライダー
	//CColliderLine* mpColliderLineX; // 横方向（X軸）の線分コライダー
	//CColliderLine* mpColliderLineZ; // 横方向（Z軸）の線分コライダー
	CColliderCapsule* mpColliderCapsule;  // カプセルコライダー

	std::vector<CNavNode*> mMoveRoute; // 求めた最短経路記憶用
	int mNextMoveIndex;                // 次に移動するノードのインデックス値
};
#endif
