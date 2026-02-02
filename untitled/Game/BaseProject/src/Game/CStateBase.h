#pragma once
#include <functional>

class CEnemyBase;

// 遷移条件の関数ポインタをCondFuncと命名
using CondFunc = std::function<bool()>;

// 状態の遷移情報
struct Transition
{
	int nextStateId = -1;			// 次に遷移するステート番号 
	bool isWaitEnd = false;			// 遷移条件を満たしても、現在のステートが終了するまで待つかどうか
	CondFunc condition = nullptr;	// 遷移条件の関数ポインタ
};

struct AttackTrans
{
	int nextAttackId = -1;			// 次に遷移するアタックステート番号
	bool isWaitEnd = false;			// 遷移条件を満たしても、現在のステートが終了するまで待つかどうか？
	CondFunc condition = nullptr;	// 遷移条件の関数ポインタ
};

// 書く状態のベースクラス
class CStateBase {
public:

	// コンストラクタ
	explicit CStateBase(CEnemyBase* owner, const std::string& name);
	// デストラクタ
	virtual ~CStateBase() = default;

	// 状態の処理が終わったかどうか
	bool IsEnd() const;

	// 状態番号を設定
	void SetIndex(int index);
	// 状態番号を取得
	int GetIndex() const;

	// 状態の名前を取得
	const std::string& GetName() const;

	/// <summary>
	/// 他の状態への遷移条件を追加
	/// </summary>
	/// <param name="nextStateId">遷移先の状態の番号</param>
	/// <param name="isWaitEnd">現在の状態が終了するのを待つか</param>
	/// <param name="func">遷移条件の関数ポインタ</param>
	void AddTransition(int nextStateId, bool isWaitEnd = true, CondFunc func = nullptr);

	/// <summary>
	/// 他の攻撃状態への遷移条件を追加
	/// </summary>
	/// <param name="nextAttackStateId">遷移先の攻撃状態の番号</param>
	/// <param name="isWaitEnd">現在の攻撃状態が終了するのを待つか</param>
	/// <param name="func">遷移条件の関数ポインタ</param>
	void AddAttackTrans(int nextAttackId, bool isWaitEnd = true, CondFunc func = nullptr);

	/// <summary>
	/// 他の状態へ遷移する条件を満たしているか
	/// </summary>
	/// <returns>遷移条件を満たしていた場合は、遷移先の状態の番号を返す</returns>
	int CheckTransition() const;

	/// <summary>
	/// 攻撃ステートの遷移条件をみたしているか
	/// </summary>
	/// <returns>遷移条件を満たしていた場合は、遷移先の状態の番号を返す</returns>
	int CheckAttackTrans() const;

	// 開始処理（継承先で実装）
	virtual void Enter();
	// 更新処理（継承先で実装）
	virtual void Update();
	// 終了処理（継承先で実装）
	virtual void Exit();


protected:

	bool mIsEnd;	// この状態の処理が終わったか
	int mIndex;		// ステートマシンに登録されている状態番号

	std::string mStateName;	// この状態の名前
	CEnemyBase* mpOwner;	// この状態の持ち主
	CNavNode* mpNavNode;

	// 他の状態への遷移条件のリスト
	std::vector<Transition> mTransitions;
	// 他の攻撃状態への遷移条件のリスト
	std::vector<AttackTrans> mAttackTrans;
};