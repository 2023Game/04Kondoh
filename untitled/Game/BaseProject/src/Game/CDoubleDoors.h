#pragma once

class CInteractObject;
class CLDoubleDoor;
class CRDoubleDoor;

class CDoubleDoors : public CObjectBase
{
public:
	// コンストラクタ
	CDoubleDoors();
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

protected:

	bool IsSwitchOn() const;

	CVector mOpenPosL;	// 左扉の開いた時の位置
	CVector mClosePosL;	// 左扉の閉まった時の位置
	CVector mOpenPosR;	// 右扉の開いた時の位置
	CVector mClosePosR;	// 右扉の閉まった時の位置

private:

	// 入力系のオブジェクトリスト
	std::list<CInteractObject*> mpInputObjs;

	CLDoubleDoor* mpLDoor;
	CRDoubleDoor* mpRDoor;

};