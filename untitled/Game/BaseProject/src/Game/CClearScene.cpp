#include "CClearScene.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "CSound.h"
#include "CBGMManager.h"

CClearScene::CClearScene()
	: CSceneBase(EScene::eGameClear)
	, mpClearUI(nullptr)
{
}

CClearScene::~CClearScene()
{
}

void CClearScene::Load()
{
	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f,0.0f,0.0f),

		)
}

void CClearScene::Update()
{

}
