#pragma once
#include "CObjectBase.h"

class CSingleDoor;
class CInteractObject;

class CDoubleDoors : public CObjectBase
{
public:
	// コンストラクタ
	CDoubleDoors(const CVector& posL,const CVector& posR,
				 const CVector& angle, const CVector& size);
	// デストラクタ
	~CDoubleDoors();

	// 接続するスイッチを追加
	void AddInputObjs(CInteractObject* sw);
	// 左扉の開閉した時の各座標を設定
	void SetAnimPosL(const CVector& openPosL, const CVector& closePosL);
	// 右扉の開閉した時の各座標を設定
	void SetAnimPosR(const CVector& openPosR, const CVector& closePosR);
	// 更新処理
	void Update();

private:

	bool mIsOpened;

	CSingleDoor* mpLDoor;
	CSingleDoor* mpRDoor;
};