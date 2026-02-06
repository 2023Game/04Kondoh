#ifndef CENEMYBASE_H
#define CENEMYBASE_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"
#include "CStateMachine.h"
#include "CStateAttack.h"

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
		int rangeNamber;
	};

	// 攻撃範囲データ
	struct AttackRangeData
	{
		float closeRange = 0.0f;	// 至近距離	
		float nearRange = 0.0f;		// 近距離
		float middleRange = 0.0f;	// 中距離
		float longRange = 0.0f;		// 遠距離
		float detectRange = 0.0f;	// 判定距離
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
	// ステートから呼び出すアニメーション切り替え処理
	virtual void ChangeStateAnimation(int stateIndex, int no = 0);

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

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);
	// プレイヤーが視野範囲内に入ったかどうか
	bool IsFoundPlayer() const;
	// 現在位置からプレイヤーが見えているかどうか
	bool IsLookPlayer() const;

	// プレイヤーの攻撃を検知したか？
	bool IsPlayerAttackDetected() const;

	// 攻撃範囲を追加（敵によって違うので）
	void AddAttackRange(float closeRng, float nearRng,float middleRng, float longRng, float detectRng);
	
	/// <summary>
	/// 攻撃範囲データの取得
	/// </summary>
	/// <param name="index">攻撃範囲のリストの要素数</param>
	/// <returns></returns>
	const AttackRangeData& GetRangeData(int index) const;

	/// <summary>
	/// 攻撃出来るかどうか
	/// </summary>
	/// <param name="range">攻撃範囲を設定</param>
	/// <returns>攻撃範囲より内側：true</returns>
	bool CanAttackPlayer(float range) const;
	// 移動する攻撃範囲か？
	bool IsMoveAttackRange(const AttackRangeData& range);

	/// <summary>
	/// プレイヤーの攻撃を検知時の処理
	/// </summary>
	/// <returns>trueの場合は、状態が変わった</returns>
	bool DetectedPlayerAttack();

	// 攻撃を攻撃範囲と確率で決めるための関数
	// TODO：ゆくゆくは、複数の条件で決めれるように成りたい
	void SelectAttack(const AttackRangeData& range, int rand);

	// 歩く速度を取得
	float GetWalkSpeed() const;
	// 走る速度を取得
	float GetRunSpeed() const;

	// 攻撃状態を取得
	int GetAttackType() const;
	// アタックデータを取得
	const std::vector<AttackData>* GetAttackData() const;
 
	// プレイヤーを見失った位置ノードの座標を取得
	CVector GetLostPlayerNodePos() const;
	// プレイヤーを見失った位置ノードの座標を設定
	void SetLostPlayerNodePos(const CVector& pos) const;

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);
	// 指定した位置まで経路探索で移動する
	bool NavMoveTo(const CVector& targetPos, float speed);

	// 移動速度を取得
	const CVector& GetMoveSpeed() const;
	// 移動速度を設定
	void SetMoveSpeed(const CVector& moveSpeed);

	// 戦闘状態を取得
	bool GetIsBattle() const;
	// 戦闘状態を設定
	void SetIsBattle(bool isbattle);

	// 戦闘相手を取得
	CObjectBase* GetBattleTarget() const;
	// 戦闘相手を設定
	void SetBattleTarget(CObjectBase* target);


protected:

	// 現在のステートを取得
	CStateBase* GetCurrentState() const;

	// 敵の初期化
	void InitEnemy(std::string path, const std::vector<AnimData>* pAnimData);

	/// <summary>
	/// 視野に関する情報を設定
	/// </summary>
	/// <param name="angle">視野範囲の角度</param>
	/// <param name="length">視野範囲の距離</param>
	/// <param name="eyeHeight">視野の高さ</param>
	/// <param name="lookAtSpeed">対象に向く速度</param>
	void SetFovs(float angle, float length, float eyeHeight, float lookAtSpeed);
	// 頭の正面方向ベクトルを取得
	CVector GetHeadForwardVec() const;

	float mFovAngle;	// 視野範囲の角度
	float mFovLength;	// 視野範囲の距離
	float mEyeHeight;	// 視野の高さ
	float mLookAtSpeed;	// 対象に向く速度

	int mAttackType;		// 攻撃タイプ
	float mElapsedTime;			// 経過時間計測用
	float mBattleElapsedTime;	// 攻撃用経過時間
	float mIdleTime;		// 待機時間
	float mBattleIdletime;	// 戦闘時の待機時間

	// アニメーションデータのテーブル
	const std::vector<AnimData>* mpAnimData;
	// 攻撃データのテーブル
	const std::vector<AttackData>* mpAttackData;
	// 攻撃範囲
	std::vector<AttackRangeData> mRangeData;

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 重力やジャンプによる上下の移動速度
	float mWalkSpeed;	// 歩く速度
	float mRunSpeed;	// 走る速度

	// プレイヤーの攻撃を既に検知済みである
	bool mIsDetectedPlayerAttack;

	bool mIsBattle;			// 戦闘状態か
	bool mIsGuard;			// ガード状態か
	bool mIsAvoid;			// 回避状態か

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

	// 戦闘相手
	CObjectBase* mpBattleTarget;
	// 攻撃ステートを取得
	CStateAttack* mpAttackState;

	// プレイヤーを見失った位置のノード
	CNavNode* mpLostPlayerNode;
	// 巡回ポイントのリスト
	std::vector<CVector> mPatrolPoints;

	// 移動先の経路探索用のノード
	CNavNode* mpMoveNavNode;
	// 求めた最短経路記憶用
	std::vector<CNavNode*> mMoveRoute;
	// 次に移動するノードのインデックス値
	int mNextMoveIndex;
	// 移動経路を更新するかどうか
	bool mIsUpdateMoveRoute;
};

#endif // !CENEMYBASE_H

