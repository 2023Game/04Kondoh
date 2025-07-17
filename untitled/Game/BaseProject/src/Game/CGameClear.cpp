#include "CGameClear.h"
#include "CExpandButton.h"

CGameClear::CGameClear()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mSelectIndex(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	mpBackground = new CImage
	(
		"UI/clear_back",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);

	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(220.0f, 140.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	//btn1
}

CGameClear::~CGameClear()
{
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


}

void CGameClear::Render()
{
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

}

void CGameClear::UpdateSelect()
{

}
