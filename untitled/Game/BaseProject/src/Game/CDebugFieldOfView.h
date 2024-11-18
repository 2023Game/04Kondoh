#pragma once
#include "CObjectBase.h"

// 視野範囲を表示するデバッグ機能
class CDebugFieldOfView : public CObjectBase
{
public:
	// コンストラクタ
	CDebugFieldOfView(CObjectBase* owner,
		float fovAngle, float fovLength);
	// デストラクタ
	~CDebugFieldOfView();

	// 視野範囲を表示する対象を設定
	void SetOwner(CObjectBase* owner);
	// 視野範囲を設定
	void Set(float angle, float length);

	// 更新処理
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;  // 表示する対象のポインタ
	float mFovAngle;       // 視野範囲の角度（ディグリー）
	float mFovLength;      // 視野範囲の距離

};