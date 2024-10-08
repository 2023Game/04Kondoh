#include "CEnemyA.h"
#include "CEffect.h"
#include "CCollisionManager.h"


// プレイヤーのアニメーションデータのテーブル
const CEnemyA::AnimData CEnemyA::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	{ "素材入れ\\EnamyA\\Idle.x",		        true,	88.0f	},	// 待機

};


// コンストラクタ
CEnemyA::CEnemyA()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
{
	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("EnemyA");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);
}

//更新処理
void CEnemyA::Update()
{
	CXCharacter::Update(); 
}

void CEnemyA::Render()
{
	CXCharacter::Render();
}

void CEnemyA::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return; 
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.frameLength);
}
