#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

class CImage;
class CExpandButton;

class CGameClear : public CTask
{
public:
	// コンストラクタ
	CGameClear();
	// デストラクタ
	~CGameClear();

	// クリア画面終了か
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

	enum class EState
	{
		eOpen,
		eSelect
	};
	void ChangeState(EState state);

	// 待機状態
	void UpdateOpen();
	// メニュー選択
	void UpdateSelect();

	// [ReStart]クリック時のコールバック関数
	void OnClickReStart();
	// [QUIT]クリック時のコールバック関数
	void OnClickQuit();

	EState mState;		
	int mStateStep;		// 状態内でのステップ管理
	int mSelectIndex;	// 現在選択している項目
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// クリア画面終了フラグ

	CImage* mpBackground;	// ゲームクリア背景イメージ
	std::vector<CExpandButton*> mButtons;
};
