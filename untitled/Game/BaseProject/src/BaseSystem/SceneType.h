#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属していない

	eTitle,	//タイトルシーン
	eGame,	//ゲームシーン
	eGameOver, // ゲームオーバーシーン
	eGameClear,

	eBootMenu,	//起動時メニュー
	eColTest,	//衝突判定テスト
};
