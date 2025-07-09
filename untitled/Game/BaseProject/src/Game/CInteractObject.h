#pragma once
#include "CObjectBase.h"

// 調べるオブジェクトのベースクラス（抽象クラス）
class CInteractObject : public CObjectBase
{
public:
	// コンストラクタ
	CInteractObject(
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);

	// デストラクタ
	virtual ~CInteractObject();

	// 調べられる状態かどうか
	virtual bool CanInteract() const;

	// 調べる（絶対！！継承先で実装）
	// 純粋仮想関数 (関数() = 0;)
	virtual void Interact() = 0;
	// インタラクトオブジェクトがオンになったか
	virtual bool IsOnInteractObj() const;

	// 調べる内容のテキストを返す
	std::string GetInteractStr() const;



#if _DEBUG	
	// デバッグ表示用の名前を取得
	std::string GetDebugName() const;
	// デバッグ表示用の名前を設定
	void SetDebugName(std::string name);
#endif

protected:

	bool mIsPlaying;	// アニメーション再生中か
	bool mIsOnIntaractObj;	// オンになっているか

	// 入力オブジェクトのリスト
	std::list<CInteractObject*> mpInputObjs;

	std::string mInteractStr;	// 調べる内容のテキスト



#if _DEBUG
	std::string mDebugName;	// デバッグ表示用
#endif
};
