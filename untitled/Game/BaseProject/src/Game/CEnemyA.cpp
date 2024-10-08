#include "CEnemyA.h"
#include "CEffect.h"
#include "CCollisionManager.h"


// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemyA::AnimData CEnemyA::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	{ "�f�ޓ���\\EnamyA\\Idle.x",		        true,	88.0f	},	// �ҋ@

};


// �R���X�g���N�^
CEnemyA::CEnemyA()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
{
	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("EnemyA");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);
}

//�X�V����
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
