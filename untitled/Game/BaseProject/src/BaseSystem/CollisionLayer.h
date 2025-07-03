#pragma once
#include <initializer_list>

// 衝突判定レイヤー
enum class ELayer
{
	eNone = -1,
	eTest,
	eField,
	eWall,
	ePlayer,
	eEnemy,
	eInteractObj,	// 調べるオブジェクト
	eInteractSearch,// 調べつオブジェクト探知用
	eObj,	// 普通のオブジェクト

	eAttackCol,	// 攻撃レイヤー
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
