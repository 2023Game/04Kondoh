//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CEnemyA.h"
#include "CInput.h"
#include "CCamera.h"
#include "CGameCamera2.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CField.h"
#include "CEnemyManager.h"
#include "CEnemyA.h"
#include "CGaugeUI2D.h"
#include "CInteractObject.h"

#include "CNavNode.h"
#include "CNavManager.h"
#include "CDebugFieldOfView.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

#define PLAYER_HP			1000	// �v���C���[��HP
#define PLAYER_CAP_UP		13.5f	// �v���C���[�̍���
#define PLAYER_CAP_DWON		 2.8f	// �v���C���[�̒�
#define PLAYER_WIDTH		 3.0f	// �v���C���[�̕�

#define ATTACK1_CAP_UP		80.0f	// �U���R���C�_�[1�̏�
#define ATTACK1_CAP_DWON	0.0f	// �U���R���C�_�[1�̉�
#define ATTACK2_CAP_UP		0.0f	// �U���R���C�_�[2�̏�
#define ATTACK2_CAP_DWON	-30.0f	// �U���R���C�_�[2�̉�

#define DAMAGE_S		1.0f	// ��U���̃_���[�W
#define DAMAGE_M		2.0f	// ���U���̃_���[�W
#define DAMAGE_L		3.0f	// ���U���̃_���[�W
#define DAMAGE_DIA		2.0f	// �_���[�W�̔{��

#define STAN_VAL_S		10.0f	// ��U���̃X�^���l
#define STAN_VAL_M		20.0f	// ���U���̃X�^���l
#define STAN_VAL_L		30.0f	// ���U���̃X�^���l
#define STAN_VAL_DIA	2.0f	// �X�^���l�̔{��

#define KNOCKBACK_S		6.0f	// ��U���̃m�b�N�o�b�N����
#define KNOCKBACK_M		8.0f	// ���U���̃m�b�N�o�b�N����
#define KNOCKBACK_L		10.0f	// ���U���̃m�b�N�o�b�N����

#define DEATH_RPOB 40	// ���S�A�j���[�V�����m��
#define DEATH_WAIT_TIME	5.0

#define WALK_SPEED	0.8f
#define RUN_SPEED	1.2f
#define JUMP_WALK_SPEED 1.0f
#define JUMP_RUN_SPEED 1.5f
#define JUMP_SPEED	1.5f
#define GRAVITY		0.08f // 0.0625
#define JUMP_END_Y	1.0f  

#define EVA_MOVE_DIST	90.0f  // ������̈ړ����x
#define EVA_MOVE_START   5.0f  // ������̈ړ��J�n�t���[�� 
#define EVA_MOVE_END    40.0f  // ������̈ړ��I���t���[�� 

#define RUN_INPUT_INTERVAL 0.15f // �_�b�V���L�[�̓��̓C���^�[�o��

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

// �A�j���[�V�����t�@�C���̃p�X
#define PLAYER_ANIM_PATH "Character\\Player\\anim\\"
// �U���A�j���[�V�����t�@�C���̃p�X
#define ATTACK_ANIM_PATH "Character\\Player\\anim\\Attack\\"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",								 true,	  0.0f,	 1.0f},	// �퓬����T�|�[�Y
	{ PLAYER_ANIM_PATH"BattleIdle.x",	 true,	170.0f,	 1.0f},	// �퓬���̑ҋ@

	{ PLAYER_ANIM_PATH"BattleWalk.x",	 true,	 34.0f,	10.0f},	// ���s
	{ PLAYER_ANIM_PATH"BackWalk.x",		 true,	 39.0f,	 1.0f},	// ���������s
	{ PLAYER_ANIM_PATH"LeftWalk.x",		 true,	 40.0f,	 1.0f},	// ���������s
	{ PLAYER_ANIM_PATH"RightWalk.x",	 true,	 35.0f,	 1.0f},	// �E�������s

	{ PLAYER_ANIM_PATH"Run.x",			 true,	 35.0f,	 10.0f},	// ����
	{ PLAYER_ANIM_PATH"BackRun.x",		 true,	 26.0f,	 1.0f},	// �������֑���
	{ PLAYER_ANIM_PATH"LeftRun.x",		 true,	 30.0f,	 0.01f},	// �������֑���
	{ PLAYER_ANIM_PATH"RightRun.x",		 true,	 30.0f,	 0.01f},	// �E�����֑���

	{ ATTACK_ANIM_PATH"UpAttackS.x",	false,	 54.0f,	 1.0f},	// ���U��
	{ ATTACK_ANIM_PATH"UpAttackM.x",	false,	 69.0f,	 0.1f},	// ����U��
	{ ATTACK_ANIM_PATH"UpAttackL.x",	false,	 92.0f,	 1.0f},	// ����U��

	{ ATTACK_ANIM_PATH"RightAttackS.x",	false,	 60.0f,	 1.0f},	// ��E�U��
	{ ATTACK_ANIM_PATH"RightAttackM.x",	false,	 65.0f,	 1.5f},	// ���E�U��
	{ ATTACK_ANIM_PATH"RightAttackL.x",	false,	 86.0f,	 1.0f},	// ���E�U��

	{ ATTACK_ANIM_PATH"LeftAttackS.x",	false,	 50.0f,	 1.0f},	// �㍶�U��
	{ ATTACK_ANIM_PATH"LeftAttackM.x",	false,	 60.0f,	 1.0f},	// �����U��
	{ ATTACK_ANIM_PATH"LeftAttackL.x",	false,	 99.0f,	 1.0f},	// �����U��

	{ PLAYER_ANIM_PATH"GuardStart.x",	false,	 18.0f,	 0.01f},// �h��J�n
	{ PLAYER_ANIM_PATH"Guard.x",		 true,	 43.0f,	 1.0f},	// �h�䒆
	{ PLAYER_ANIM_PATH"GuardEnd.x",		false,	 21.0f,	 0.01f},// �h��I��
	{ PLAYER_ANIM_PATH"GuardDamage.x",	false,	 24.0f,	 1.0f},	// �h�䎞�̋�����
	{ PLAYER_ANIM_PATH"GuardParry.x",	false,	 26.0f,	 1.0f},	// �h�䎞�̃p���B
	{ PLAYER_ANIM_PATH"Avoid.x",		false,	 50.0f,	 3.0f},	// ���

	{ PLAYER_ANIM_PATH"jump_start.x",	false,	 25.0f,	 1.0f},	// �W�����v�J�n
	{ PLAYER_ANIM_PATH"jump.x",			 true,	  1.0f,	 1.0f},	// �W�����v��
	{ PLAYER_ANIM_PATH"jump_end.x",		false,	 26.0f,	 2.0f},	// �W�����v�I��
	{ PLAYER_ANIM_PATH"jump_Attack.x",	false,	 47.0f,	 1.0f},	// �W�����v�U��

	{ PLAYER_ANIM_PATH"Damage1.x",		false,	 30.0f,	 1.0f},	// ������1
	{ PLAYER_ANIM_PATH"Damage2.x",		false,	 27.0f,	 1.0f},	// ������2
	{ PLAYER_ANIM_PATH"Death1.x",		false,	118.0f,	 1.0f},	// ���S�P
	{ PLAYER_ANIM_PATH"Death2.x",		false,	 70.0f,	 1.0f},	// ���S�Q
};


// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eBattleIdle)
	, mSelectAttackPower(EAttackPower::eAttackM)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mpRideObject(nullptr)
	, mpSearchCol(nullptr)
	, mIsGrounded(false)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mIsBattleMode(true)
	, mIsAvoiding(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	mMaxHp = PLAYER_HP;
	mHp = mMaxHp;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Player");

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
	ChangeAnimation(EAnimType::eBattleTPose);

	// �J�v�Z���R���C�_�[�쐬
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, PLAYER_CAP_DWON, 0.0f),
		CVector(0.0f, PLAYER_CAP_UP, 0.0f),
		PLAYER_WIDTH, true
	);
	mpBodyCol->SetCollisionTags
	({ ETag::eEnemy, ETag::eField, ETag::eRideableObject, ETag::eWall, ETag::eInteractObject });
	mpBodyCol->SetCollisionLayers
	({ ELayer::eField, ELayer::eWall,ELayer::eAttackCol,ELayer::eEnemy, ELayer::eInteractObj });


	// �U���p�̃R���C�_�P�i���̐n�̕����j
	mpAttackCol1 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK1_CAP_DWON),
		CVector(0.0f, 0.0f, ATTACK1_CAP_UP),
		1.0f, true
	);
	mpAttackCol1->SetCollisionTags({ ETag::eEnemy, ETag::eInteractObject });
	mpAttackCol1->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall, ELayer::eInteractObj });
	mpAttackCol1->Rotate(CVector(-4.5f, 14.1f, 0.0f));
	mpAttackCol1->SetEnable(false);

	// �U���p�̃R���C�_�Q�i���̎�����̕����j
	mpAttackCol2 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK2_CAP_UP),
		CVector(0.0f, 0.0f, ATTACK2_CAP_DWON),
		1.0, true
	);
	mpAttackCol2->SetCollisionTags({ ETag::eEnemy, ETag::eInteractObject });
	mpAttackCol2->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall, ELayer::eInteractObj });
	mpAttackCol2->Rotate(CVector(-4.5f, 14.1f, 0.0f));
	mpAttackCol2->SetEnable(false);

	// �U���p�̃R���C�_�R�i���̕����j
	mpAttackCol3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		25.0, true
	);
	mpAttackCol3->SetCollisionTags({ ETag::eEnemy, ETag::eInteractObject });
	mpAttackCol3->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall, ELayer::eInteractObj });
	mpAttackCol3->Translate(0.0f, 0.0f, -5.0f);
	mpAttackCol3->SetEnable(false);

	//
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		10.0f
	);
	// ���ׂ�I�u�W�F�N�g�̂ݏՓ˂���悤�ɐݒ�
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eInteractObj });
	mpSearchCol->Position(0.0f, 5.3f, 0.0f);


	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// ���̃{�[�����擾
	CModelXFrame* Sword = mpModel->FinedFrame("Armature_mixamorig_Sword_joint");
	const CMatrix& swordMTX = Sword->CombinedMatrix();
	// ���̃{�[�����擾
	CModelXFrame* Shield = mpModel->FinedFrame("Armature_mixamorig_Shield_joint");
	const CMatrix& shieldMTX = Shield->CombinedMatrix();

	// �U���p�̃R���C�_�[���s��ɐݒ�
	mpAttackCol1->SetAttachMtx(&swordMTX);
	mpAttackCol2->SetAttachMtx(&swordMTX);
	mpAttackCol3->SetAttachMtx(&shieldMTX);

	mpHpUI = new CGaugeUI2D();
	mpHpUI->SetMaxPoint(mMaxHp);
	mpHpUI->SetCurrPoint(mHp);
	mpHpUI->SetPos(CVector2(370.0f, 660.0f));

	mRandDeathAnim = Math::Rand(0, 99);
}

CPlayer::~CPlayer()
{
	// �R���C�_�[��j��
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpAttackCol1);
	SAFE_DELETE(mpAttackCol2);
	SAFE_DELETE(mpAttackCol3);
	SAFE_DELETE(mpSearchCol);

	// �o�H�T���p�̃m�[�h��j��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
	}

	spInstance = nullptr;
}

// �U������
bool CPlayer::IsAttacking() const
{
	if (mState == EState::eAttack) return true;
	return false;
}

// �U���J�n
void CPlayer::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CXCharacter::AttackStart();

	if (IsAttackType(EAttackPower::eAttackS, EAttackDir::eUp))
	{
		mpAttackCol2->SetEnable(true);
	}
	else if (IsAttackType(EAttackPower::eAttackS, EAttackDir::eLeft))
	{
		mpAttackCol3->SetEnable(true);
	}
	else
	{
		mpAttackCol1->SetEnable(true);
	}
}

// �U���I��
void CPlayer::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CXCharacter::AttackEnd();

	// �U���R���C�_�[���I�t
	mpAttackCol1->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
}

// �h�䒆��
bool CPlayer::IsGuarding() const
{
	if (mState == EState::eGuard) return true;
	return false;
}

bool CPlayer::IsGuardParry() const
{
	if (mState == EState::eGuardParry) return true;
	return false;
}

bool CPlayer::IsJumping() const
{
	if (mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		return true;
	}
	return false;
}

void CPlayer::TakeDamage(int damage, float stan, float knockback, CObjectBase* causer)
{
	// �x�[�X�N���X�̃_���[�W�������Ăяo��
	CCharaBase::TakeDamage(damage, stan, knockback, causer);
	// ���S���Ȃ����
	if (!IsDeath())
	{
		// �U��������������̕����֌���
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// �ړ����~
		mMoveSpeed = CVector::zero;
	}
}

// �����菈��
void CPlayer::Hit()
{
	mRandHitAnim = Math::Rand(0, 1);
	ChangeState(EState::eHit);
}

// ���S����
void CPlayer::Death()
{
	ChangeState(EState::eDeath);
}

// �X�V
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (CInput::PushKey('T'))
	{
		mIsBattleMode = !mIsBattleMode;
	}

	// TODO:�ʏ펞�ҋ@�Ɛ퓬���ҋ@�̐؂�ւ�
	//ChangeState(EState::eBattleIdle);

	// �}�E�X�z�C�[���̉�]�ʂ̍���
	int WheelDelta = CInput::GetDeltaMouseWheel();
	int powerNum = (int)EAttackPower::Num;

	// �}�E�X�z�C�[������ɃX�N���[������Ă�����A���݂̍U���З͂��A�b�v
	if (WheelDelta > 0)
	{
		int power = ((int)mSelectAttackPower + 1) % powerNum;
		mSelectAttackPower = (EAttackPower)power;
	}
	// �}�E�X�z�C�[�������ɃX�N���[������Ă�����A���݂̍U���З̓_�E��
	else if (WheelDelta < 0)
	{
		int power = ((int)mSelectAttackPower + powerNum - 1) % powerNum;
		mSelectAttackPower = (EAttackPower)power;
	}

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		//case EState::eIdle:			UpdateIdle();		break;
	case EState::eBattleIdle:	UpdateBattleIdle();	break;
	case EState::eAttack:		UpdateAttack();		break;
	case EState::eGuardStart:	UpdateGuardStart();	break;
	case EState::eGuard:		UpdateGuard();		break;
	case EState::eGuardEnd:		UpdateGuardEnd();	break;
	case EState::eGuardParry:	UpdateGuardParry();	break;
	case EState::eAvoid:		UpdateAvoid();		break;
	case EState::eJumpStart:	UpdateJumpStart();	break;
	case EState::eJump:			UpdateJump();		break;
	case EState::eJumpEnd:		UpdateJumpEnd();	break;
	case EState::eHit:			UpdateHit();		break;
	}

	// �ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == EState::eBattleIdle
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	if (mState == EState::eBattleIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		ChangeAttack();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// �ړ�
	Position(Position() + moveSpeed);

	// ���b�N�I���ƈړ��̎d���ƃo�g�����[�h
	if (!mIsBattleMode)
	{
		// �v���C���[���ړ������֌�����
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.15f); // 0.125f
		Rotation(CQuaternion::LookRotation(forward));
	}
	else if (mIsBattleMode && !mIsAvoiding)
	{
		// �J�����Ƀv���C���[�̌�����Ǐ]����
		// ���C���J�������擾
		CCamera* camera = CCamera::MainCamera();
		// �����_���擾
		CVector atPos = camera->GetAtPos();
		// �����_�ƃJ�����̈ʒu�̍������߂�
		CVector vec = atPos - camera->Position();
		vec.Y(0.0f);
		// �����x�N�g���ɂ���
		CVector targetPos = vec.Normalized();
		Rotation(CQuaternion::LookRotation(targetPos));
		// TODO�F�v���C���[��G�Ɍ�����
		// TODO�F�G�Ɏ��_���{�^���������ă��b�N���邩�A�����œG�Ɍ����悤�ɂ���


	}

	// �uP�v�L�[����������A�Q�[�����I��
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �R���C�_�[�̍s���ύX
	mpAttackCol1->Update();
	mpAttackCol2->Update();
	mpAttackCol3->Update();

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	mIsGrounded = false;
	mIsHittingWall = false;

	mpHpUI->SetMaxPoint(mMaxHp);
	mpHpUI->SetCurrPoint(mHp);

	CDebugPrint::Print("FPS:%f\n \n", Times::FPS());
	CVector pos = Position();

	CDebugPrint::Print("���v���C���[�̏��\n");
	//CDebugPrint::Print("�@HP�F%d\n", (int)mHp);
	CDebugPrint::Print("�@���W�F%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	//CDebugPrint::Print("�@���ݓx�F%.2f\n", mStunPoints);
	//CDebugPrint::Print("�@��ԁF", GetStateStr(mState).c_str());
	//CDebugPrint::Print("�@Grounded�F%s\n", mIsGrounded ? "true" : "false");
	//CDebugPrint::Print("�@�I�𒆂̍U���̋����F%d\n", (int)mSelectAttackPower);
	//CDebugPrint::Print("�@�m�b�N�o�b�N�����F%f\n", mKnockBack);
	//CDebugPrint::Print("�@�U���̋����F%s\n", GetAttackPowerStr().c_str());
	//CDebugPrint::Print("�@�U���̕����F%s\n", GetAttackDirStr().c_str());
	CDebugPrint::Print("�@\n");

	// ���ׂ�I�u�W�F�N�g�̃��X�g���N���A
	mNearInteractObjs.clear();
}


// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �����蔻��
	if (self == mpBodyCol)
	{
		if (other->Layer() == ELayer::eField)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// �Փ˂����n�ʂ������V�䂩����ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// ���ς̌��ʂ��v���X�ł���΁A���ƏՓ˂���
			if (dot >= 0.0f)
			{
				// �����Ȃǂŏ��ɏォ��Փ˂������i���ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// �ڒn����
				mIsGrounded = true;
				// �ڒn�����n�ʂ̖@�����L�����Ă���
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A�V��ƏՓ˂���
			else
			{
				// �W�����v�ȂǂœV��ɉ�����Փ˂������i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		else if (other->Layer() == ELayer::eWall)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			mIsHittingWall = true;
			mHitWallPos = Position() + adjust * hit.weight;
			mHitWallNormal = hit.adjust.forward.Normalized();

			// �����߂��x�N�g���̕����W���ړ�
			Position(Position() + adjust * hit.weight);
		}
		else if (other->Layer() == ELayer::eInteractObj)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕����W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
	// �U���p�R���C�_�[�Ƃ̓����蔻��
	else if (self == mpAttackCol1 || self == mpAttackCol2 || self == mpAttackCol3)
	{
		if (other->Tag() == ETag::eEnemy && other->Layer() == ELayer::eEnemy)
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
				float knockback = 0.0f;
				CalcDamage(chara, &damage, &stan, &knockback);
				// �_���[�W��^����
				chara->TakeDamage(damage, stan, knockback, this);
			}
		}
		//else if (other->Tag() == ETag::eInteractObject && other->Layer() == ELayer::eInteractObj)
		//{
		//	CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		//	if (obj != nullptr && !IsAttackHitObj(obj))
		//	{

		//		// �U���q�b�g�ς݃��X�g�ɓo�^
		//		AddAttackHitObj(obj);
		//		obj->Interact();
		//	}
		//}
	}
	// ���ׂ�I�u�W�F�N�g�̒T�m�R���C�_�[�Ƃ̓����蔻��
	else if (self == mpSearchCol)
	{
		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr)
		{
			// �Փ˂������ׂ�I�u�W�F�N�g�����X�g�ɒǉ�
			mNearInteractObjs.push_back(obj);

#if _DEBUG
			// �T�m�͈͓��ɓ������I�u�W�F�N�g�̖��O��\��
			CDebugPrint::Print
			(
				"%s:%s\n",
				obj->GetDebugName().c_str(),
				obj->GetInteractStr().c_str()
			);
#endif
		}
	}
}

// �`��
void CPlayer::Render()
{
	CXCharacter::Render();
}


// ���݂̏�Ԃ��擾
CPlayer::EState CPlayer::GetState()
{
	return mState;
}


CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CPlayer::ChangeAnimation(EAnimType type, bool restart)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, restart);
}

// �U��������A�U���̈З͂ɂ����
// �؂�ւ���A�j���[�V�����̎�ނ��擾
CPlayer::EAnimType CPlayer::GetAttackAnimType() const
{
	switch (mAttackDir)
	{
	case EAttackDir::eUp:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eUpAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eUpAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eUpAttackL;
		break;
	case EAttackDir::eLeft:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eLeftAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eLeftAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eLeftAttackL;
		break;
	case EAttackDir::eRight:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eRightAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eRightAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eRightAttackL;
		break;
	}
	return EAnimType::None;
}

void CPlayer::ChangeState(EState state)
{
	// ���łɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CPlayer::ChangeAvoid()
{
	// �L�[�̓��͏�񂩂�����������߂�
	mAvoidDir = CalcMoveVec(true);
	// �����Ԃ֐؂�ւ�
	ChangeState(EState::eAvoid);
}

void CPlayer::ChangeAttack()
{

	if (mState == EState::eBattleIdle)
	{
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eLeft;
		}
		else if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eRight;
		}
	}
	// ����͂ŁA��U��
	else if (IsJumping())
	{
		if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eUp;
		}
	}
	else if (mState == EState::eAttack)
	{
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eLeft;
		}
		else if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eRight;
		}
	}
}

// �_���[�W�ʂ��v�Z���ĕԂ�
void CPlayer::CalcDamage(CCharaBase* taker, int* outDamage, float* outStan, float* outKnockback) const
{
	if (mSelectAttackPower == EAttackPower::eAttackS)
	{
		// �_���[�W��^����
		*outDamage = DAMAGE_S;
		*outStan = STAN_VAL_S;
		*outKnockback = 8.0f;
	}
	else if (mSelectAttackPower == EAttackPower::eAttackM)
	{
		*outDamage = DAMAGE_M;
		*outStan = STAN_VAL_M;
		*outKnockback = 8.0f;
	}
	else if (mSelectAttackPower == EAttackPower::eAttackL)
	{
		*outDamage = DAMAGE_L;
		*outStan = STAN_VAL_L;
		*outKnockback = 8.0f;
	}

	// �p���B�o���邩�ǂ����A���f����
	if (taker->CheckAttackParry(mAttackDir, mAttackPower))
	{
		if (mSelectAttackPower == EAttackPower::eAttackS)
		{
			*outDamage = DAMAGE_S;
			// ���ݓx�����Z����
			*outStan *= STAN_VAL_DIA;
		}
		else if (mSelectAttackPower == EAttackPower::eAttackM)
		{
			*outDamage = DAMAGE_M;
			*outStan *= STAN_VAL_DIA;
		}
		else if (mSelectAttackPower == EAttackPower::eAttackL)
		{
			*outDamage = DAMAGE_L;
			*outStan *= STAN_VAL_DIA;
		}
	}
	
}

//bool CPlayer::HittingWallOfForward(float range)
//{
//	CVector forward = CVector::forward.Normalized();
//
//	// ���g�̐��ʕ����̃x�N�g���̓��ς����߂Ċp�x���o��
//	float dot = CVector::Dot(mHitWallNormal, forward);
//	// ����p�x�̃��W�A�������߂�
//	float angleR = Math::DegreeToRadian(range);
//	// ���߂����ςƎ���p�x�ŁA����͈͂����f����
//	if (dot < cosf(angleR)) return true;
//
//	return false;
//}

// ��퓬���̑ҋ@���
//void CPlayer::UpdateIdle()
//{
//	// �ڒn���Ă���΁A
//	if (mIsGrounded)
//	{
//		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
//		if (CInput::PushKey(VK_SPACE))
//		{
//			mState = EState::eJumpStart;
//		}
//	}
//}

// �퓬���̑ҋ@���
void CPlayer::UpdateBattleIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{
		if (CInput::Key(VK_SHIFT))
		{
			ChangeState(EState::eGuardStart);
		}
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		else if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
		// �V�t�g�ŉ��
		else if (CInput::Key('F'))
		{
			ChangeAvoid();
		}
		else if (CInput::Key('I'))
		{
			CInteractObject* obj = GetNearInteractObj();

			if (obj == nullptr) return;

			obj->Interact();
		}
	}
}

// �U��
void CPlayer::UpdateAttack()
{
	// �v���C���[�̍U����Ԃɍ��킹�ăA�j���[�V������؂�ւ���
	EAnimType anim = GetAttackAnimType();

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(anim, true);
		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrameRatio() >=  0.25f)
		{
			// �U���J�n
			AttackStart();
			mStateStep++;
		}
		break;
	case 2:
		if (GetAnimationFrameRatio() >= 0.75f)
		{
			ChangeAttack();
			// �U���I��
			AttackEnd();
			mStateStep++;
		}
		break;
	case 3:
		// �U���A�j���[�V�������I��������A
		if (IsAnimationFinished())
		{
			// �퓬���̑ҋ@��Ԃֈڍs
			ChangeState(EState::eBattleIdle);
		}
		break;
	}
}

void CPlayer::UpdateGuardStart()
{
	switch (mStateStep)
	{
	case 0:
		mMoveSpeed = CVector::zero;
		ChangeAnimation(EAnimType::eGuardStart);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eGuard);
		}

		if (CInput::PullKey(VK_SHIFT))
		{
			ChangeState(EState::eGuardEnd);
		}
		break;
	}
}

// �h��
void CPlayer::UpdateGuard()
{
	mMoveSpeed = CVector::zero;
	ChangeAnimation(EAnimType::eGuard);

	if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_RBUTTON))
	{
		ChangeState(EState::eGuardParry);
	}

	if (CInput::PullKey(VK_SHIFT))
	{
		ChangeState(EState::eGuardEnd);
	}
}

void CPlayer::UpdateGuardEnd()
{
	switch (mStateStep)
	{
	case 0:
		mMoveSpeed = CVector::zero;
		ChangeAnimation(EAnimType::eGuardEnd);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eBattleIdle);
		}
		break;
	}
}


void CPlayer::UpdateGuardParry()
{

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eGuardParry);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eGuard);
		}
		break;
	}
	if (CInput::PullKey(VK_SHIFT))
	{
		ChangeState(EState::eBattleIdle);
	}
}

// ���
void CPlayer::UpdateAvoid()
{
	mMoveSpeed = CVector::zero;

	switch (mStateStep)
	{
	case 0:
		mIsAvoiding = true;
		ChangeAnimation(EAnimType::eAvoid);
		mStateStep++;
		break;
	case 1:
	{
		float frame = GetAnimationFrame();
		if (frame >= EVA_MOVE_START)
		{
			if (frame < EVA_MOVE_END)
			{
				CVector current = VectorZ();
				CVector forward = CVector::Slerp(current, mAvoidDir, 0.5);
				Rotation(CQuaternion::LookRotation(forward));
				// ������̈ړ����x�����߂�
				mMoveSpeed = mAvoidDir * EVA_MOVE_DIST * Times::DeltaTime();
			}
			else
			{
				mStateStep++;
			}
		}
		break;
	}
	case 2:
		if (IsAnimationFinished())
		{
			mIsAvoiding = false;
			ChangeState(EState::eBattleIdle);
		}
		break;
	}
}

// �W�����v�J�n
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	ChangeState(EState::eJump);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// �W�����v��
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		ChangeState(EState::eJumpEnd);
	}
}

// �W�����v�I��
void CPlayer::UpdateJumpEnd()
{
	// �W�����v�A�j���[�V�������I�����A
	// �n�ʂɐڒn������A�ҋ@��Ԃ֖߂�
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState(EState::eBattleIdle);
	}
}

// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
CVector CPlayer::CalcMoveVec(bool force) const
{
	CVector move = CVector::zero;

	// �L�[�̓��̓x�N�g�����擾
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	float inputLen = input.LengthSqr();
	// �����ňړ����郂�[�h�ŁA�ǂ̃L�[�����͂���Ă��Ȃ����
	// ���ʕ����֋����ړ�����
	if (force && inputLen == 0.0f)
	{
		input.Y(-1.0f);
		inputLen = 1.0f;
	}

	// ���̓x�N�g���̒����œ��͂���Ă��邩����
	if (inputLen > 0.0f)
	{
		// ������x�N�g��(�ݒu���Ă���ꍇ�́A�n�ʂ̖@��)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
		// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// �J�����̐��ʕ����x�N�g���Ə�����x�N�g���̊O�ς���
		// �������̈ړ��x�N�g�������߂�
		CVector moveSide = CVector::Cross(up, camForward);
		// �������̈ړ��x�N�g���Ə�����x�N�g���̊O�ς���
		// ���ʕ����̈ړ��x�N�g�������߂�
		CVector moveForward = CVector::Cross(moveSide, up);

		// ���߂��e�����̈ړ��x�N�g������A
		// �ŏI�I�ȃv���C���[�̈ړ��x�N�g�������߂�
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}


// �ړ��̍X�V����
void CPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// ���̃t���[���ŉ����ꂽ�ړ��L�[���L�����Ă���
	int pushKey = -1;
	if (CInput::PushKey('W')) pushKey = 'W';
	if (CInput::PushKey('S')) pushKey = 'S';
	if (CInput::PushKey('A')) pushKey = 'A';
	if (CInput::PushKey('D')) pushKey = 'D';

	// �_�b�V���L�[���o�^����Ă��邩�ǂ���
	if (mPushRunKey != -1)
	{
		// �_�b�V�����łȂ�
		if (!mIsRun)
		{
			// �ړ��J�n���Ă���̌o�ߎ��Ԃ��v��
			mMoveElapsedTime += Times::DeltaTime();
			// �ēx�����ꂩ�̈ړ��L�[�����͂��ꂽ
			if (pushKey != -1)
			{
				// �P��ڂɉ����ꂽ�L�[�Ɠ����ꍇ�́A�_�b�V���J�n
				if (pushKey == mPushRunKey)
				{
					mIsRun = true;
				}
				// �Ⴄ�L�[�������ꍇ�́A�_�b�V���L�[�̓��͏�Ԃ�����
				else
				{
					mPushRunKey = pushKey;
					mMoveElapsedTime = 0.0f;
				}
			}
		}
	}
	// �_�b�V���L�[�����͂���Ă��Ȃ��ꍇ
	else
	{
		// ������͂��ꂽ�L�[���_�b�V���L�[�ɓo�^
		mPushRunKey = pushKey;
	}

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		if (IsJumping())
		{
			float speed = (mIsRun ? JUMP_RUN_SPEED : JUMP_WALK_SPEED);
			mMoveSpeed = move * speed;
		}
		else
		{
			float speed = (mIsRun ? RUN_SPEED : WALK_SPEED);
			mMoveSpeed = move * speed;
		}


		// �퓬�ҋ@��Ԏ��̕��s�A�j���[�V����
		if (mState == EState::eBattleIdle)
		{
			EAnimType moveAnim = EAnimType::eBattleWalk;
			if (CInput::Key('A'))
			{
				mIsRun ? moveAnim = EAnimType::eLeftRun : moveAnim = EAnimType::eBattleLeftWalk;
			}
			else if (CInput::Key('D'))
			{
				mIsRun ? moveAnim = EAnimType::eRightRun : moveAnim = EAnimType::eBattleRightWalk;
			}
			else if (CInput::Key('W'))
			{
				mIsRun ? moveAnim = EAnimType::eRun : moveAnim = EAnimType::eBattleWalk;
			}
			else if (CInput::Key('S'))
			{
				mIsRun ? moveAnim = EAnimType::eBackRun : moveAnim = EAnimType::eBattleBackWalk;
			}

			ChangeAnimation(moveAnim);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		if (mState == EState::eBattleIdle)
		{
			ChangeAnimation(EAnimType::eBattleIdle);
		}
		// �_�b�V������������A
		// �ړ��J�n����_�b�V���L�[�̓��̓C���^�[�o���𒴂����ꍇ
		if (mIsRun || mMoveElapsedTime > RUN_INPUT_INTERVAL)
		{
			mIsRun = false;
			mPushRunKey = -1;
			mMoveElapsedTime = 0.0f;
		}
	}
}

void CPlayer::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
	{
		EAnimType animType = EAnimType::eHit1;
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos - VectorZ() * mKnockBack;

		if (mRandHitAnim == 0)
		{
			animType = EAnimType::eHit1;
		}
		else if (mRandHitAnim == 1)
		{
			animType = EAnimType::eHit2;
		}
		ChangeAnimation(animType, true);
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
				if (!mIsHittingWall)
				{
					// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
					float moveFrame = moveEndFrame - moveStartFrame;
					float percent = (frame - moveStartFrame) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				else
				{
					Position(mHitWallPos);
					mStateStep++;
				}
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
			ChangeState(EState::eBattleIdle);
		}
		break;
	}
}

void CPlayer::UpdateDeath()
{
	if (mRandDeathAnim < DEATH_RPOB)
	{
		ChangeAnimation(EAnimType::eDeath1);
		mStateStep++;
	}
	else
	{
		ChangeAnimation(EAnimType::eDeath2);
		mStateStep++;
	}
}


std::string CPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eBattleIdle:	return "�ҋ@(�퓬)";
	case EState::eAttack:		return "�U��";
	case EState::eGuard:		return "�h��";
	case EState::eGuardParry:	return "�h��p���B";
	case EState::eAvoid:		return "���";
	case EState::eHit:			return "������";
	case EState::eStun:			return "�C��";
	case EState::eDeath:		return "���S";
	}
	return "";
}

CInteractObject* CPlayer::GetNearInteractObj() const
{
	// ��ԋ߂��̒��ׂ�I�u�W�F�N�g�̃|�C���g
	CInteractObject* nearObj = nullptr;
	float nearDist = 0.0f;	// ���݈�ԋ߂��ɂ��钲�ׂI�u�W�F�N�g�܂ł̋���
	CVector pos = Position();
	// �T�m�͈͓��̒��ׂ�I�u�W�F�N�g�����Ԃɒ��ׂ�
	for (CInteractObject* obj : mNearInteractObjs)
	{
		// ���ݒ��ׂ��Ȃ���Ԃł���΁A�X���[
		if (!obj->CanInteract()) continue;

		float dist = (obj->Position() - pos).LengthSqr();
		// ��ԍŏ��̒��ׂ�I�u�W�F�N�g���A
		// ���߂����������݂̈�ԋ߂��I�u�W�F�N�g�����߂��ꍇ�́A
		if (nearObj == nullptr || dist < nearDist)
		{
			// ��ԋ߂��I�u�W�F�N�g���X�V
			nearObj = obj;
			nearDist = dist;
		}
	}
	return nearObj;
}
