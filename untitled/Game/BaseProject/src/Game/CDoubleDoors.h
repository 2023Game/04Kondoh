#pragma once

class CInteractObject;

class CDoubleDoors : public CObjectBase
{
public:
	CDoubleDoors(CVector pos, CVector angle, CVector size);
	~CDoubleDoors();

	void AddInputObjs(CInteractObject* sw);

protected:
	// スイッチを押して扉が開くかどうか
	bool IsSwitchOn() const;

private:

	// 入力系のオブジェクトのリスト
	std::list<CInteractObject*> mpInputObjs;
};