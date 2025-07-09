#include "CInteractObject.h"

// コンストラクタ
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, sortOrder, pause)
	, mInteractStr("調べる")
	, mIsPlaying(false)
	, mIsOnIntaractObj(false)

#if _DEBUG	
	, mDebugName("InteractObj")
#endif
{
}

// デストラクタ
CInteractObject::~CInteractObject()
{
}

// 調べられる状態かどうか
bool CInteractObject::CanInteract() const
{
	return true;
}

bool CInteractObject::IsOnInteractObj() const
{
	return mIsOnIntaractObj;
}


// 調べる内容のテキストを返す
std::string CInteractObject::GetInteractStr() const
{
	return mInteractStr;
}


#if _DEBUG	

// デバッグ表示用の名前を取得
std::string CInteractObject::GetDebugName() const
{
	return mDebugName;
}

// デバッグ表示用の名前を設定
void CInteractObject::SetDebugName(std::string name)
{
	mDebugName = name;
}

#endif
