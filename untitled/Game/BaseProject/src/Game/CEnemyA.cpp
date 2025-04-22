#include "CEnemyA.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CDebugFieldOfView.h"
#include "CPlayer.h"
#include "Maths.h"
#include "Primitive.h"
#include "CField.h"
#include "CNavNode.h"
#include "CNavManager.h"

#define ANIM_PATH "Character\\EnemyA\\anim\\"
#define ATTACK_ANIM_PATH "Character\\EnemyA\\anim\\Attack\\"
#define DAMAGEHIT_PATH "Character\\EnemyA\\anim\\DamageHit\\"
#define CHANCEHIT_PATH "Character\\EnemyA\\anim\\ChanceHit\\"

#define ENEMY_HEIGHT 16.0f      // �G�̍���
#define ENEMY_SOLE   5.0f       // �G�̒�
#define ENEMY_WIDTH  5.0f       // �G�̔��a
#define FOV_ANGLE 45.0f         // ����͈͂̊p�x
#define FOV_LENGTH 100.0f       // ����͈͂̋���
#define EYE_HEIGHT 10.0f        // ���_�̍���
#define WALK_SPEED 10.0f        // �����̑��x
#define RUN_SPEED 40.0f         // �����Ă��鎞�̑��x
#define ROTATE_SPEED 6.0f       // ��]���x

#define DISTANT_ATTACK_RANGE 50.0f	// �^�b�N���U���͈�
#define NEAR_ATTACK_RANGE 30.0f		// �ガ�����Ɖ񂵏R��A�O�A�U���͈̔�
#define HEADBUTT_ATTACK_RANGE 15.0f	// ���˂��U���͈�

#define BLOWL_PROB 20		// ���ガ�����̊m��
#define BLOWR_PROB 40		// �E�ガ�����̊m��
#define KICKL_PROB 50		// ���񂵏R��̊m��
#define KICKR_PROB 60		// �E�񂵏R��̊m��
#define TRIPLE_PROB 95		// �O�A�U���̊m��
#define TACKLE_PROB 40		// �^�b�N���̊m��
#define FIFTY_FIFTY_PROB 50	// �����̊m��

#define TACKLE_MOVE_DIST	50.0f	// �^�b�N�����̈ړ�����
#define TACKLE_MOVE_START	0.5f	// �^�b�N�����̈ړ��J�n�t���[��
#define TACKLE_MOVE_END		55.0f	// �^�b�N�����̈ړ��I���t���[��
#define KICK_MOVE_DIST		10.0f	// �񂵏R�莞�̈ړ�����
#define KICK_MOVE_START		1.0f	// �񂵏R�莞�̈ړ��J�n�t���[��
#define KICK_MOVE_END		50.0f	// �񂵏R�莞�̈ړ��I���t���[��
#define ATTACK_WAIT_TIME	1.0f	// �U���I�����̑҂�����

#define IDLE_TIME_MIN 0.0f			// �ҋ@���̍ŒZ�ҋ@����
#define IDLE_TIME_MAX 10.0f			// �ҋ@���̍Œ��ҋ@����
#define BATTLE_IDLE_TIME_MIN 1.0f	// �퓬�ҋ@���̍ŒZ�ҋ@����
#define BATTLE_IDLE_TIME_MAX 5.0f	// �퓬�ҋ@���̍Œ��ҋ@����

#define ATTACK_ANGLE		35.0f	// �U���͈͂̊p�x
#define ATTACK_LENGTH		50.0f	// �U���͈͂̋���

#define PATROL_INTERVAL 3.0f    // ���̏���Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST 10.0f  // ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME 5.0f          // �ҋ@��Ԃ̎���
#define DEATH_WAIT_TIME 3.0f	// ���S���̑ҋ@����
#define LOOKAT_SPEED 5.0f		// �Ώۂ̕����Ɍ������x

#define GUARD_PROB 90		// �h��̊m���i�p�[�Z���g�j

#define DETECT_COL_POS CVector(0.0f, 5.0f, 3.0f)	// ����p�R���C�_�̍��W
#define DETECT_COL_SCL CVector(0.7f, 1.5f, 0.7f)	// ����p�R���C�_�̑傫��

#define EATTACKWAY CPlayer::EAttackDir		// �v���C���[�̍U������
#define EATTACKPWOER CPlayer::EAttackPower	// �v���C���[�̍U���З�

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",									true,	0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"Idle.x",					true,	126.0f,	1.0f},	// �ҋ@
	{ ANIM_PATH"BattleIdle.x",				true,	110.0f,	1.0f},	// �퓬�ҋ@		
	{ ANIM_PATH"Walk.x",					true,	42.0f,	1.0f},	// ���s
	{ ANIM_PATH"Run.x",						true,	23.0f,	1.0f},	// ����
	{ ANIM_PATH"Death.x",					false,	129.0f,	1.0f},	// ���S
	{ ANIM_PATH"Stan.x",					false,	129.0f,	1.0f},	// �C��A�j���[�V����

	{ ATTACK_ANIM_PATH"LeftAttackS.x",		false,	34.0f,	0.5f},	// ���ガ����
	{ ATTACK_ANIM_PATH"RightAttackS.x",		false,	34.0f,	0.5f},	// �E�ガ����
	{ ATTACK_ANIM_PATH"LeftAttackM.x",		false,	53.0f,	1.0f},	// ���񂵏R��
	{ ATTACK_ANIM_PATH"RightAttackM.x",		false,	53.0f,	1.0f},	// �E�񂵏R��
	{ ATTACK_ANIM_PATH"Tackle.x",			false,	56.0f,	1.0f},	// ���������r�^�b�N��
	{ ATTACK_ANIM_PATH"HeadButt.x",			false,	68.0f,	1.0f},	// ���˂��U��

	{ DAMAGEHIT_PATH"Damage1.x",			true,	42.0f,	1.0f},	// ������1
	{ DAMAGEHIT_PATH"Damage2.x",			true,	42.0f,	1.0f},	// ������2
	{ DAMAGEHIT_PATH"Damage3.x",			true,	42.0f,	1.0f},	// ������3
	{ CHANCEHIT_PATH"chancehit1.x",			true,	23.0f,	1.0f},	// �`�����X��1
	{ CHANCEHIT_PATH"chancehit2.x",			true,	23.0f,	1.0f},	// �`�����X��2
	{ CHANCEHIT_PATH"chancehit3.x",			true,	23.0f,	1.0f},	// �`�����X��3
	{ CHANCEHIT_PATH"chancehit4.x",			true,	23.0f,	1.0f},	// �`�����X��4

};

const std::vector<CEnemyBase::AttackData> ATTACK_DATA =
{
	// �w��Ȃ�
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, 0.0f, 0.0f},
	// ���ガ����
	{ EAttackDir::eLeft,	EAttackPower::eAttackS,	true, 0.0f, 12.0f},
	// �E�ガ����
	{ EAttackDir::eRight,	EAttackPower::eAttackS,	true, 0.0f, 12.0f},
	// ���񂵏R��
	{ EAttackDir::eLeft,	EAttackPower::eAttackM,	true, 0.0f, 10.0f},
	// �E�񂵏R��
	{ EAttackDir::eRight,	EAttackPower::eAttackM,	true, 0.0f, 10.0f},
	// ���������r�^�b�N��
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, 0.0f, 0.0f},
	// ���˂��U��
	{ EAttackDir::eUp,		EAttackPower::eAttackL,	false, 0.0f, 0.0f},
	// �O�A�U��
	{ EAttackDir::eNone,	EAttackPower::eAttackS,	false, 0.0f, 0.0f},
};

// �R���X�g���N�^
CEnemyA::CEnemyA(std::vector<CVector> patrolPoints)
	: mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mPlayerAttackAngle(ATTACK_ANGLE)
	, mPlayerAttackLength(ATTACK_LENGTH)
	, mpDebugFov(nullptr)
	, mAttackStartPos(CVector::zero)
	, mAttackEndPos(CVector::zero)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
	, mpBattleTarget(nullptr)
	, mIsBattle(false)
	, mAttackCount(0)
	, mIsTripleAttack(false)
{
	//���̓G�L�����̍U���f�[�^��ݒ�
	mpAttackData = &ATTACK_DATA;
	mMaxHp = 100;
	mHp = mMaxHp;

	// �G��������
	InitEnemy("EnemyA", &ANIM_DATA);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);


	// �{�̂̃R���C�_�[
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, ENEMY_HEIGHT, 0.0f),
		CVector(0.0f,   ENEMY_SOLE, 0.0f),
		ENEMY_WIDTH
	);
	// �t�B�[���h�ƁA�v���C���[�̍U���R���C�_�[�ƃq�b�g����悤�ɐݒ�
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eAttackCol });

	// ����̃J�v�Z���R���C�_
	mpLAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		30.0
	);
	// �Փ˂���^�O�ƃ��C���[��ݒ�
	mpLAttackCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpLAttackCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpLAttackCol->SetEnable(false);

	// �E��̃J�v�Z���R���C�_
	mpRAttackCol = new CColliderSphere
	(
		this,ELayer::eAttackCol,
		30.0
	);
	// �Փ˂���^�O�ƃ��C���[��ݒ�
	mpRAttackCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpRAttackCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpRAttackCol->SetEnable(false);

	// �v���C���[�����݂��Ȃ��ꍇ�́A�͈͊O�Ƃ���
	CPlayer* player = CPlayer::Instance();

	// ����͈͂̃f�o�b�O�\�����쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
	// �U���͈͂̃f�o�b�O�\�����쐬
	mpDebugAttack = new CDebugFieldOfView(player, mPlayerAttackAngle, mPlayerAttackLength);

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// �v���C���[�����������ʒu�̃m�[�h���쐬
	mpLostPlayerNode = new CNavNode(CVector::zero, true);

	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}

	// ����̃{�[�����擾
	CModelXFrame* LHand = mpModel->FinedFrame("Armature_mixamorig_LeftHand");
	const CMatrix& LhandMTX = LHand->CombinedMatrix();
	// �E��̃{�[�����擾
	CModelXFrame* RHand = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	const CMatrix& RhandMTX = RHand->CombinedMatrix();

	// �U���p�̃R���C�_�[���s��ɐݒ�
	mpLAttackCol->SetAttachMtx(&LhandMTX);
	mpRAttackCol->SetAttachMtx(&RhandMTX);

	// �ҋ@�ő厞�Ԃ������_���Ō��߂�i�P�񂾂�����j
	mIdleTime = Math::Rand(0.0f, 5.0f);
}

CEnemyA::~CEnemyA()
{
	// �R���C�_�[��j��
	SAFE_DELETE(mpLAttackCol);
	SAFE_DELETE(mpRAttackCol);

	// ����͈͂̃f�o�b�O�\�������݂�����A�ꏏ�ɍ폜����
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}

	// �o�H�T���p�̃m�[�h��j��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpLostPlayerNode);

		//	����|�C���g�ɔz�u�����m�[�h���S�č폜
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			delete del;
			itr++;
		}
		mPatrolPoints.clear();
	}
}

// �I�u�W�F�N�g�폜����
void CEnemyA::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�I�u�W�F�N�g������͈͂̃f�o�b�O�\���Ȃ�
	// �|�C���^����ɂ���
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

//�X�V����
void CEnemyA::Update()
{

	if (CInput::Key('U')) {
		ChangeState(2);
		//ChangeAnimation(EAnimType::eWalk);
	}
	else if (CInput::Key('L')) {
		ChangeState((int)EState::ePatrol);
		//ChangeAnimation(EAnimType::eRun);
	}

	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ���
	switch (mState)
	{
	case (int)EState::eIdle:		UpdateIdle();		break;
	case (int)EState::ePatrol:		UpdatePatrol();		break;
	case (int)EState::eBattleIdle:	UpdateBattleIdle();	break;
	case (int)EState::eChase:		UpdateChase();		break;
	case (int)EState::eLost:		UpdateLost();		break;
	case (int)EState::eAttack:		UpdateAttack();		break;
	case (int)EState::eGuard:		UpdateGuard();		break;
	case (int)EState::eAvoid:		UpdateAvoid();		break;
	case (int)EState::eStan:		UpdateStan();		break;
	case (int)EState::eDeath:		UpdateDeath();		break;
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update(); 

	// �R���C�_���X�V
	mpLAttackCol->Update();
	mpRAttackCol->Update();

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("���G�̏��\n");
	CDebugPrint::Print("�@HP�F%d\n", mHp);
	CDebugPrint::Print("�@���ݓx�F%.2f\n", mStanPoints);
	CDebugPrint::Print("�@��ԁF%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("�@�U���^�C�v�F%s\n", GetAttackTypeStr(mAttackType).c_str());
	CDebugPrint::Print("�@�U���̋����F%s\n", GetAttackPowerStr().c_str());
	CDebugPrint::Print("�@�U���̕����F%s\n", GetAttackDirStr().c_str());
	CDebugPrint::Print("�@�t���[���F%.2f\n", GetAnimationFrame());
	CDebugPrint::Print("�@�ő�ҋ@���ԁF%.2f\n", mIdleTime);

}

void CEnemyA::Render()
{
	CXCharacter::Render();

	// ���񒆂ł����
	if (mState == (int)EState::ePatrol)
	{
		float rad = 1.0f;
		// ����|�C���g��S�ĕ`��
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CMatrix m;
			m.Translate(mPatrolPoints[i]->GetPos() + CVector(0.0f, rad * 2.0f, 0.0f));
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireSphere(m, rad, c);
		}
	}

	// ���������Ƃ�
	if (mState == (int)EState::eLost)
	{
		//�v���C���[�����������ʒu�Ƀf�o�b�O�\��
		float rad = 2.0f;
		CMatrix m;
		m.Translate(mpLostPlayerNode->GetPos() + CVector(0.0f, rad, 0.0f));
		Primitive::DrawWireSphere(m, rad, CColor::blue);
	}

	// ���C�R���C�_�[�̃f�o�b�O�\��
	CPlayer* player = CPlayer::Instance();
	CField* field = CField::Instance();

	if (player != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
		CVector playerPos = player->Position() + offsetPos;
		CVector selfPos = Position() + offsetPos;

		// �v���C���[�Ƃ̊ԂɎՕ��������݂���ꍇ
		CHitInfo hit;
		if (field->CollisionRay(selfPos, playerPos, &hit))
		{
			// �Փ˂����ʒu�܂ł̐���`��
			Primitive::DrawLine
			(
				selfPos, hit.cross,
				CColor::red,
				2.0f
			);
		}
		// �Օ��������݂��Ȃ������ꍇ
		else
		{
			// �v���C���[�̈ʒu�܂ł̗ΐ���`��
			Primitive::DrawLine
			(
				selfPos, playerPos,
				CColor::green,
				2.0f
			);
		}
	}
}

bool CEnemyA::IsState(int state)
{
	if ((int)mState == state) return true;
	return false;
}

bool CEnemyA::IsAttacking() const
{
	if (mState == (int)EState::eAttack) return true;
	return false;
}

void CEnemyA::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CEnemyBase::AttackStart();

	// �p���`�U�����ł���΁A�p���`�U���̃R���C�_�[���I���ɂ���
	if (mAttackType == (int)EAttackType::eBlowR)
	{
		mpRAttackCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eBlowL)
	{
		mpLAttackCol->SetEnable(true);
	}
}

void CEnemyA::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CEnemyBase::AttackEnd();

	// �U���R���C�_�[���I�t
	mpLAttackCol->SetEnable(false);
	mpRAttackCol->SetEnable(false);
}

void CEnemyA::TakeDamage(int damage, float stan, CObjectBase* causer)
{
	// �x�[�X�N���X�̃_���[�W�������Ăяo��
	CEnemyBase::TakeDamage(damage, stan, causer);
	// ���S���Ȃ����
	if (!IsDeath())
	{
		// �U���������������퓬����ɐݒ�
		mpBattleTarget = causer;
		// �퓬��Ԃ�؂�ւ���
		mIsBattle = true;

		if (!IsAttacking())
		{
			// �ړ����~
			mMoveSpeed = CVector::zero;
			// �U��������������̕����֌���
			LookAtBattleTarget(true);
		}
	}

}

void CEnemyA::Death()
{
	ChangeState((int)EState::eDeath);
}

void CEnemyA::Stan()
{
	ChangeState((int)EState::eStan);
}

// �Փˏ���
void CEnemyA::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayer* player = CPlayer::Instance();

	// �x�[�X�̏Փˏ������Ăяo��
	CEnemyBase::Collision(self, other, hit);

	if (self == mpLAttackCol || self == mpRAttackCol)
	{
		if (other->Tag() == ETag::ePlayer && other->Layer() == ELayer::ePlayer)
		{
			// �q�b�g�����̂��L�����N�^�[���A
			// �܂��U�����q�b�g���Ă��Ȃ��L�����N�^�[�ł����
			CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
			if (chara != nullptr && !IsAttackHitObj(chara))
			{
				// �U���q�b�g�ς݃��X�g�ɓo�^
				AddAttackHitObj(chara);

				// �^����_���[�W�̌v�Z
				int damage = 0;
				float stan = 0.0f;
				// �_���[�W��^����
				chara->TakeDamage(3, 0.0f, this);

			}
		}
	}
}

void CEnemyA::CalcDamage(CCharaBase* taker, int* outDamage, float* outStan) const
{
	if (mAttackType == (int)EAttackType::eBlowL)
	{

	}
}


//��Ԑ؂�ւ�
void CEnemyA::ChangeState(int state)
{
	// �U�����ɑ��̏�Ԃֈڍs����ꍇ��
	// �U���I���������Ăяo��
	if (mState != state && IsAttacking())
	{
		AttackEnd();
	}

	// ��Ԑ؂�ւ�
	CEnemyBase::ChangeState(state);
}

// �U���^�C�v�؂�ւ�
void CEnemyA::ChangeAttackType(int attacktype)
{
	// �U�����ɑ��̍U���^�C�v�ֈڍs����ꍇ��
	// �U���I���������Ăяo��
	if (mAttackType != attacktype && IsState((int)EState::eAttack))
	{
		AttackEnd();
	}
	// �U���^�C�v�؂�ւ�
	CEnemyBase::ChangeAttackType(attacktype);
}



// TODO:�G�l�~�[�x�[�X�N���X�ɈڐA����
// �v���C���[������͈͓��ɓ��������ǂ���
bool CEnemyA::IsFoundPlayer() const
{
	// �v���C���[�����݂��Ȃ��ꍇ�́A�͈͊O�Ƃ���
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	// �v���C���[���W�̎擾
	CVector playerPos = player->Position();
	// �������g�̍��W���擾
	CVector pos = Position();
	// ���g����v���C���[�܂ł̃x�N�g�������߂�
	CVector vec = playerPos - pos;
	vec.Y(0.0f);


	// 1: ����p�x�������߂�
	// �x�N�g���𐳋K�����ĕ����v�f�݂̂ɂ��邽��
	// �������P�ɂ���
	CVector dir = vec.Normalized();
	// ���g�̐��ʕ����̃x�N�g�����擾
	CVector forward = VectorZ();
	// �v���C���[�܂ł̃x�N�g����
	// ���g�̐��ʕ����̃x�N�g���̓��ς����߂Ċp�x���o��
	float dot = CVector::Dot(dir, forward);
	// ����p�x�̃��W�A�������߂�
	float angleR = Math::DegreeToRadian(mFovAngle);
	// ���߂����ςƎ���p�x�ŁA����͈͂����f����
	if (dot < cosf(angleR)) return false;


	// 2: ���싗���������߂�
	// �v���C���[�܂ł̋����Ǝ��싗���ŁA����͈͓������f����
	float dist = vec.Length();
	if (dist > mFovLength) return false;


	// �v���C���[�Ƃ̊ԂɎՕ������Ȃ������肷��
	if (!IsLookPlayer()) return false;


	// �S�Ă̏������N���A�����̂ŁA����͈͓��ł���
	return true;
}

// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
bool CEnemyA::IsLookPlayer() const
{
	// �v���C���[�����݂��Ȃ��ꍇ�́A�����Ă��Ȃ�
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;
	// �t�B�[���h�����݂��Ȃ��ꍇ�́A�Օ������Ȃ��̂Ō�����
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
	// �v���C���[�̍��W���擾
	CVector playerPos = player->Position() + offsetPos;
	// �������g�̍��W���擾
	CVector selfPos = Position() + offsetPos;

	CHitInfo hit;
	// �t�B�[���h�ƃ��C������s���A�Օ��������݂����ꍇ�́A�v���C���[�������Ȃ�
	if (field->CollisionRay(selfPos, playerPos, &hit)) return false;

	// �v���C���[�Ƃ̊ԂɎՕ������Ȃ��̂ŁA�v���C���[�������Ă���
	return true;
}

// �v���C���[���U���o���邩�ǂ���
bool CEnemyA::CanAttackPlayer(float range) const
{
	// �v���C���[�����Ȃ��ꍇ�́A�U���ł��Ȃ�
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();
	// �U���͈͂��O�ɂ�����A�U�����Ȃ�
	if (dist > range) return false;

	// �S�Ă̏������݂�����
	return true;
}

 //�U�����Ɉړ����鋗����
bool CEnemyA::AttackRangeMin()
{
	// �v���C���[�����Ȃ��ꍇ�́A�U���ł��Ȃ�
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();

	if (dist > HEADBUTT_ATTACK_RANGE) return false;

	// �S�Ă̏������݂�����
	return true;
}

// TODO:�����Ԉ���Ă��邩��
// �v���C���[�̍U�������m�������H
bool CEnemyA::IsPlayerAttackDetected() const
{
	// �v���C���[�����݂��Ȃ��ꍇ�́A�͈͊O�Ƃ���
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	if (!player->IsAttacking()) return false;

	// �v���C���[���W�̎擾
	CVector playerPos = player->Position();
	// �������g�̍��W���擾
	CVector pos = Position();
	// �v���C���[���玩�g�܂ł̃x�N�g�������߂�
	CVector vec = pos - playerPos;
	vec.Y(0.0f);


	// 1: ����p�x�������߂�
	// �x�N�g���𐳋K�����ĕ����v�f�݂̂ɂ��邽��
	// �������P�ɂ���
	CVector dir = vec.Normalized();
	// �v���C���[�̐��ʕ����̃x�N�g�����擾
	CVector forward = player->VectorZ();
	// ���g�܂ł̃x�N�g����
	// �v���C���[�̐��ʕ����̃x�N�g���̓��ς����߂Ċp�x���o��
	float dot = CVector::Dot(dir, forward);
	// ����p�x�̃��W�A�������߂�
	float angleR = Math::DegreeToRadian(mPlayerAttackAngle);
	// ���߂����ςƎ���p�x�ŁA����͈͂����f����
	if (dot < cosf(angleR)) return false;


	// 2: �U�������������߂�
	// �v���C���[�܂ł̋����Ǝ��싗���ŁA����͈͓������f����
	float dist = vec.Length();
	if (dist > mPlayerAttackLength) return false;

	// �v���C���[�Ƃ̊ԂɎՕ������Ȃ������肷��
	if (!IsLookPlayer()) return false;

	// �S�Ă̏������N���A�����̂ŁA����͈͓��ł���
	return true;

}

bool CEnemyA::DetectedPlayerAttack()
{
	CPlayer* player = CPlayer::Instance();

	// �v���C���[�̍U�������m�ς݂łȂ����
	if (!mIsDetectedPlayerAttack)
	{
		// �v���C���[�̍U�������m�������ǂ���
		if (IsPlayerAttackDetected())
		{
			mIsDetectedPlayerAttack = true;

			int rand = Math::Rand(0, 99);

			if (rand < GUARD_PROB)
			{
				ChangeState((int)EState::eStan);
					return true;
			}
		}
	}
	// �v���C���[�̍U�������m�ς�
	else
	{
		// �v���C���[�̍U�����I�������A���m�t���O��������
		if (!player->IsAttacking())
		{
			mIsDetectedPlayerAttack = false;
		}
	}
	return false;
}

// �ǂ̍U�������邩���肷��
void CEnemyA::AttackPickDetect()
{
	EState nextstate = EState::eBattleIdle;
	EAttackType nextattack = EAttackType::eNone;

	// ���ߋ����̍U������
	if (CanAttackPlayer(HEADBUTT_ATTACK_RANGE))
	{
		nextattack = EAttackType::eHeadButt;
		nextstate = EState::eAttack;
	}
	// �ߋ����̍U������
	else if (CanAttackPlayer(NEAR_ATTACK_RANGE))
	{
		int attackRand = Math::Rand(0, 99);

		if (attackRand < BLOWL_PROB)
		{
			nextattack = EAttackType::eBlowL;
			nextstate = EState::eAttack;
		}
		else if (attackRand < BLOWR_PROB)
		{
			nextattack = EAttackType::eBlowR;
			nextstate = EState::eAttack;
		}
		else if (attackRand < KICKL_PROB)
		{
			nextattack = EAttackType::eRoundKickL;
			nextstate = EState::eAttack;
		}
		else if (attackRand < KICKR_PROB)
		{
			nextattack = EAttackType::eRoundKickR;
			nextstate = EState::eAttack;
		}
		else if (attackRand < TRIPLE_PROB)
		{
			mIsTripleAttack = true;
			nextattack = EAttackType::eTripleAttack;
			nextstate = EState::eAttack;
		}
		else
		{
			nextstate = EState::eBattleIdle;
		}
	}
	// �������̍U������
	else if (CanAttackPlayer(DISTANT_ATTACK_RANGE))
	{
		int attackRand = Math::Rand(0, 99);

		if (attackRand < TACKLE_PROB)
		{
			nextattack = EAttackType::eTackle;
			nextstate = EState::eAttack;
		}
		else
		{
			nextstate = EState::eChase;
		}
	}
	else
	{
		nextstate = EState::eChase;
	}

		ChangeAttackType((int)nextattack);
		ChangeState((int)nextstate);
}


// �w�肵���ʒu�܂ňړ�����
bool CEnemyA::MoveTo(const CVector& targetPos, float speed)
{
	// �ړI�n�܂ł̃x�N�g�������߂�
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);

	// �ړ������x�N�g�������߂�
	CVector moveDir = vec.Normalized();

	// ���X�Ɉړ������֌�����
	CVector forward = CVector::Slerp
	(
		VectorZ(), // ���݂̐���
		moveDir,   // �ړ�����
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// ����̈ړ����������߂�
	float moveDist = speed * Times::DeltaTime();
	// �ړI�n�܂ł̎c��̋���
	float remainDist = vec.Length();
	// �c��̋������ړ��������Z���ꍇ
	if (remainDist <= moveDist)
	{
		// �ړI�n�܂ňړ�����
		pos = CVector(targetPos.X(), pos.Y(), targetPos.Z());
		Position(pos);
		return true;    // �ړI�n�ɓ��������̂ŁAtrue��Ԃ�
	}

	// �c��̋������ړ�������蒷���ꍇ�́A
	// �ړ��������ړI�n�ֈړ�
	pos += moveDir * moveDist;
	Position(pos);

	// �ړI�n�ɂ͓������Ȃ�����
	return false;
}


bool CEnemyA::RandMove(float speed)
{

}

// �퓬����̕��֌���
void CEnemyA::LookAtBattleTarget(bool immediate)
{
	// �퓬���肪���Ȃ���΁A�������Ȃ�
	if (mpBattleTarget == nullptr) return;

	// �퓬����܂ł̕����x�N�g�������߂�
	CVector targetPos = mpBattleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);
	vec.Normalize();
	// �����ɐ퓬����̕����֌���
	if (immediate)
	{
		Rotation(CQuaternion::LookRotation(vec));
	}
	// ���X�ɐ퓬����̕����֌���
	else
	{
		CVector forward = CVector::Slerp
		(
			VectorZ(), vec,
			LOOKAT_SPEED * Times::DeltaTime()
		);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// ���ɏ��񂷂�|�C���g��ύX
void CEnemyA::ChangePatrolPoint()
{
	// ����|�C���g���ݒ肳��Ă��Ȃ��ꍇ�́A�������Ȃ�
	int size = mPatrolPoints.size();
	if (size == 0) return;

	// ����J�n���ł���΁A��ԋ߂�����|�C���g��I��
	if (mNextPatrolIndex == -1)
	{
		int nearIndex = -1;     // ��ԋ߂�����|�C���g�̔ԍ�
		float nearDist = 0.0f;  // ��ԋ߂�����|�C���g�܂ł̋���
		// �S�Ă̏���|�C���g�𒲂ׁA��ԋ߂�����|�C���g��T��
		for (int i = 0; i < size; i++)
		{
			CVector point = mPatrolPoints[i]->GetPos();
			CVector vec = point - Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// ����|�C���g���߂�����ꍇ�́A�X���[
			if (dist < PATROL_NEAR_DIST) continue;
			// ��ԋ߂�����|�C���g�������́A
			// ���݈�ԋ߂�����|�C���g��肳��ɋ߂��ꍇ�́A
			// ����|�C���g�̔ԍ���u��������
			if (nearIndex < 0 || dist < nearDist)
			{
				nearIndex = i;
				nearDist = dist;
			}
		}
		mNextPatrolIndex = nearIndex;
	}
	// ���񒆂������ꍇ�A���̏���|�C���g���w�肷��
	else 
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
	}

	// ���ɏ��񂷂�|�C���g�����܂����ꍇ
	if (mNextPatrolIndex >= 0)
	{
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			// ����|�C���g�̌o�H�T���m�[�h�̈ʒu��ݒ肵�������ƂŁA
			// �e�m�[�h�ւ̐ڑ������X�V
			for (CNavNode* node : mPatrolPoints)
			{
				node->SetPos(node->GetPos());
			}

			// ����|�C���g�܂ł̍ŒZ�o�H�����߂�
			if (navMgr->Navigate(mpNavNode, mPatrolPoints[mNextPatrolIndex], mMoveRoute))
			{
				// ���̖ړI�n�̃C���f�b�N�X��ݒ�
				mNextMoveIndex = 1;
			}
		}
	}
}

// �ҋ@��Ԃ̍X�V����
void CEnemyA::UpdateIdle()
{
	// �v���C���[������͈͓��ɓ���������́A
	// ���ɐ퓬��Ԃł���΁A�ǐՏ�Ԃֈڍs
	if (IsFoundPlayer() || mIsBattle)
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// �ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	if (mElapsedTime < mIdleTime)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// �ҋ@���Ԃ��o�߂�����A�����Ԃֈڍs
		ChangeState((int)EState::ePatrol);
	}
}

// ���񒆂̍X�V����
void CEnemyA::UpdatePatrol()
{
	if (IsFoundPlayer())
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�F����|�C���g�����߂�
	case 0:
		mNextPatrolIndex = -1;
		ChangePatrolPoint();
		mStateStep++;
		break;
		// �X�e�b�v1�F����|�C���g�܂ňړ�
	case 1:
	{
		ChangeAnimation((int)EAnimType::eWalk);
		// �ŒZ�o�H�̎��̃m�[�h�܂ňړ�
		CNavNode* moveNode = mMoveRoute[mNextMoveIndex];
		if (MoveTo(moveNode->GetPos(), WALK_SPEED))
		{
			// �ړ����I���΁A���̃m�[�h�֐؂�ւ�
			mNextMoveIndex++;
			// �Ō�̃m�[�h�i�ړI�n�̃m�[�h�j�������ꍇ�́A���̃X�e�b�v�֐i�߂�
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				mStateStep++;
			}
		}
		break;
	}
		// �X�e�b�v2�F�ړ���̑ҋ@
	case 2:
		ChangeAnimation((int)EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// �ҋ@�ő厞�Ԃ������_���Ō��߂�
			mIdleTime = Math::Rand(IDLE_TIME_MIN, IDLE_TIME_MAX);
			ChangePatrolPoint();
			mStateStep = 1;
			mElapsedTime = 0.0f;
		}
		break;
	}
}

// �퓬�ҋ@��Ԃ̍X�V����
void CEnemyA::UpdateBattleIdle()
{
	if (DetectedPlayerAttack()) return;

	ChangeAnimation((int)EAnimType::eBattleIdle);

	LookAtBattleTarget();
	switch (mStateStep)
	{
	case 0:	// �X�e�b�v0�F�ҋ@���Ԃ������_���Ō���
		// �ҋ@���Ԃ����܂��ĂȂ����
		if (mBattleIdletime <= 0.0f)
		{
			// �����_���őҋ@���Ԃ�����
			mBattleIdletime = Math::Rand
			(
				BATTLE_IDLE_TIME_MIN,
				BATTLE_IDLE_TIME_MAX
			);
		}
		mStateStep++;
		break;
	case 1:
		// �퓬���̑ҋ@���ԑ҂�
		if (mElapsedTime < mBattleIdletime)
		{
			// TODO:�v���C���[�̍U���͈͊O�Ɉړ�����
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// �U���̎�ނƃX�e�[�g�����߂�
			AttackPickDetect();
			// �퓬�ҋ@���Ԃ�������
			mBattleIdletime = 0.0f;
		}
	}
}

// �ǐՒ��̍X�V����
void CEnemyA::UpdateChase()
{

	if (DetectedPlayerAttack()) return;

	// �v���C���[�̍��W�֌����Ĉړ�����
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();

	// �퓬��Ԃɓ����Ă��Ȃ����
	if (!mIsBattle)
	{
		// �퓬��ԂɂȂ�
		mIsBattle = true;
		mpBattleTarget = player;
	}

	// �v���C���[�������Ȃ��Ȃ�����A����������Ԃɖ߂�
	if (!IsLookPlayer())
	{
		mpLostPlayerNode->SetPos(targetPos);
		ChangeState((int)EState::eLost);
		return;
	}

	// ����A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eRun);

	// �o�H�T���Ǘ��N���X�����݂���΁A
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// ���g�̃m�[�h����v���C���[�̃m�[�h�܂ł̍ŒZ���������߂�
		CNavNode* playerNode = player->GetNavNode();
		if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
		{
			// ���g�̃m�[�h����v���C���[�̃m�[�h�܂Ōq�����Ă�����A
			// �ړ�����ʒu�����̃m�[�h�̈ʒu�ɐݒ�
			targetPos = mMoveRoute[1]->GetPos();
		}
	}
	// �ړ�����
	if (MoveTo(targetPos, RUN_SPEED))
	{
	}

	if (CanAttackPlayer(DISTANT_ATTACK_RANGE))
	{
		// �v���C���[�ɍU���ł���Ȃ�΁A�U����Ԃֈڍs
		AttackPickDetect();
	}
}

// �����������̍X�V����
void CEnemyA::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		// �퓬��ԏI��
		mIsBattle = false;
		mpBattleTarget = nullptr;
		ChangeState((int)EState::eIdle);
		return;
	}
	// �v���C���[����������A�ǐՏ�Ԃֈڍs
	if (IsLookPlayer()) 
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// ����A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eRun);

	switch (mStateStep)
	{
		// �X�e�b�v0�F���������ʒu�܂ł̍ŒZ�o�H�����߂�
	case 0:
		if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
		{
			// ���������ʒu�܂Ōo�H���q�����Ă�����A���̃X�e�b�v��
			mNextMoveIndex = 1;
			mStateStep++;
		}
		else
		{
			// �o�H���q�����Ă��Ȃ���΁A�ҋ@��Ԃ֖߂�
			ChangeState((int)EState::eIdle);
		}
		break;
	case 1:
		// �v���C���[�����������ʒu�܂ňړ�
		if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				// �퓬��ԏI��
				mIsBattle = false;
				mpBattleTarget = nullptr;
				// �ړ����I���΁A�ҋ@��Ԃֈڍs
				ChangeState((int)EState::eIdle);
			}
		}
		break;
	}
}

// TODO:�U���R���C�_�[���I���ɂȂ�܂ŁA�v���C���[�̕����֌���

// �U�����̍X�V����
void CEnemyA::UpdateAttack()
{
	// �U���̎�ނ��ƂɁA�X�V������؂�ւ���
	switch (mAttackType)
	{
	case (int)EAttackType::eBlowL:			UpdateBlowL();			break;
	case (int)EAttackType::eBlowR:			UpdateBlowR();			break;
	case (int)EAttackType::eRoundKickL:		UpdateRoundKickL();		break;
	case (int)EAttackType::eRoundKickR:		UpdateRoundKickR();		break;
	case (int)EAttackType::eTackle:			UpdateTackle();			break;
	case (int)EAttackType::eHeadButt:		UpdateHeadButt();		break;
	case (int)EAttackType::eTripleAttack:	UpdataTripleAttack();	break;
	}

	if (mAttackType == (int)EAttackType::eNone)
	{
		ChangeState((int)EState::eBattleIdle);
	}
}

void CEnemyA::UpdateGuard()
{
}

void CEnemyA::UpdateAvoid()
{
}

void CEnemyA::UpdateDeath()
{
	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0�F���S�A�j���[�V�����Đ�
	case 0:
		ChangeAnimation((int)EAnimType::eDeath, true);
		mStateStep++;
		break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
	case 1:
		// ���S�A�j���[�V�������I��������A�폜
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
		// �X�e�b�v2�F���S��̑҂�
	case 2:
		if (mElapsedTime < DEATH_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// �҂����Ԃ��I��������A�폜
		else
		{
			Kill();
		}
		break;
	}
}

void CEnemyA::UpdateStan()
{
	// TODO:�A�j���[�V�������Đ��A�P�b��ɕ����A
	// ���E�ɓ����Ă�����F�U����ԂɁA�����Ă��Ȃ�������F�ҋ@��Ԃ�
	// �P�b�̊ԂōU��������ƁF�U���`�����X�I�I��Ԃ�
	switch (mStateStep)
	{
		// ���݃A�j���[�V�����Đ�
	case 0:
		ChangeAnimation((int)EAnimType::eStan, true);
		mStateStep++;
		break;
		// �A�j���[�V�����I���҂�
	case 1:
		// �A�j���[�V�������I���Αҋ@��Ԃ֖߂�
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

//void CEnemyA::UpdateChance()
//{
//	// TODO�F�m�b�N�o�b�N����悤�ɂȂ�
//	// �U���ɔ{�������
//}

// �ガ�����U��
void CEnemyA::UpdateBlowL()
{
	switch (mStateStep)
	{
	case 0: // �A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eBlowL, true);
		mStateStep++;
		break;
	case 1: // �U���p�R���C�_�[�I��
	{
		// �p���B���o����͈͂��I��������A�U���R���C�_�[���I���ɂ���
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		if (frame >= data.parryEndFrame)
		{
			AttackStart();
			mStateStep++;
		}
		// �U���R���C�_�[���I���ɂȂ�܂ŁA�v���C���[�̕����֌���
		else
		{
			LookAtBattleTarget();
		}
		break;
	}
	case 2:	// �X�e�b�v2 : �U���A�j���[�V�����̏I���҂�
		if (IsAnimationFinished())
		{
			AttackEnd();
			if (mIsTripleAttack)
			{
				mStateStep++;
			}
			else
			{
				// ���Ԃ��o�߂�����A�ҋ@��Ԃֈڍs
				ChangeState((int)EState::eBattleIdle);
			}
		}
		break;
	case 3:	// �X�e�b�v3 : �U���I�����̑҂�����
		mAttackCount++;
		ChangeAttackType((int)EAttackType::eTripleAttack);
		ChangeState((int)EState::eAttack);
		break;
	}
}

// �ガ�����U��
void CEnemyA::UpdateBlowR()
{
	switch (mStateStep)
	{
	case 0: // �A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eBlowR, true);
		mStateStep++;
		break;
	case 1: // �U���p�R���C�_�[�I��
	{
		// �p���B���o����͈͂��I��������A�U���R���C�_�[���I���ɂ���
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		if (frame >= data.parryEndFrame)
		{
			AttackStart();
			mStateStep++;
		}
		// �U���R���C�_�[���I���ɂȂ�܂ŁA�v���C���[�̕����֌���
		else
		{
			LookAtBattleTarget();
		}
		break;
	}
	case 2:	// �X�e�b�v2 : �U���A�j���[�V�����̏I���҂�
		if (IsAnimationFinished())
		{
			AttackEnd();
			if (mIsTripleAttack)
			{
				mStateStep++;
			}
			else
			{
				// ���Ԃ��o�߂�����A�ҋ@��Ԃֈڍs
				ChangeState((int)EState::eBattleIdle);
			}
		}
		break;
	case 3:	// �X�e�b�v3 : �U���I�����̑҂�����
		mAttackCount++;
		ChangeAttackType((int)EAttackType::eTripleAttack);
		ChangeState((int)EState::eAttack);
		break;
	}
}

// �񂵏R��U��
void CEnemyA::UpdateRoundKickL()
{
	switch (mStateStep)
	{
		case 0:	// �X�e�b�v�O�F�U���A�j���[�V�����A�U���J�n�ʒu�ƍU���I���ʒu�̐ݒ�
			mAttackStartPos = Position();
			mAttackEndPos = mAttackStartPos + VectorZ() * KICK_MOVE_DIST;
			ChangeAnimation((int)EAnimType::eRoundKickL, true);
			mStateStep++;
			break;
		case 1:	// �X�e�b�v�P�F�U�����̈ړ�����
		{
			// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
			float frame = GetAnimationFrame();
			AttackStart();
			if (!AttackRangeMin()) {
				if (frame >= KICK_MOVE_START)
				{
					// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
					if (frame < KICK_MOVE_END)
					{
						// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
						float moveFrame = KICK_MOVE_END - KICK_MOVE_START;
						float percent = (frame - KICK_MOVE_START) / moveFrame;
						CVector pos = CVector::Lerp(mAttackStartPos, mAttackEndPos, percent);
						Position(pos);
					}
					// �ړ��I���t���[���܂œ��B�����ꍇ
					else
					{
						Position(mAttackEndPos);
						mStateStep++;
					}
				}
			}
			else
			{
				mStateStep++;
			}
			break;
		}
		case 2:	// �X�e�b�v�Q�F�U���A�j���[�V�����̏I���҂�
			if (IsAnimationFinished())
			{
				AttackEnd();
				ChangeState((int)EState::eBattleIdle);
			}
			break;
	}
}

// �񂵏R��U��
void CEnemyA::UpdateRoundKickR()
{
	switch (mStateStep)
	{
	case 0:	// �X�e�b�v�O�F�U���A�j���[�V�����A�U���J�n�ʒu�ƍU���I���ʒu�̐ݒ�
		mAttackStartPos = Position();
		mAttackEndPos = mAttackStartPos + VectorZ() * KICK_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eRoundKickR, true);
		mStateStep++;
		break;
	case 1:	// �X�e�b�v�P�F�U�����̈ړ�����
	{
		// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
		float frame = GetAnimationFrame();
		AttackStart();
		if (!AttackRangeMin()) {
			if (frame >= KICK_MOVE_START)
			{
				// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
				if (frame < KICK_MOVE_END)
				{
					// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
					float moveFrame = KICK_MOVE_END - KICK_MOVE_START;
					float percent = (frame - KICK_MOVE_START) / moveFrame;
					CVector pos = CVector::Lerp(mAttackStartPos, mAttackEndPos, percent);
					Position(pos);
				}
				// �ړ��I���t���[���܂œ��B�����ꍇ
				else
				{
					Position(mAttackEndPos);
					mStateStep++;
				}
			}
		}
		else
		{
			mStateStep++;
		}
		break;
	}
	case 2:	// �X�e�b�v�Q�F�U���A�j���[�V�����̏I���҂�
		if (IsAnimationFinished())
		{
			AttackEnd();
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// ���������r�^�b�N��
void CEnemyA::UpdateTackle()
{
	switch (mStateStep)
	{
	case 0:	// �X�e�b�v0 : �U���A�j���[�V����
		// �U���J�n�ʒu�ƍU���I���ʒu�̐ݒ�
		mAttackStartPos = Position();
		mAttackEndPos = mAttackStartPos + VectorZ() * TACKLE_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eTackle, false);
		mStateStep++;
		break;
	case 1:	// �X�e�b�v�P�F�U�����̈ړ�����
	{
		// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
		float frame = GetAnimationFrame();
		AttackStart();
		if (!AttackRangeMin()) {
			if (frame >= TACKLE_MOVE_START)
			{
				// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
				if (frame < TACKLE_MOVE_END)
				{
					// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
					float moveFrame = TACKLE_MOVE_END - TACKLE_MOVE_START;
					float percent = (frame - TACKLE_MOVE_START) / moveFrame;
					CVector pos = CVector::Lerp(mAttackStartPos, mAttackEndPos, percent);
					Position(pos);
				}
				// �ړ��I���t���[���܂œ��B�����ꍇ
				else
				{
					Position(mAttackEndPos);
					mStateStep++;
				}
			}
		}
		else
		{
			mStateStep++;
		}
		break;
	}
	case 2:	// �X�e�b�v2 : �U���A�j���[�V�����̏I���҂�
		if (IsAnimationFinished())
		{
			AttackEnd();
			// ���Ԃ��o�߂�����A�ҋ@��Ԃֈڍs
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// �����o���U��
void CEnemyA::UpdateHeadButt()
{
	switch (mStateStep)
	{
	case 0: // �A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eHeadButt, true);
		mStateStep++;
		break;
	case 1: // �U���p�R���C�_�[�I��
	{
		// �p���B���o����͈͂��I��������A�U���R���C�_�[���I���ɂ���
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		if (frame >= data.parryEndFrame)
		{
			AttackStart();
			mStateStep++;
		}
		// �U���R���C�_�[���I���ɂȂ�܂ŁA�v���C���[�̕����֌���
		else
		{
			LookAtBattleTarget();
		}
		break;
	}
	case 2:	// �X�e�b�v2 : �U���A�j���[�V�����̏I���҂�
		if (IsAnimationFinished())
		{
			AttackEnd();
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// �O�A�U��
void CEnemyA::UpdataTripleAttack()
{
	switch (mAttackCount)
	{
	case 0:
	{
		int attackrand = Math::Rand(0, 99);
		// �O�����Z�q
		attackrand < FIFTY_FIFTY_PROB ?
			ChangeAttackType((int)EAttackType::eBlowL) : ChangeAttackType((int)EAttackType::eBlowR);
		ChangeState((int)EState::eAttack);
		break;
	}
	case 1:
	{
		if (mElapsedTime < ATTACK_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			int attackrand = Math::Rand(0, 99);
			attackrand < FIFTY_FIFTY_PROB ?
				ChangeAttackType((int)EAttackType::eBlowL) : ChangeAttackType((int)EAttackType::eBlowR);
			ChangeState((int)EState::eAttack);
			break;
		}
	}
	case 2:
	{
		if (mElapsedTime < ATTACK_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			int attackrand = Math::Rand(0, 99);
			attackrand < FIFTY_FIFTY_PROB ?
				ChangeAttackType((int)EAttackType::eBlowL) : ChangeAttackType((int)EAttackType::eBlowR);
			ChangeState((int)EState::eAttack);
			break;
		}
	}
	case 3:
		mAttackCount = 0;
		mIsTripleAttack = false;
		ChangeState((int)EState::eBattleIdle);
		break;
	}
}


// ��Ԃ̕�������擾
std::string CEnemyA::GetStateStr(int state) const
{
	switch ((int)state)
	{
	case (int)EState::eIdle:    return "�ҋ@";
	case (int)EState::ePatrol:  return "����";
	case (int)EState::eChase:   return "�ǐ�";
	case (int)EState::eLost:    return "������";
	case (int)EState::eAttack:  return "�U��";
	case (int)EState::eDeath:	return "���S";
	case (int)EState::eStan:	return "�C��";
	}
	return "";
}

std::string CEnemyA::GetAttackTypeStr(int state) const
{
	switch ((int)state)
	{
	case (int)EAttackType::eNone:			return "�w��Ȃ�";
	case (int)EAttackType::eBlowL:			return "���ガ����";
	case (int)EAttackType::eBlowR:			return "�E�ガ����";
	case (int)EAttackType::eRoundKickL:		return "���񂵏R��";
	case (int)EAttackType::eRoundKickR:		return "�E�񂵏R��";
	case (int)EAttackType::eTackle:			return "���������r�^�b�N��";
	case (int)EAttackType::eHeadButt:		return "���˂��U��";
	case (int)EAttackType::eTripleAttack:	return "�O�A�U��";
	}
	return std::string();
}

CColor CEnemyA::GetStateColor(int state) const
{
	switch ((int)state)
	{
	case (int)EState::eIdle:    return CColor::white;
	case (int)EState::ePatrol:  return CColor::green;
	case (int)EState::eChase:   return CColor::red;
	case (int)EState::eLost:    return CColor::yellow;
	case (int)EState::eAttack:  return CColor::magenta;
	case (int)EState::eDeath:	return CColor::black;
	case (int)EState::eStan:	return CColor::cyan;
	}
	return CColor::white;
}


