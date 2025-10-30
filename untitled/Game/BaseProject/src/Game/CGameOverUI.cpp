#include "CGameOverUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// ゲームオーバメニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f 
// ゲームオーバメニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

CGameOverUI::CGameOverUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mSelectIndex(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// ゲームオーバ画面の背景イメージを生成
	mpGameOverBg = new CImage
	(
		"UI/GameOver_bg.jpg",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// [CONTINUE]ボタンを生成
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(220.0f, 140.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn1->LoadButtonImage("UI/title_start.png", "UI/title_start.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CGameOverUI::OnClickContinue, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn1->SetEnable(false);
	btn1->SetScale(0.0f);
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// [QUIT]ボタンの生成
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 650.0f),
		CVector2(220.0f, 140.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn2->LoadButtonImage("UI/title_quit.png", "UI/title_quit.png");
	btn2->SetOnClickFunc(std::bind(&CGameOverUI::OnClickQuit, this));
	btn2->SetEnable(false);
	btn2->SetScale(0.0f);
	mButtons.push_back(btn2);
}

CGameOverUI::~CGameOverUI()
{
	SAFE_DELETE(mpGameOverBg);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

bool CGameOverUI::IsEnd() const
{
	return mIsEnd;
}

bool CGameOverUI::IsStartGame() const
{
	return mSelectIndex == 0;
}

bool CGameOverUI::IsExitGame() const
{
	return mSelectIndex == 1;
}

void CGameOverUI::Update()
{
	switch (mState)
	{
	case EState::eOpen:
		UpdateOpen();
		break;
	case EState::eSelect:
		UpdateSelect();
		break;
	}

	mpGameOverBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

void CGameOverUI::Render()
{
	mpGameOverBg->Render();

	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

void CGameOverUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CGameOverUI::OnClickContinue()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

void CGameOverUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}

// 待機状態
void CGameOverUI::UpdateOpen()
{
	switch (mStateStep)
	{
	case 0:
		if (mElapsedTime < OPEN_ANIM_TIME)
		{
			// スケール値を一端1.0より大きくして、1.0へ戻るイージングアニメーション
			float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(scale);
			}
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(1.0f);
			}
			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
	// ステップ１：
	case 1:
		if (mElapsedTime < OPENED_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// 一定時間待ったら、ボタンをオンにしてタッチできる
			// （誤タップを防ぐための待ち時間）
			for (CExpandButton* btn : mButtons)
			{
				btn->SetEnable(true);
			}
			ChangeState(EState::eSelect);
		}
	}
}

// メニュー選択
void CGameOverUI::UpdateSelect()
{
}