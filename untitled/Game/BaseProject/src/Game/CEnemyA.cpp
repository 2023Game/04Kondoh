#include "CEnemyA.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"


// プレイヤーのアニメーションデータのテーブル
const CEnemyA::AnimData CEnemyA::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	{ "Character\\EnamyA\\Idle.x",		        true,	126.0f	},	// 待機
	{ "Character\\EnamyA\\walk.x",		true,	42.0f	},	// 歩行
	{ "Character\\EnamyA\\Run.x",		true,	23.0f	},	// 走る

};


// コンストラクタ
CEnemyA::CEnemyA()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	,mState(EState::eIdle)
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

CEnemyA::~CEnemyA()
{
}

//更新処理
void CEnemyA::Update()
{

	if (CInput::Key('U')) {
		ChangeAnimation(EAnimType::eWalk);
	}
	else if (CInput::Key('L')) {
		ChangeAnimation(EAnimType::eRun);
	}

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
