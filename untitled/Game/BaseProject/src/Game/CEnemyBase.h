#ifndef CENEMYBASE_H
#define CENEMYBASE_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"
#include "CStateMachine.h"

class CGaugeUI3D;

// リスポーンポイント
struct RespawnData
{
	CVector respawnPoint;
	std::vector<CVector> patrolPoints;
};


/*
エネミーベースクラス
キャラクタークラスを継承
*/
class CEnemyBase : public CXCharacter
{
public:
	//アニメションデータ
	struct AnimData
	{
		std::string path;   // アニメーションデータのパス
		bool loop;          // ループするかどうか
		float frameLength;  // アニメーションのフレーム数
		float speed;        // アニメーションの再生速度（1.0で等倍）
	};

	// 攻撃データ
	struct AttackData
	{
		EAttackDir dir;					// 攻撃の方向
		EAttackPower power;				// 攻撃の強さ
		bool attackParry;				// アタックパリィ出来るかどうか
		bool guardParry;				// ガードパリィ出来るかどうか
		float attackParryStartFrame;	// アタックパリィ出来るアニメーション開始フレーム数
		float attackParryEndFrame;		// アタックパリィ出来るアニメーション終了フレーム数
		float guardParryStartFrame;		// ガードパリィ出来るアニメーション開始フレーム
		float guardParryEndFrame;		// ガードパリィ出来るアニメーション終了フレーム
	};


	// コンストラクタ
	CEnemyBase();
	//デストラクタ
	virtual ~CEnemyBase();

	//オブジェクトを削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	// パリィ
	virtual void Parry();

	// アタックパリィ出来るかどうか
	bool CheckAttackParry(EAttackDir dir, EAttackPower power) const override;
	// ガードパリィ出来るかどうか
	bool CheckGuardParry() const override;

	// 状態切り替え処理
	virtual void ChangeState(int state);
	// 攻撃タイプ切り替え処理
	virtual void ChangeAttackType(int attacktype);
	// アニメーション切り替え処理
	void ChangeAnimation(int type, int no = 0, bool restart = false);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 次に巡回するポイントを変更
	bool ChangePatrolPoint();
	// 巡回ルートを更新する
	bool UpdatePatrolRoute();
	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);
	// 巡回時の移動
	bool PatrolMove(const CVector& targetPos, float speed, bool cheace);
	// 巡回ポイントの

	// 次に巡回するポイント番号の設定
	int SetNextIndex(int index);

protected:

	// 指定のステートか
	virtual bool IsState(int state);

	// 敵の初期化
	void InitEnemy(std::string path, const std::vector<AnimData>* pAnimData);

	// プレイヤーが視野範囲内に入ったかどうか
	bool IsFoundPlayer() const;
	// 現在位置からプレイヤーが見えているかどうか
	bool IsLookPlayer() const;
	// 視野範囲と距離を設定
	void SetAngLeng(float angle, float length);
	// 頭の正面方向ベクトルを取得
	CVector GetHeadForwardVec() const;

	float mFovAngle;	// 視野範囲の角度
	float mFovLength;	// 視野範囲の距離
	float mEyeHeight;	// 視野の高さ

	int mState;				// 状態
	int mAttackType;		// 攻撃タイプ
	int mStateStep;			// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用
	float mBattleElapsedTime;	// 攻撃用経過時間
	float mIdleTime;		// 待機時間
	float mBattleIdletime;	// 戦闘時の待機時間

	// アニメーションデータのテーブル
	const std::vector<AnimData>* mpAnimData;
	// 攻撃データのテーブル
	const std::vector<AttackData>* mpAttackData;

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 重力やジャンプによる上下の移動速度

	bool mIsAttackParry;	// 攻撃パリィ
	bool mIsGuardParry;		// 防御パリィ

	bool mIsHitWall;		// 壁に当たったかどうか
	bool mIsGrounded;		// 接地しているかどうか
	CVector mGroundNormal;	// 接地している地面の法線

	CCollider* mpBodyCol;	// 本体のコライダー

	CGaugeUI3D* mpHpGauge;		// HPゲージ
	CVector mGaugeOffsetPos;	// ゲージのオフセット座標

	const CMatrix* mpHeadMtx;	// 頭の行列を取得

	// 敵の行動を管理するステートマシン
	CStateMachine mStateMachine;	


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

#endif // !CENEMYBASE_H

