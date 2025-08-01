#pragma once
#include <initializer_list>

// オブジェクト識別用のタグ
enum class ETag
{
	eNone = -1,

	eField,	// フィールドを構成するオブジェクト
	eWall,	// 壁
	eDoor,	// 扉
	eRideableObject,	// 乗ることができるオブジェクト
	eInteractObject,	// 調べられるオブジェクト
	eItem,	// アイテムのオブジェクト

	ePlayer,// プレイヤー
	eEnemy,	// エネミー

	eBullet,// 弾丸
	eFlame,	// 炎
	eSlash,	// 斬撃

	eUI,

	eCamera,// カメラ

	eDebug, // デバッグ用のタグ

	Num
};
// オブジェクト識別用のタグの初期化リスト
using Tags = std::initializer_list<ETag>;
