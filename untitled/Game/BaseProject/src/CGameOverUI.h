#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CGameOverUI : public CTask
{
public:
	// コンストラクタ
	CGameOverUI();
	// デストラクタ
	~CGameOverUI();

	// ゲームオーバ画面終了か
	bool IsEnd() const;
	// ゲームを開始するか
	bool IsStartGame() const;
	// ゲームを終了するか
	bool IsExitGame() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:

	// 待機状態
	void UpdateOpen();
	// メニュー選択
	void UpdateSelect();

	// ゲームオーバの状態
	enum class EState
	{
		eOpen,		// 待機状態
		eSelect,	// メニュー選択
	};
	// 状態切り替え
	void ChangeState(EState state);

	// [CONTINUE]クリック時のコールバック関数
	void OnClickContinue();
	// [QUIT]クリック時のコールバック関数
	void OnClickQuit();

	EState mState;
	int mStateStep;		// 状態内でのステップ管理
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// ゲームオーバ画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	CImage* mpGameOverBg;	// ゲームオーバ背景イメージ
	std::vector<CExpandButton*> mButtons;
};
