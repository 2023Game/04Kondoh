#include "CClearUI.h"

CClearUI::CClearUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mSelectIndex(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
}

CClearUI::~CClearUI()
{
}

bool CClearUI::IsEnd() const
{
	return false;
}

bool CClearUI::IsStartGame() const
{
	return false;
}

bool CClearUI::IsExitGame() const
{
	return false;
}

void CClearUI::Update()
{
}

void CClearUI::Render()
{
}

void CClearUI::OnClickReStart()
{
}

void CClearUI::OnClickQuit()
{
}
