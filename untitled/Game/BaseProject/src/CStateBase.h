#pragma once

class CEnemyBase;

// 書く状態のベースクラス
class CStateBase {
public:

	// コンストラクタ
	explicit CStateBase(const std::string& name, CEnemyBase* owner);
	// デストラクタ
	virtual ~CStateBase() = default;

	// 状態の処理が終わったかどうか
	bool IsEnd() const;

	// 状態番号を設定
	void SetIndex(int index);

	// 状態の名前を取得
	const std::string& GetName() const;

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
};