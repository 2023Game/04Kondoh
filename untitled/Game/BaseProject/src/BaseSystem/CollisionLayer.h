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

	eAttackCol,		// 攻撃レイヤー
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
