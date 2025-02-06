#ifndef CENEMYBASE_H
#define CENEMYBASE_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "Cmodel.h"

class CGaugeUI3D;

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

	// コンストラクタ
	CEnemyBase();
	//デストラクタ
	virtual ~CEnemyBase();

	//オブジェクトを削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

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

protected:

	// 指定のステートか
	virtual bool IsState(int state);

	// 敵の初期化
	void InitEnemy(std::string path, const std::vector<AnimData>* pAnimData);

	// 状態切り替え
	virtual void ChangeState(int state);

	// 攻撃タイプ切り替え
	virtual void ChangeAttackType(int attacktype);

	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);

	int mState;           // 状態
	int mAttackType;	  // 攻撃タイプ
	int mStateStep;       // 状態内のステップ管理用
	float mElapsedTime;   // 経過時間計測用

	// アニメーションデータのテーブル
	const std::vector<AnimData>* mpAnimData;

	CVector mMoveSpeed;    // 前後左右の移動速度
	float mMoveSpeedY;     // 重力やジャンプによる上下の移動速度

	bool mIsGrounded;      // 接地しているかどうか
	CVector mGroundNormal; // 接地している地面の法線

	CCollider* mpBodyCol;  // 本体のコライダー

	CGaugeUI3D* mpHpGauge;    // HPゲージ
	CVector mGaugeOffsetPos;  // ゲージのオフセット座標


};

#endif // !CENEMYBASE_H

