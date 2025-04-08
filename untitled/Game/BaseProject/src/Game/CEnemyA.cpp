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
#define RUN_SPEED 20.0f         // �����Ă��鎞�̑��x
#define ROTATE_SPEED 6.0f       // ��]���x

#define ATTACK_RANGE 20.0f      // �U���͈�
#define ATTACK_RANGE_MIN 12.0f  // �ŏ��U���͈�
#define ATTACK_MOVE_DIST 8.0f   // �U�����̈ړ�����
#define ATTACK_MOVE_START 12.0f // �U�����̈ړ��J�n�t���[��
#define ATTACK_MOVE_END 24.0f   // �U�����̈ړ��I���t���[��
#define ATTACK_WAIT_TIME 1.0f   // �U���I�����̑҂�����

#define PATROL_INTERVAL 3.0f    // ���̏���Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST 10.0f  // ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME 5.0f          // �ҋ@��Ԃ̎���
#define DEATH_WAIT_TIME 3.0f	// ���S���̑ҋ@����
#define LOOKAT_SPEED 2.0f		// �Ώۂ̕����Ɍ������x

#define DETECT_COL_POS CVector(0.0f, 5.0f, 3.0f)	// ����p�R���C�_�̍��W
#define DETECT_COL_SCL CVector(0.7f, 1.5f, 0.7f)	// ����p�R���C�_�̑傫��

#define EATTACKWAY CPlayer::EAttackDir		// �v���C���[�̍U������
#define EATTACKPWOER CPlayer::EAttackPower	// �v���C���[�̍U���З�

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",									true,	0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"Idle.x",					true,	126.0f,	1.0f},	// �ҋ@
	{ ANIM_PATH"walk.x",					true,	42.0f,	1.0f},	// ���s
	{ ANIM_PATH"Run.x",						true,	23.0f,	1.0f},	// ����
	{ ANIM_PATH"Death.x",					false,	129.0f,	1.0f},	// ���S
	{ ANIM_PATH"Stan.x",					false,	129.0f,	1.0f},	// �C��A�j���[�V����

	{ ATTACK_ANIM_PATH"LRLAttack.x",		false,	98.0f,	1.0f},  // �E��U��
	{ ATTACK_ANIM_PATH"RightAttackM.x",		false,	93.0f,	0.5f},  // �E���U��
	{ ATTACK_ANIM_PATH"LeftAttackS.x",		false,	34.0f,	1.0f},  // ����U��
	{ ATTACK_ANIM_PATH"LeftAttackM.x",		false,	53.0f,	1.0f},  // �����U��
	{ ATTACK_ANIM_PATH"UpAttackS.x",		false,	69.0f,	1.0f},  // ���U��
	{ ATTACK_ANIM_PATH"UpAttackM.x",		false,	81.0f,	2.0f},  // �㒆�U��
	{ ATTACK_ANIM_PATH"DownAttackS.x",		false,	69.0f,	1.0f},  // ����U��
	{ ATTACK_ANIM_PATH"DownAttackM.x",		false,	73.0f,	1.0f},  // �����U��

	{ DAMAGEHIT_PATH"Damage1.x",				true,	42.0f,	1.0f},	// ������1
	{ DAMAGEHIT_PATH"Damage2.x",				true,	42.0f,	1.0f},	// ������2
	{ DAMAGEHIT_PATH"Damage3.x",				true,	42.0f,	1.0f},	// ������3
	{ CHANCEHIT_PATH"chancehit1.x",				true,	23.0f,	1.0f},	// �`�����X��1
	{ CHANCEHIT_PATH"chancehit2.x",				true,	23.0f,	1.0f},	// �`�����X��2
	{ CHANCEHIT_PATH"chancehit3.x",				true,	23.0f,	1.0f},	// �`�����X��3
	{ CHANCEHIT_PATH"chancehit4.x",				true,	23.0f,	1.0f},	// �`�����X��4

};

const std::vector<CEnemyBase::AttackData> ATTACK_DATA =
{
	{ EAttackDir::eRight,	EAttackPower::eAttackS,		true, 0.0f, 50.0f},
	{ EAttackDir::eLeft,	EAttackPower::eAttackS,		true, 0.0f, 20.0f},
	{ EAttackDir::eUp,		EAttackPower::eAttackM,		true, 0.0f, 49.0f},
	{ EAttackDir::eDown,	EAttackPower::eAttackS,		true, 0.0f, 37.0f},
};

// �R���X�g���N�^
CEnemyA::CEnemyA(std::vector<CVector> patrolPoints)
	: mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mAttackStartPos(CVector::zero)
	, mAttackEndPos(CVector::zero)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
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


	// ����͈͂̃f�o�b�O�\�����쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

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
	case (int)EState::eIdle:		UpdateIdle();	break;
	case (int)EState::ePatrol:		UpdatePatrol();	break;
	case (int)EState::eChase:		UpdateChase();	break;
	case (int)EState::eLost:		UpdateLost();	break;
	case (int)EState::eAttack:		UpdateAttack();	break;
	case (int)EState::eStan:		UpdateStan();	break;
	case (int)EState::eDeath:		UpdateDeath();	break;
//	case (int)EState::eGuardParry:	UpdateGuardParry();
//	case (int)EState::eAttackParry:	UpdateStan();	break;
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
	//CDebugPrint::Print("�X�e�b�v : %d\n", mStateStep);
	//CDebugPrint::Print("�X�^�� : %s\n", IsParry() ? "�p���B�����I" : "�c�O�����I");
	CDebugPrint::Print("�@�U���̋����F%s\n", GetAttackPowerStr().c_str());
	CDebugPrint::Print("�@�U���̕����F%s\n", GetAttackDirStr().c_str());
	CDebugPrint::Print("�@�t���[���F%.2f\n", GetAnimationFrame());
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
	if (mAttackType == (int)EAttackType::eRightAttack)
	{
		mpRAttackCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eLeftAttack)
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

	if (IsDeath())
	{
		// �U���������������퓬����ɐݒ�
		mpBattleTarget = causer;
		// �U��������������̕����֌���
		LookAtBattleTarget(true);
		// �퓬��Ԃ�؂�ւ���
		mIsBattle = true;
		// �ړ����~
		mMoveSpeed = CVector::zero;
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

	if (self == mpLAttackCol)
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
				// �_���[�W��^����
				chara->TakeDamage(3, 0.0f, this);

			}
		}

	}
	else if (self == mpRAttackCol)
	{
		// �q�b�g�����̂��L�����N�^�[���A
		// �܂��U�����q�b�g���Ă��Ȃ��L�����N�^�[�ł����
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr && !IsAttackHitObj(chara))
		{
			// �_���[�W��^����
			chara->TakeDamage(3, 0.0f,this);
			// �U���q�b�g�ς݃��X�g�ɓo�^
			AddAttackHitObj(chara);
		}
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
bool CEnemyA::CanAttackPlayer() const
{
	// �v���C���[�����Ȃ��ꍇ�́A�U���ł��Ȃ�
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();
	if (dist > ATTACK_RANGE) return false;

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
	if (dist > ATTACK_RANGE_MIN) return false;

	// �S�Ă̏������݂�����
	return true;
	return false;
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
	// �v���C���[������͈͓��ɓ�������A�ǐՏ�Ԃֈڍs
	if (IsFoundPlayer())
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// �ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	if (mElapsedTime < IDLE_TIME)
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
			ChangePatrolPoint();
			mStateStep = 1;
			mElapsedTime = 0.0f;
		}
		break;
	}
}

// �ǐՒ��̍X�V����
void CEnemyA::UpdateChase()
{
	// �v���C���[�̍��W�֌����Ĉړ�����
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();

	// �v���C���[�������Ȃ��Ȃ�����A����������Ԃɖ߂�
	if (!IsLookPlayer())
	{
		mpLostPlayerNode->SetPos(targetPos);
		ChangeState((int)EState::eLost);
		return;
	}
	// �v���C���[�ɍU���ł���Ȃ�΁A�U����Ԃֈڍs
	if (CanAttackPlayer())
	{
		// �E�U���ɕύX�iTODO�F�U���̎�ނ�؂�ւ���j
		ChangeAttackType((int)EAttackType::eRightAttack);
		ChangeState((int)EState::eAttack);
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
}

// �����������̍X�V����
void CEnemyA::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
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
				// �ړ����I���΁A�ҋ@��Ԃֈڍs
				ChangeState((int)EState::eIdle);
			}
		}
		break;
	}
}

// �U�����̍X�V����
void CEnemyA::UpdateAttack()
{
	switch (mStateStep)
	{
		// �X�e�b�v0 : �U���A�j���[�V����
	    case 0:
			// �U���J�n�ʒu�ƍU���I���ʒu�̐ݒ�
			mAttackStartPos = Position();
			mAttackEndPos = mAttackStartPos + VectorZ() * ATTACK_MOVE_DIST;
			ChangeAnimation((int)EAnimType::eRightAttackS,true);
			mStateStep++;
			break;
		// �X�e�b�v1 : �U�����̈ړ�����
		case 1:
		{
			// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
			float frame = GetAnimationFrame();
			AttackStart();
			if (!AttackRangeMin()) {
				if (frame >= ATTACK_MOVE_START)
				{

					// �ړ��I���t���[���܂œ��B���ĂȂ��ꍇ
					if (frame < ATTACK_MOVE_END)
					{
						// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
						float moveFrame = ATTACK_MOVE_END - ATTACK_MOVE_START;
						float percent = (frame - ATTACK_MOVE_START) / moveFrame;
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
		// �X�e�b�v2 : �U���A�j���[�V�����̏I���҂�
		case 2:
			if (IsAnimationFinished())
			{
				AttackEnd();
				mStateStep++;
			}
			break;
		// �X�e�b�v3 : �U���I�����̑҂�����
		case 3:
			if (mElapsedTime < ATTACK_WAIT_TIME)
			{
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				// ���Ԃ��o�߂�����A�ҋ@��Ԃֈڍs
				ChangeState((int)EState::eIdle);
			}
			break;
	}
	
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
			ChangeState((int)EState::eIdle);
		}
		break;
	}
}

//void CEnemyA::UpdateChance()
//{
//	// TODO�F�m�b�N�o�b�N����悤�ɂȂ�
//	// �U���ɔ{�������
//}




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
