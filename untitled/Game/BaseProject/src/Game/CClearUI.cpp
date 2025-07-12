#include "CClearUI.h"
#include "CExpandButton.h"

CClearUI::CClearUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mSelectIndex(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(220.0f, 140.0f),
		ETaskPriority::eUI, ETaskPauseType::eGame,
		false, false
	);
	//btn1
}

CClearUI::~CClearUI()
{
}

bool CClearUI::IsEnd() const
{
	return mIsEnd;
}

bool CClearUI::IsStartGame() const
{
	return mSelectIndex == 0;
}

bool CClearUI::IsExitGame() const
{
	return mSelectIndex == 1;
}

void CClearUI::Update()
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


}

void CClearUI::Render()
{
}

void CClearUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CClearUI::OnClickReStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

void CClearUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}

void CClearUI::UpdateOpen()
{

}

void CClearUI::UpdateSelect()
{

}
