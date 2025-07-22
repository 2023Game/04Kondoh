#include "CGameClear.h"
#include "CExpandButton.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CSceneManager.h"
#include "CInput.h"
#include "Easing.h"

#define CLEAR_ALPHA 1.0f
// ゲームクリアメニューのアニメーション時間
#define OPENED_ANIM_TIME 0.75f
// ゲームクリアメニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

CGameClear::CGameClear()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mSelectIndex(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
	, mIsOpened(false)
{

	mpBackground = new CImage
	(
		"UI/clear_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetAlpha(0.0f);

	mpClearText = new CImage
	(
		"UI/clear_gameclear.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpClearText->SetCenter(mpClearText->GetSize() * 0.5f);
	mpClearText->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpClearText->SetAlpha(0.0f);

	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.8f, 650.0f),
		CVector2(220.0f, 90.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn1->LoadButtonImage("UI/title_quit.png", "UI/title_quit.png");
	btn1->SetOnClickFunc(std::bind(&CGameClear::OnClickQuit, this));
	btn1->SetScale(1.0f);
	mButtons.push_back(btn1);

	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.3f, 650.0f),
		CVector2(220.0f, 90.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn2->LoadButtonImage("UI/clear_restart.png", "UI/clear_restart.png");
	btn2->SetOnClickFunc(std::bind(&CGameClear::OnClickReStart, this));
	btn2->SetScale(1.0f);
	mButtons.push_back(btn2);

	SetEnable(false);
	SetShow(false);
}

CGameClear::~CGameClear()
{
}

void CGameClear::Open()
{
	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	SetEnable(true);
	SetShow(true);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

void CGameClear::Close()
{
	// タイトル画面はカーソル表示
	CInput::ShowCursor(false);
	SetEnable(false);
	SetShow(false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

bool CGameClear::IsOpened() const
{
	return mIsOpened;
}

bool CGameClear::IsEnd() const
{
	return mIsEnd;
}

bool CGameClear::IsStartGame() const
{
	return mSelectIndex == 0;
}

bool CGameClear::IsExitGame() const
{
	return mSelectIndex == 1;
}

void CGameClear::Update()
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

	mpBackground->Update();
	mpClearText->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

void CGameClear::Render()
{
	mpBackground->Render();
	mpClearText->Render();

	for (CButton* btn : mButtons)
	{
		btn->Render();
	}


}

void CGameClear::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CGameClear::OnClickReStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;

	CSceneManager::Instance()->LoadScene(EScene::eTitle);
}

void CGameClear::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;

	System::ExitGame();
}

void CGameClear::UpdateOpen()
{
	switch (mStateStep)
	{
	case 0:
		if (mElapsedTime < OPENED_ANIM_TIME)
		{
			float alpha = Easing::QuartInOut(mElapsedTime, OPENED_ANIM_TIME, 0.0f, 0.6f);
			mpBackground->SetAlpha(alpha);
			mpClearText->SetAlpha(alpha);
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			mpBackground->SetAlpha(0.6f);
			mpClearText->SetAlpha(1.0f);
			mElapsedTime = 0.0f;
			mStateStep++;
		}
		break;
	case 1:
		if (mElapsedTime < OPENED_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangeState(EState::eSelect);
		}
		break;
	}
}

void CGameClear::UpdateSelect()
{

}
