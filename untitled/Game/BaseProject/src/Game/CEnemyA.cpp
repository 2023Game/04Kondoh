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

#define ENEMY_HP 100
#define ENEMY_HEIGHT 16.0f      // �G�̍���
#define ENEMY_SOLE   5.0f       // �G�̒�
#define ENEMY_WIDTH  5.0f       // �G�̔��a
#define FOV_ANGLE 45.0f         // ����͈͂̊p�x
#define FOV_LENGTH 200.0f       // ����͈͂̋���
#define EYE_HEIGHT 10.0f        // ���_�̍���
#define WALK_SPEED 10.0f        // �����̑��x
#define BATTLE_WALK_SPEED 0.01f	// �퓬���̈ړ�
#define RUN_SPEED 40.0f         // �����Ă��鎞�̑��x
#define ROTATE_SPEED 6.0f       // ��]���x
#define LOOKAT_SPEED 4.0f		// �Ώۂ̕����Ɍ������x

#define IDLE_TIME 5.0f          // �ҋ@��Ԃ̎���
#define DEATH_WAIT_TIME 3.0f	// ���S���̑ҋ@����
#define STUN_WAIT_TIME 3.0f		// �C�⎞�̑ҋ@����

#define DISTANT_ATTACK_RANGE	150.0f	// �^�b�N���U���͈�
#define NEAR_ATTACK_RANGE		 30.0f	// �ガ�����Ɖ񂵏R��A�O�A�U���͈̔�
#define HEADBUTT_ATTACK_RANGE	  10.0f	// ���˂��U���͈�

#define BLOW_DMG		10.0f	// �ガ�����̃_���[�W
#define KICK_DMG		20.0f	// �񂵏R��̃_���[�W
#define TACKLE_DMG		50.0f	// �^�b�N���̃_���[�W
#define HEADBUTT_DMG	 5.0f	// ���˂��̃_���[�W

#define STUN_THRESHOLD	100.0f	// ���݂̂������l
#define BLOW_STUN		10.0f	// �ガ�����̃X�^���l
#define KICK_STUN		30.0f	// �񂵏R��̃X�^���l
#define TACKLE_STUN		20.0f	// �^�b�N���̃X�^���l
#define HEADBUTT_STUN	50.0f	// ���˂��̃X�^���l

#define BLOW_KNOCKBACK		50.0f	// �ガ�����̃m�b�N�o�b�N����
#define KICK_KNOCKBACK		50.0f	// 
#define TACKLE_KNOCKBACK	50.0f	// 
#define HEADBUTT_KNOCKBACK	50.0f	//

#define BLOWL_PROB	20		// ���ガ�����̊m��
#define BLOWR_PROB	40		// �E�ガ�����̊m��
#define KICKL_PROB	50		// ���񂵏R��̊m��
#define KICKR_PROB	60		// �E�񂵏R��̊m��
#define TRIPLE_PROB	65		// �O�A�U���̊m��
#define TACKLE_PROB	90		// �^�b�N���̊m��
#define GUARD_PROB	10		// �h��̊m��
#define FIFTY_FIFTY_PROB 50	// �����̊m��

#define TACKLE_MOVE_DIST	150.0f	// �^�b�N�����̈ړ�����
#define TACKLE_MOVE_START	0.5f	// �^�b�N�����̈ړ��J�n�t���[��
#define TACKLE_MOVE_END		55.0f	// �^�b�N�����̈ړ��I���t���[��
#define TACKLE_WAIT_DIST	10.0f	// �^�b�N���I�����̗\������̈ړ�����
#define TACKLE_WAIT_START	0.0f	// �^�b�N���I�����̗\������̊J�n
#define TACKLE_WAIT_END		60.0f	// �^�b�N���I�����̗\������̏I��
#define TACKLE_COOL_TIME	4.0f	// �^�b�N���U���̃N�[���^�C��

#define BLOW_MOVE_DIST		15.0f	// �ガ�������̈ړ�����
#define BLOW_MOVE_START		 1.0f	// �ガ�������̈ړ��J�n�t���[��
#define BLOW_MOVE_END		30.0f	// �ガ�������̈ړ��I���t���[��
#define KICK_MOVE_DIST		15.0f	// �񂵏R�莞�̈ړ�����
#define KICK_MOVE_START		 1.0f	// �񂵏R�莞�̈ړ��J�n�t���[��
#define KICK_MOVE_END		50.0f	// �񂵏R�莞�̈ړ��I���t���[��
#define TRIPLE_WAIT_TIME	0.005f	// �O�A�U���̊Ԃ̍U���I�����̑҂�����

#define PLAYER_ATTACK_ANGLE		35.0f	// �U���͈͂̊p�x
#define PLAYER_ATTACK_LENGTH	50.0f	// �U���͈͂̋���

#define IDLE_TIME_MIN 0.0f			// �ҋ@���̍ŒZ�ҋ@����
#define IDLE_TIME_MAX 6.0f			// �ҋ@���̍Œ��ҋ@����
#define BATTLE_IDLE_TIME_MIN 0.0f	// �퓬�ҋ@���̍ŒZ�ҋ@����
#define BATTLE_IDLE_TIME_MAX 2.0f	// �퓬�ҋ@���̍Œ��ҋ@����
#define LOST_TIME 5.0f		// �ǂɈ��������������̒ǐՏ����L������

#define PATROL_INTERVAL 3.0f    // ���̏���Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST 10.0f  // ����J�n���ɑI������鏄��|�C���g�̍ŒZ����

#define EATTACKWAY CPlayer::EAttackDir		// �v���C���[�̍U������
#define EATTACKPWOER CPlayer::EAttackPower	// �v���C���[�̍U���З�

#define GAUGE_OFFSET_Y 23.0f

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",									 true,	  0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"Idle.x",					 true,	126.0f,	1.0f},	// �ҋ@
	{ ANIM_PATH"BattleIdle.x",				 true,	110.0f,	0.5f},	// �퓬�ҋ@		
	{ ANIM_PATH"Walk.x",					 true,	 42.0f,	1.0f},	// ���s
	{ ANIM_PATH"Run.x",						 true,	 23.0f,	1.0f},	// ����
	{ ANIM_PATH"Stun.x",					false,	 74.0f,	1.0f},	// ����
	{ ANIM_PATH"StunWait.x",				 true,	129.0f, 1.0f},	// �����҂�����
	{ ANIM_PATH"Death.x",					false,	129.0f,	1.0f},	// ���S

	{ ATTACK_ANIM_PATH"LeftAttackS.x",		false,	 34.0f,	0.5f},	// ���ガ����
	{ ATTACK_ANIM_PATH"RightAttackS.x",		false,	 34.0f,	0.5f},	// �E�ガ����
	{ ATTACK_ANIM_PATH"LeftAttackM.x",		false,	 53.0f,	0.5f},	// ���񂵏R��
	{ ATTACK_ANIM_PATH"RightAttackM.x",		false,	 53.0f,	0.5f},	// �E�񂵏R��
	{ ATTACK_ANIM_PATH"Tackle.x",			false,	 56.0f,	1.0f},	// �^�b�N��
	{ ATTACK_ANIM_PATH"TackleWait.x",		false,	 71.0f,	1.5f},	// �^�b�N���I���̗\������	
	{ ATTACK_ANIM_PATH"HeadButt.x",			false,	 68.0f,	1.0f},	// ���˂��U��

	{ ANIM_PATH"BattleWalkL.x",				 true,	 32.0f,	1.0f},	// �퓬���̍����s
	{ ANIM_PATH"BattleWalkR.x",				 true,	 32.0f,	1.0f},	// �퓬���̉E���s

	{ ANIM_PATH"GuardIdle.x",				 true,	151.0f,	0.8f},	// �h�䎞�̑ҋ@
	{ ANIM_PATH"GuardHit.x",				false,	 54.0f,	1.0f},	// �h�䎞�̃q�b�g

	{ DAMAGEHIT_PATH"Damage1.x",			false,	 42.0f,	1.0f},	// ������1
	{ DAMAGEHIT_PATH"Damage2.x",			false,	 42.0f,	1.0f},	// ������2
	{ DAMAGEHIT_PATH"Damage3.x",			false,	 42.0f,	1.0f},	// ������3
	{ CHANCEHIT_PATH"chancehit1.x",			false,	 23.0f,	1.0f},	// �`�����X��1
	{ CHANCEHIT_PATH"chancehit2.x",			false,	 23.0f,	1.0f},	// �`�����X��2
	{ CHANCEHIT_PATH"chancehit3.x",			false,	 23.0f,	1.0f},	// �`�����X��3
	{ CHANCEHIT_PATH"chancehit4.x",			false,	 23.0f,	1.0f},	// �`�����X��4

};

// �v���C���[�̍U���f�[�^�̃e�[�u��
const std::vector<CEnemyBase::AttackData> ATTACK_DATA =
{
	// �w��Ȃ�
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, false, 0.0f, 0.0f, 0.0f, 0.0f},
	// ���ガ����
	{ EAttackDir::eLeft,	EAttackPower::eAttackS,	true, true, 0.0f, 10.0f, 16.0f, 22.0f},
	// �E�ガ����
	{ EAttackDir::eRight,	EAttackPower::eAttackS,	true, true, 0.0f, 10.0f, 16.0f, 24.0f},
	// ���񂵏R��
	{ EAttackDir::eLeft,	EAttackPower::eAttackM,	true, true, 0.0f, 10.0f, 23.0f, 43.0f},
	// �E�񂵏R��
	{ EAttackDir::eRight,	EAttackPower::eAttackM,	true, true, 0.0f, 10.0f, 23.0f, 43.0f},
	// �^�b�N��
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, false, 0.0f, 0.0f, 0.0f, 0.0f},
	// �^�b�N���I�����̗\������
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, false, 0.0f, 0.0f, 0.0f, 0.0f},
	// ���˂��U��
	{ EAttackDir::eUp,		EAttackPower::eAttackL,	false, true, 0.0f, 0.0f, 20.0f, 30.0f},
	// �O�A�U��
	{ EAttackDir::eNone,	EAttackPower::eAttackS,	false, false, 0.0f, 0.0f, 0.0f, 0.0f},
};

// �R���X�g���N�^
CEnemyA::CEnemyA(const CVector& pos, std::vector<CVector> patrolPoints)
	: mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mPlayerAttackAngle(PLAYER_ATTACK_ANGLE)
	, mPlayerAttackLength(PLAYER_ATTACK_LENGTH)
	, mpDebugFov(nullptr)
	, mpBattleTarget(nullptr)
	, mMoveStartPos(CVector::zero)
	, mMoveEndPos(CVector::zero)
	, mIsBattle(false)
	, mIsTripleAttack(false)
	, mRandMoveAngle(0.0f)
	, mRandHitAnim(0)
	, mAttackCount(0)
	, mTackleCount(0)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
{
	Position(pos);

	//���̓G�L�����̍U���f�[�^��ݒ�
	mpAttackData = &ATTACK_DATA;
	mMaxHp = ENEMY_HP;
	mHp = mMaxHp;

	// �Q�[�W�̃I�t�Z�b�g�ʒu��ݒ�
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

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
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eWall});
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::ePlayer, ELayer::eAttackCol });

	// ����̃J�v�Z���R���C�_
	mpLHandCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		40.0
	);
	// �Փ˂���^�O�ƃ��C���[��ݒ�
	mpLHandCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpLHandCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpLHandCol->SetEnable(false);

	// �E��̃J�v�Z���R���C�_
	mpRHandCol = new CColliderSphere
	(
		this,ELayer::eAttackCol,
		40.0
	);
	// �Փ˂���^�O�ƃ��C���[��ݒ�
	mpRHandCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpRHandCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpRHandCol->SetEnable(false);

	// �����̋��R���C�_
	mpLFootCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		50.0f
	);
	// �Փ˂���^�O�ƃ��C���[��ݒ�
	mpLFootCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpLFootCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpLFootCol->SetEnable(false);

	// �E���̋��R���C�_
	mpRFootCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		50.0f
	);
	// �Փ˂���^�O�ƃ��C���[��ݒ�
	mpRFootCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpRFootCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpRFootCol->SetEnable(false);

	// ���̋��R���C�_
	mpHeadCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		25.0, true
	);
	mpHeadCol->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpHeadCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpHeadCol-> SetEnable(false);

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
	CModelXFrame* handL = mpModel->FinedFrame("Armature_mixamorig_LeftHand");
	const CMatrix& lHandMTX = handL->CombinedMatrix();
	// �E��̃{�[�����擾
	CModelXFrame* handR = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	const CMatrix& rHandMTX = handR->CombinedMatrix();
	// �����̃{�[�����擾
	CModelXFrame* footL = mpModel->FinedFrame("Armature_mixamorig_LeftFoot");
	const CMatrix& lFootMTX = footL->CombinedMatrix();
	// �E���̃{�[�����擾
	CModelXFrame* footR = mpModel->FinedFrame("Armature_mixamorig_RightFoot");
	const CMatrix& rFootMTX = footR->CombinedMatrix();
	// 
	CModelXFrame* head = mpModel->FinedFrame("Armature_mixamorig_Head");
	const CMatrix& headMTX = head->CombinedMatrix();

	// �U���p�̃R���C�_�[���s��ɐݒ�
	mpLHandCol->SetAttachMtx(&lHandMTX);
	mpRHandCol->SetAttachMtx(&rHandMTX);
	mpLFootCol->SetAttachMtx(&lFootMTX);
	mpRFootCol->SetAttachMtx(&rFootMTX);
	mpHeadCol->SetAttachMtx(&headMTX);

	// �ҋ@�ő厞�Ԃ������_���Ō��߂�i�P�񂾂�����j
	mIdleTime = Math::Rand(0.0f, 5.0f);
	// �~�^���̈ړ��p�x�������_���Ō��߂�i�P�񂾂�����j
	mRandMoveAngle = Math::Rand(-30.0f, 30.0f);

	// ����ɍX�V�������Ă�Ń��f���̈ʒu�𒲐�
	CXCharacter::Update();
}

CEnemyA::~CEnemyA()
{
	// �R���C�_�[��j��
	SAFE_DELETE(mpLHandCol);
	SAFE_DELETE(mpRHandCol);
	SAFE_DELETE(mpLFootCol);
	SAFE_DELETE(mpRFootCol);
	SAFE_DELETE(mpHeadCol);

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
		mpNavNode->Kill();
		mpLostPlayerNode->Kill();

		//	����|�C���g�ɔz�u�����m�[�h���S�č폜
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			del->Kill();
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
	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ���
	switch (mState)
	{
	case (int)EState::eIdle:		UpdateIdle();		break;	// �ҋ@���
	case (int)EState::ePatrol:		UpdatePatrol();		break;	// ������
	case (int)EState::eBattleIdle:	UpdateBattleIdle();	break;	// �퓬�ҋ@���
	case (int)EState::eChase:		UpdateChase();		break;	// �ǐՏ��
	case (int)EState::eLost:		UpdateLost();		break;	// ���������
	case (int)EState::eAttack:		UpdateAttack();		break;	// �U�����
	case (int)EState::eGuard:		UpdateGuard();		break;	// �h����
	case (int)EState::eAvoid:		UpdateAvoid();		break;	// ������
	case (int)EState::eHit:			UpdateHit();		break;	// ��������
	case (int)EState::eStun:		UpdateStun();		break;	// �������
	case (int)EState::eStunWait:	UpdateStunWait();	break;	// �����҂����
	case (int)EState::eParried:		UpdateParried();	break;	// �p���B���ꂽ���
	case (int)EState::eDeath:		UpdateDeath();		break;	// ���S���
	}

	// �L�����N�^�[�̍X�V
	CEnemyBase::Update(); 

	// �o�H�T���p�̃m�[�h���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// �R���C�_���X�V
	mpLHandCol->Update();
	mpRHandCol->Update();
	mpLFootCol->Update();
	mpRFootCol->Update();
	mpHeadCol->Update();

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	//CDebugPrint::Print("���G�̏��\n");
	//CDebugPrint::Print("�@HP�F%d\n", mHp);
	//CDebugPrint::Print("�@���ݓx�F%.2f\n", mStunPoints);
	CDebugPrint::Print("�@��ԁF%s\n", GetStateStr(mState).c_str());
	//CDebugPrint::Print("�@�U���^�C�v�F%s\n", GetAttackTypeStr(mAttackType).c_str());
	//CDebugPrint::Print("�@�U���̋����F%s\n", GetAttackPowerStr().c_str());
	//CDebugPrint::Print("�@�U���̕����F%s\n", GetAttackDirStr().c_str());
	//CDebugPrint::Print("�@�o�ߎ��ԁF%.2f\n", GetAnimationFrame());
	//CDebugPrint::Print("�@�퓬���̑ҋ@���ԁF%.2f\n", mBattleIdletime);
	//CDebugPrint::Print("�@�~�^���̈ړ��p�x�F%.2f\n", mRandMoveAngle);
	//CDebugPrint::Print("�@�p���B�o�������F%s\n", mIsAttackParry ? "�p���BOK" : "�p���BNO");

	mIsHitWall = false;
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
		mpRHandCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eBlowL)
	{
		mpLHandCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eRoundKickL)
	{
		mpLFootCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eRoundKickR)
	{
		mpRFootCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eHeadButt)
	{
		mpHeadCol->SetEnable(true);
	}
}

void CEnemyA::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CEnemyBase::AttackEnd();

	// �U���R���C�_�[���I�t
	mpLHandCol->SetEnable(false);
	mpRHandCol->SetEnable(false);
	mpLFootCol->SetEnable(false);
	mpRFootCol->SetEnable(false);
	mpHeadCol->SetEnable(false);
}

bool CEnemyA::IsGuarding() const
{
	if (mState == (int)EState::eGuard) return true;
	return false;
}

void CEnemyA::TakeDamage(int damage, float stan, float knockback, CObjectBase* causer)
{
	// �x�[�X�N���X�̃_���[�W�������Ăяo��
	CEnemyBase::TakeDamage(damage, stan, knockback, causer);

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
			LookAtBattleTarget(false);
		}
	}

}

void CEnemyA::Parry()
{
	ChangeState((int)EState::eParried);
}

// �����菈��
void CEnemyA::Hit()
{
	// �������Ԃ̃A�j���[�V�����������_���Őݒ�
	mRandHitAnim = Math::Rand(0, 2);
	ChangeState((int)EState::eHit);
}

// ���ݏ���
void CEnemyA::Stun()
{
	ChangeState((int)EState::eStun);
}

// ���S����
void CEnemyA::Death()
{
	ChangeState((int)EState::eDeath);
}

// �Փˏ���
void CEnemyA::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayer* player = CPlayer::Instance();

	// �x�[�X�̏Փˏ������Ăяo��
	CEnemyBase::Collision(self, other, hit);

	if (self == mpLHandCol || self == mpRHandCol 
		|| self == mpLFootCol || self == mpRFootCol
		|| self == mpHeadCol
		)
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

				if (chara->IsGuardParry() && CheckGuardParry())
				{
					TakeDamage(0, 1000, 0.0f, chara);
				}
				else
				{
					// �^����_���[�W�̌v�Z
					int damage = 0;
					float stan = 0.0f;
					float knockback = 0.0f;
					CalcDamage(chara, &damage, &stan, &knockback);

					// �_���[�W��^����
					chara->TakeDamage(damage, stan, knockback, this);
				}
			}
		}
	}
}

void CEnemyA::CalcDamage(CCharaBase* taker, int* outDamage, float* outStan, float* outKnockback) const
{
	switch (mAttackType)
	{
	case (int)EAttackType::eBlowL:
		*outDamage = BLOW_DMG;
		*outStan = BLOW_STUN;
		*outKnockback = BLOW_KNOCKBACK;
		break;
	case (int)EAttackType::eBlowR:
		*outDamage = BLOW_DMG;
		*outStan = BLOW_STUN;
		*outKnockback = BLOW_KNOCKBACK;
		break;
	case (int)EAttackType::eRoundKickL:
		*outDamage = KICK_DMG;
		*outStan = KICK_STUN;
		*outKnockback = KICK_KNOCKBACK;
		break;
	case (int)EAttackType::eRoundKickR:
		*outDamage = KICK_DMG;
		*outStan = KICK_STUN;
		*outKnockback = KICK_KNOCKBACK;
		break;
	case (int)EAttackType::eTackle:
		*outDamage = TACKLE_DMG;
		*outStan = TACKLE_STUN;
		*outKnockback = TACKLE_KNOCKBACK;
		break;
	case (int)EAttackType::eHeadButt:
		*outDamage = HEADBUTT_DMG;
		*outStan = HEADBUTT_STUN;
		*outKnockback = HEADBUTT_KNOCKBACK;
		break;
	}

	if (taker->IsGuarding())
	{
		*outDamage *= 0.7f;
		*outStan *= 0.7f;
		*outKnockback *= 0.3f;
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

	if (!IsFoundPlayer()) return false;

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
bool CEnemyA::IsMoveAttackRange()
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
	// �v���C���[�̍U���͈͂��H
	if (IsPlayerAttackRange()) return true;
}

// �v���C���[�̍U�������m���̏���
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
				ChangeState((int)EState::eGuard);

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

// �v���C���[�̍U���͈͓����H
bool CEnemyA::IsPlayerAttackRange() const
{
	CPlayer* player = CPlayer::Instance();
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
		if (mElapsedTime < TACKLE_COOL_TIME)
		{
			mElapsedTime += Times::DeltaTime();
			nextstate = EState::eChase;
		}
		else
		{
			int attackRand = Math::Rand(0, 99);

			if (attackRand < TACKLE_PROB)
			{
				mElapsedTime = 0.0f;
				nextattack = EAttackType::eTackle;
				nextstate = EState::eAttack;
			}
			else
			{
				mElapsedTime = 0.0f;
				nextstate = EState::eChase;
			}
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
bool CEnemyA::ChangePatrolPoint()
{
	// ���g�̌o�H�T���p�m�[�h���X�V���̏ꍇ�́A�������Ȃ�
	if (mpNavNode->IsUpdaing()) return false;
	// ����|�C���g���ݒ肳��Ă��Ȃ��ꍇ�́A�������Ȃ�
	int size = mPatrolPoints.size();
	if (size == 0) return false;

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

	return mNextPatrolIndex >= 0;
}

bool CEnemyA::UpdatePatrolRoute()
{
	// ����|�C���g�̌o�H�T���m�[�h�̈ʒu��ݒ肵�������ƂŁA
	// �e�m�[�h�ւ̐ڑ������X�V
	for (CNavNode* node : mPatrolPoints)
	{
		node->SetPos(node->GetPos());
	}

	if (!(0 <= mNextPatrolIndex && mNextPatrolIndex < mPatrolPoints.size())) return false;

	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr) return false;

	// ���g�̃m�[�h���X�V���Ȃ�΁A�o�H�T�����s��Ȃ�
	if (mpNavNode->IsUpdaing()) return false;
	// ����|�C���g���X�V���Ȃ�΁A�o�H�T�����s��Ȃ�
	CNavNode* patrolPoint = mPatrolPoints[mNextPatrolIndex];
	if (patrolPoint->IsUpdaing()) return false;
	// ����|�C���g�܂ł̍ŒZ�o�H�����߂�
	if (navMgr->Navigate(mpNavNode, patrolPoint, mMoveRoute))
	{
		// ���̖ړI�n�̃C���f�b�N�X��ݒ�
		mNextMoveIndex = 1;
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
		if (ChangePatrolPoint())
		{
			mStateStep++;
		}
		break;
	// �X�e�b�v1�F����|�C���g�܂ł̌o�H�T��
	case 1:
		if (UpdatePatrolRoute())
		{
			mStateStep++;
		}
		break;
	// �X�e�b�v2�F����|�C���g�܂ňړ�
	case 2:
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
	// �X�e�b�v3�F�ړ���̑ҋ@
	case 3:
		ChangeAnimation((int)EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// �ҋ@�ő厞�Ԃ������_���Ō��߂�
			mIdleTime = Math::Rand(IDLE_TIME_MIN, IDLE_TIME_MAX);
			if (ChangePatrolPoint())
			{
				mStateStep = 1;
				mElapsedTime = 0.0f;
			}
		}
		break;
	}
}

// �퓬�ҋ@��Ԃ̍X�V����
void CEnemyA::UpdateBattleIdle()
{
	if (DetectedPlayerAttack()) return;

	if (!IsFoundPlayer())
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// �v���C���[�̍��W�֌����Ĉړ�����
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();

	if (mRandMoveAngle < -10.0f)
	{
		ChangeAnimation((int)EAnimType::eBattleWalkR);
	}
	else if (mRandMoveAngle > 10.0f)
	{
		ChangeAnimation((int)EAnimType::eBattleWalkL);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eBattleIdle);
	}

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
		if (mBattleElapsedTime < mBattleIdletime)
		{
			
			UpdateHorizonMove();
			//UpdateForwardMove();

			// �v���C���[�������Ȃ��Ȃ�����A����������Ԃɖ߂�
			if (!IsLookPlayer())
			{
				mpLostPlayerNode->SetPos(targetPos);
				ChangeState((int)EState::eLost);
				return;
			}
			mBattleElapsedTime += Times::DeltaTime();
			
		}
		else
		{
			mRandMoveAngle = Math::Rand(-30.0f, 30.0f);
			// �U���̎�ނƃX�e�[�g�����߂�
			AttackPickDetect();
			// �퓬�ҋ@���Ԃ�������
			mBattleIdletime = 0.0f;
			mBattleElapsedTime = 0.0f;
		}
		break;
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
		// ���g�̃m�[�h�ƃv���C���[�̃m�[�h���X�V���łȂ���΁A
		// ���g�̃m�[�h����v���C���[�̃m�[�h�܂ł̍ŒZ���������߂�
		CNavNode* playerNode = player->GetNavNode();
		if (!mpNavNode->IsUpdaing() && 
			playerNode != nullptr && !playerNode->IsUpdaing())
		{
			if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
			{
				// ���g�̃m�[�h����v���C���[�̃m�[�h�܂Ōq�����Ă�����A
				// �ړ�����ʒu�����̃m�[�h�̈ʒu�ɐݒ�
				targetPos = mMoveRoute[1]->GetPos();
			}
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
	CPlayer* target = CPlayer::Instance();

	if (navMgr == nullptr || target == nullptr)
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
	case 0:
		mElapsedTime = 0.0f;
		mStateStep++;
		break;
		// �X�e�b�v1�F���������ʒu�܂ł̍ŒZ�o�H�����߂�
	case 1:
		// 
		mpLostPlayerNode->SetPos(target->Position());

		// ���g�̃m�[�h�ƌ��������ʒu�̃m�[�h���X�V���łȂ����
		if (!mpNavNode->IsUpdaing() && !mpLostPlayerNode->IsUpdaing())
		{
			if (!navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
			{
				// �o�H���q�����Ă��Ȃ���΁A�ҋ@��Ԃ֖߂�
				ChangeState((int)EState::eIdle);
				mpLostPlayerNode->SetEnable(false);

			}
			else
			{
				// ���������ʒu�܂Ōo�H���q�����Ă�����A���̃X�e�b�v��
				mNextMoveIndex = 1;
				mStateStep++;
			}
		}
		break;
	case 2:
		// ���̈ړ���̃C���f�b�N�X�l���s���l�łȂ����
		if (0 <= mNextMoveIndex && mNextMoveIndex < mMoveRoute.size())
		{
			// �v���C���[�����������ʒu�܂ňړ�
			if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
			{
				if (mNextMoveIndex >= mMoveRoute.size())
				{
					// �퓬��ԏI��
					mIsBattle = false;
					mpBattleTarget = nullptr;
					// �ړ����I���΁A�ҋ@��Ԃֈڍs
					ChangeState((int)EState::eIdle);
				}
				else
				{
					if (mElapsedTime <= LOST_TIME)
					{
						mElapsedTime += Times::DeltaTime();
					}
					else
					{
						// �퓬��ԏI��
						mIsBattle = false;
						mpBattleTarget = nullptr;
						// �ړ����I���΁A�ҋ@��Ԃֈڍs
						ChangeState((int)EState::eIdle);
					}
				}
			}
		}
		break;
	}
}

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
	case (int)EAttackType::eTackleWait:		UpdateTackleWait();		break;
	case (int)EAttackType::eHeadButt:		UpdateHeadButt();		break;
	case (int)EAttackType::eTripleAttack:	UpdateTripleAttack();	break;
	}



	if (mAttackType == (int)EAttackType::eNone)
	{
		ChangeState((int)EState::eBattleIdle);
	}
}

void CEnemyA::UpdateGuard()
{
	CPlayer* player = CPlayer::Instance();
	mMoveSpeed = CVector::zero;
	ChangeAnimation((int)EAnimType::eGuardIdle);
	if (player->IsAttacking())
	{
		mIsGuard = true;
	}
	else
	{
		mIsGuard = false;
	}
	if (!mIsGuard) return ChangeState((int)EState::eBattleIdle);
}

// ������̍X�V����
void CEnemyA::UpdateAvoid()
{

}

// �����莞�̍X�V����
void CEnemyA::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
	{
		EAnimType hitAnim = EAnimType::eHit1;
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos - VectorZ() * mKnockBack;

		if (mRandHitAnim == 0)
		{
			hitAnim = EAnimType::eHit1;
		}
		else if (mRandHitAnim == 1)
		{
			hitAnim = EAnimType::eHit2;
		}
		else if (mRandHitAnim == 2)
		{
			hitAnim = EAnimType::eHit3;
		}

		ChangeAnimation((int)hitAnim, true);
		mStateStep++;
		break;
	}
	case 1:
	{
		float frame = GetAnimationFrame();
		float moveStartFrame = mAnimationFrameSize * 0.1f;
		float moveEndFrame = mAnimationFrameSize * 0.9f;
		if (GetAnimationFrameRatio() >= 0.1)
		{
			if (GetAnimationFrameRatio() <= 0.9)
			{
				// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
				float moveFrame = moveEndFrame - moveStartFrame;
				float percent = (frame - moveStartFrame) / moveFrame;
				CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent * 1.3);
				Position(pos);
			}
			else
			{
				Position(mMoveEndPos);
				mStateStep++;
			}
		}

		break;
	}
	case 2:
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// ���ݎ��̍X�V����
void CEnemyA::UpdateStun()
{
	// TODO:�A�j���[�V�������Đ��A�P�b��ɕ����A
	// ���E�ɓ����Ă�����F�U����ԂɁA�����Ă��Ȃ�������F�ҋ@��Ԃ�
	// �P�b�̊ԂōU��������ƁF�U���`�����X�I�I��Ԃ�
	switch (mStateStep)
	{
		// ���݃A�j���[�V�����Đ�
	case 0:
		ChangeAnimation((int)EAnimType::eStun, true);
		mStateStep++;
		break;
		// �A�j���[�V�����I���҂�
	case 1:
		// �A�j���[�V�������I���Αҋ@��Ԃ֖߂�
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eStunWait);
			mStateStep++;
		}
		break;
	}
}

// �����҂��̍X�V����
void CEnemyA::UpdateStunWait()
{
	switch (mStateStep)
	{
	case 0:
		ChangeAnimation((int)EAnimType::eStunWait);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
	case 2:
		if (mElapsedTime < STUN_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// �p���B���ꂽ���̍X�V����
void CEnemyA::UpdateParried()
{
	switch (mStateStep)
	{
		// ���݃A�j���[�V�����Đ�
	case 0:
		ChangeAnimation((int)EAnimType::eStun, true);
		mStateStep++;
		break;
		// �A�j���[�V�����I���҂�
	case 1:
		// �A�j���[�V�������I���Αҋ@��Ԃ֖߂�
		if (IsAnimationFinished())
		{
			mIsAttackParry = false;
			mIsGuardParry = false;
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// ���S���̍X�V����
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


// �ガ�����U���̍X�V����
void CEnemyA::UpdateBlowL()
{
	switch (mStateStep)
	{
	case 0: // �A�j���[�V�����Đ�
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * BLOW_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eBlowL, true);
		mStateStep++;
		break;
	case 1: // �U���p�R���C�_�[�I��
	{
		// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		float blowMoveStart = data.attackParryEndFrame;
		AttackStart();
		if (!IsMoveAttackRange()) {
			if (frame >= blowMoveStart)
			{
				// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
				if (frame < BLOW_MOVE_END)
				{
					// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
					float moveFrame = BLOW_MOVE_END - blowMoveStart;
					float percent = (frame - blowMoveStart) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				// �ړ��I���t���[���܂œ��B�����ꍇ
				else
				{
					Position(mMoveEndPos);
					mStateStep++;
				}
			}
			// �U���R���C�_�[���I���ɂȂ�܂ŁA�v���C���[�̕����֌���
			else
			{
				LookAtBattleTarget();
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
			if (mIsTripleAttack)
			{
				mAttackCount++;
				mStateStep++;
			}
			else
			{
				// ���Ԃ��o�߂�����A�ҋ@��Ԃֈڍs
				ChangeState((int)EState::eBattleIdle);
			}
		}
		break;
	case 3:	// �X�e�b�v3 : 
		mStateStep = 0;
		ChangeAttackType((int)EAttackType::eTripleAttack);
		break;
	}
}

// �ガ�����U���̍X�V����
void CEnemyA::UpdateBlowR()
{
	switch (mStateStep)
	{
	case 0: // �A�j���[�V�����Đ�
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * BLOW_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eBlowR, true);
		mStateStep++;
		break;
	case 1: // �U���p�R���C�_�[�I��
	{
		// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		float blowMoveStart = data.attackParryEndFrame;
		AttackStart();
		if (!IsMoveAttackRange()) {
			if (frame >= blowMoveStart)
			{
				// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
				if (frame < BLOW_MOVE_END)
				{
					// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
					float moveFrame = BLOW_MOVE_END - blowMoveStart;
					float percent = (frame - blowMoveStart) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				// �ړ��I���t���[���܂œ��B�����ꍇ
				else
				{
					Position(mMoveEndPos);
					mStateStep++;
				}
			}
			// �U���R���C�_�[���I���ɂȂ�܂ŁA�v���C���[�̕����֌���
			else
			{
				LookAtBattleTarget();
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
			if (mIsTripleAttack)
			{
				mAttackCount++;
				mStateStep++;
			}
			else
			{
				// ���Ԃ��o�߂�����A�ҋ@��Ԃֈڍs
				ChangeState((int)EState::eBattleIdle);
			}
		}
		break;
	case 3:	// �X�e�b�v3 : 
		mStateStep = 0;
		ChangeAttackType((int)EAttackType::eTripleAttack);
		break;
	}
}

// �񂵏R��U���̍X�V����
void CEnemyA::UpdateRoundKickL()
{
	switch (mStateStep)
	{
		case 0:	// �X�e�b�v�O�F�U���A�j���[�V�����A�U���J�n�ʒu�ƍU���I���ʒu�̐ݒ�
			mMoveStartPos = Position();
			mMoveEndPos = mMoveStartPos + VectorZ() * KICK_MOVE_DIST;
			ChangeAnimation((int)EAnimType::eRoundKickL, true);
			mStateStep++;
			break;
		case 1:	// �X�e�b�v�P�F�U�����̈ړ�����
		{
			// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
			float frame = GetAnimationFrame();
			const AttackData& data = (*mpAttackData)[mAttackType];
			float kickMoveStart = data.attackParryEndFrame;
			AttackStart();
			if (!IsMoveAttackRange()) {
				if (frame >= kickMoveStart)
				{
					// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
					if (frame < KICK_MOVE_END)
					{
						// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
						float moveFrame = KICK_MOVE_END - kickMoveStart;
						float percent = (frame - kickMoveStart) / moveFrame;
						CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
						Position(pos);
					}
					// �ړ��I���t���[���܂œ��B�����ꍇ
					else
					{
						Position(mMoveEndPos);
						mStateStep++;
					}
				}
				else
				{
					LookAtBattleTarget();
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

// �񂵏R��U���̍X�V����
void CEnemyA::UpdateRoundKickR()
{
	switch (mStateStep)
	{
	case 0:	// �X�e�b�v�O�F�U���A�j���[�V�����A�U���J�n�ʒu�ƍU���I���ʒu�̐ݒ�
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * KICK_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eRoundKickR, true);
		mStateStep++;
		break;
	case 1:	// �X�e�b�v�P�F�U�����̈ړ�����
	{
		// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		float kickMoveStart = data.attackParryEndFrame;
		AttackStart();
		if (!IsMoveAttackRange()) {
			if (frame >= kickMoveStart)
			{
				// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
				if (frame < KICK_MOVE_END)
				{
					// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
					float moveFrame = KICK_MOVE_END - kickMoveStart;
					float percent = (frame - kickMoveStart) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				// �ړ��I���t���[���܂œ��B�����ꍇ
				else
				{
					Position(mMoveEndPos);
					mStateStep++;
				}
			}
			else
			{
				LookAtBattleTarget();
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

// �^�b�N���̍X�V����
void CEnemyA::UpdateTackle()
{

	switch (mStateStep)
	{
	case 0:	// �X�e�b�v0 : �U���A�j���[�V����
		// �U���J�n�ʒu�ƍU���I���ʒu�̐ݒ�
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * TACKLE_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eTackle, false);
		mStateStep++;
		break;
	case 1:	// �X�e�b�v�P�F�U�����̈ړ�����
	{
		// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
		float frame = GetAnimationFrame();
		AttackStart();
		if (!IsMoveAttackRange()) {
			if (frame >= TACKLE_MOVE_START)
			{
				// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
				if (frame < TACKLE_MOVE_END)
				{
					// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
					float moveFrame = TACKLE_MOVE_END - TACKLE_MOVE_START;
					float percent = (frame - TACKLE_MOVE_START) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				// �ړ��I���t���[���܂œ��B�����ꍇ
				else
				{
					Position(mMoveEndPos);
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
			// �^�b�N���̑҂���ԂɈڍs����
			ChangeAttackType((int)EAttackType::eTackleWait);
		}
		break;
	}
}

// �^�b�N���I�����̗\������̍X�V����
void CEnemyA::UpdateTackleWait()
{
	switch (mStateStep)
	{
	case 0:
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * TACKLE_WAIT_DIST;
		ChangeAnimation((int)EAnimType::eTackleWait);
		mStateStep++;
		break;
	case 1:
	{
		float frame = GetAnimationFrame();
		if (frame >= TACKLE_WAIT_START)
		{
			if (frame < TACKLE_WAIT_END)
			{
				// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
				float moveFrame = TACKLE_WAIT_END - TACKLE_WAIT_START;
				float percent = (frame - TACKLE_WAIT_START) / moveFrame;
				CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
				Position(pos);
			}
			else
			{
				Position(mMoveEndPos);
				mStateStep++;
			}
		}
		break;
	}
	case 2:
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// ���˂��U���̍X�V����
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
		if (frame >= data.attackParryEndFrame)
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

// �O�A�U���̍X�V����
void CEnemyA::UpdateTripleAttack()
{
	switch (mAttackCount)
	{
	case 0:
	{
		int attackrand = Math::Rand(0, 99);
		// �O�����Z�q
		ChangeAttackType(attackrand < FIFTY_FIFTY_PROB ? 
			(int)EAttackType::eBlowL :(int)EAttackType::eBlowR);
		break;
	}
	case 1:
	{
		if (mElapsedTime < TRIPLE_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			int attackrand = Math::Rand(0, 99);
			ChangeAttackType(attackrand < FIFTY_FIFTY_PROB ?
				(int)EAttackType::eBlowL : (int)EAttackType::eBlowR);
			mElapsedTime = 0;
		}
		break;
	}
	case 2:
	{
		if (mElapsedTime < TRIPLE_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			int attackrand = Math::Rand(0, 99);
			ChangeAttackType(attackrand < FIFTY_FIFTY_PROB ?
				(int)EAttackType::eBlowL : (int)EAttackType::eBlowR);
			mElapsedTime = 0;
		}
		break;
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
	case (int)EState::eIdle:		return "�ҋ@";
	case (int)EState::ePatrol:		return "����";
	case (int)EState::eBattleIdle:	return "�ҋ@(�퓬)";
	case (int)EState::eChase:		return "�ǐ�";
	case (int)EState::eLost:		return "������";
	case (int)EState::eAttack:		return "�U��";
	case (int)EState::eGuard:		return "�h��";
	case (int)EState::eAvoid:		return "���";
	case (int)EState::eHit:			return "������";
	case (int)EState::eStun:		return "����";
	case (int)EState::eStunWait:	return "�����҂�����";
	case (int)EState::eParried:		return "�p���B";
	case (int)EState::eDeath:		return "���S";
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
	case (int)EAttackType::eTackle:			return "�^�b�N��";
	case (int)EAttackType::eTackleWait:		return "�^�b�N���I�����̗\������";
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
	case (int)EState::eStun:	return CColor::cyan;
	}
	return CColor::white;
}

// ���ړ�(horizontal)
void CEnemyA::UpdateHorizonMove()
{
	// ���g�̍��W
	CVector pos = Position();
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();
	// �v���C���[���玩�g�܂ł̃x�N�g�������߂�
	CVector vec = pos - targetPos;

	// �ǂɓ���������t�����Ɉړ�����
	if (mIsHitWall)
	{
		float rand = Math::Rand(-10.0f, 10.0f);
		mRandMoveAngle = -mRandMoveAngle + rand;
	}


	float rotAngle = mRandMoveAngle * Times::DeltaTime();
	// ���߂��x�N�g����Y���ɉ�]
	CVector rotVec = CQuaternion(0.0f, rotAngle, 0.0f) * vec;
	// �v���C���[�̍��W�{��]��̃x�N�g���ŁA�ړ���̍��W�����߂�
	pos.X(targetPos.X() + rotVec.X());
	pos.Z(targetPos.Z() + rotVec.Z());

	// �v���C���[�̕�������
	LookAtBattleTarget();

	if (!IsPlayerAttackRange()) return;

	Position(pos);

}

void CEnemyA::UpdateForwardMove()
{
	// ���g�̍��W
	CVector pos = Position();
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();
	// �v���C���[���玩�g�܂ł̃x�N�g�������߂�
	CVector vec = pos - targetPos;
	float dist = vec.Length();
	float moveForward = 15.0f * Times::DeltaTime();
	vec = vec.Normalized() * (dist - moveForward);

	pos.X(targetPos.X() + vec.X());
	pos.Z(targetPos.Z() + vec.Z());

	// �v���C���[�̕�������
	LookAtBattleTarget();

	Position(pos);

}

void CEnemyA::UpdateBackMove()
{

}
