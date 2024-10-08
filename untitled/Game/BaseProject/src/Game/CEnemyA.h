#ifndef CENEMYA_H
#define CENEMYA_H
#include "CXCharacter.h"
#include "CCharaBase.h"
#include "CCollider.h"
#include "CModel.h"

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemyA : public CXCharacter
{
public:
	// コンストラクタ
	CEnemyA();
	~CEnemyA();
	// 更新処理
	void Update() override;
	// 描画
	void Render() override;
private:

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eWalk,		// 歩行

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];
};

#endif
