#include "CGameClear.h"
#include "CExpandButton.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "Easing.h"

#define CLEAR_ALPHA 0.5f
// ゲームクリアメニューのアニメーション時間
#define OPENED_ANIM_TIME 0.25f
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
		"UI/clear_back",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0, 1.0f, CLEAR_ALPHA);
	mpBackground->SetAlpha(0.0f);

	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(220.0f, 140.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn1->LoadButtonImage("UI/title_quit.png", "UI/title_quit.png");
	btn1->SetOnClickFunc(std::bind(&CGameClear::OnClickQuit, this));
	btn1->SetScale(0.0f);
	mButtons.push_back(btn1);

	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 650.0f),
		CVector2(220.0f, 140.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn2->LoadButtonImage("UI/clear_restart.png", "UI/clear_restart.png");
	btn2->SetOnClickFunc(std::bind(&CGameClear::OnClickReStart, this));
	btn2->SetScale(0.0f);
	mButtons.push_back(btn2);

	SetEnable(false);
	SetShow(false);
}

CGameClear::~CGameClear()
{
}

void CGameClear::Open()
{
	SetEnable(true);
	SetShow(true);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

void CGameClear::Close()
{
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
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

void CGameClear::Render()
{
	mpBackground->Render();

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
	mIsEnd = true;
}

void CGameClear::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}

void CGameClear::UpdateOpen()
{
	switch (mStateStep)
	{
	case 0:
		if (mElapsedTime < OPENED_ANIM_TIME)
		{
			float alpha = Easing::BackInOut(mElapsedTime, OPENED_ANIM_TIME, 0.0f, 0.6f, 1.0f);
			mpBackground->SetAlpha(alpha);
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			mpBackground->SetAlpha(0.6f);
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
