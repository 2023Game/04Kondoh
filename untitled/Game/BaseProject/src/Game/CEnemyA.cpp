#include "CEnemyA.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CDebugFieldOfView.h"
#include "CPlayer.h"
#include "Maths.h"
#include "Primitive.h"

#define FOV_ANGLE 45.0f         // ����͈͂̊p�x
#define FOV_LENGTH 100.0f       // ����͈͂̋���
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
#define IDLE_TIME 5.0f          // �ҋ@��Ԃ̎���

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemyA::AnimData CEnemyA::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	{ "Character\\EnemyA\\Idle.x",		        true,	126.0f	},	// �ҋ@
	{ "Character\\EnemyA\\walk.x",		        true,	42.0f	},	// ���s
	{ "Character\\EnemyA\\Run.x",		        true,	23.0f	},	// ����
	{ "Character\\EnemyA\\RightAttackS.x",      false,   58.0f   },  // �E�U��

};


// �R���X�g���N�^
CEnemyA::CEnemyA(std::vector<CVector> patrolPoints)
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mLostPlayerPos(CVector::zero)
	, mAttackStartPos(CVector::zero)
	, mAttackEndPos(CVector::zero)
	, mPatrolPoints(patrolPoints)
	, mNextPatrolIndex(-1)
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

	// ����͈͂̃f�o�b�O�\�����쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
}

CEnemyA::~CEnemyA()
{
	// ����͈͂̃f�o�b�O�\���������݂�����A�ꏏ�ɍ폜����
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
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
		ChangeState(EState::eChase);
		//ChangeAnimation(EAnimType::eWalk);
	}
	else if (CInput::Key('L')) {
		ChangeState(EState::ePatrol);
		//ChangeAnimation(EAnimType::eRun);
	}

	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ���
	switch (mState)
	{
	case EState::eIdle:    UpdateIdle();   break;
	case EState::ePatrol:  UpdatePatrol(); break;
	case EState::eChase:   UpdateChase();  break;
	case EState::eLost:    UpdateLost();   break;
	case EState::eAttack:  UpdateAttack(); break;
	}


	CXCharacter::Update(); 

	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("��� : %s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("�X�e�b�v : %d\n", mStateStep);
}

void CEnemyA::Render()
{
	CXCharacter::Render();

	// ���������Ƃ�
	if (mState == EState::eLost)
	{
		//�v���C���[�����������ʒu�Ƀf�o�b�O�\��
		float rad = 2.0f;
		CMatrix m;
		m.Translate(mLostPlayerPos + CVector(0.0f, rad, 0.0f));
		Primitive::DrawWireSphere(m, rad, CColor::blue);
	}
}

void CEnemyA::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return; 
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.frameLength);
}

//��Ԑ؂�ւ�
void CEnemyA::ChangeState(EState state)
{
	// ���łɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
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


	// TODO:�v���C���[�Ƃ̊ԂɎՕ������Ȃ������肷��


	// �S�Ă̏������N���A�����̂ŁA����͈͓��ł���
	return true;
}

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

// �U�����Ɉړ����鋗����
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

// ���ɏ��񂷂�|�C���g��ύX
void CEnemyA::ChangePatrolPoint()
{
	// ����|�C���g���ݒ肳��Ă��Ȃ��ꍇ�́A�������Ȃ�
	int size = mPatrolPoint.size();
	if (size == 0) return;

	// ����J�n���ł���΁A��ԋ߂�����|�C���g��I��
	if (mNextPatrolPoint == -1)
	{
		int nearIndex = -1;     // ��ԋ߂�����|�C���g�̔ԍ�
		float nearDist = 0.0f;  // ��ԋ߂�����|�C���g�܂ł̋���
		// �S�Ă̏���|�C���g�𒲂ׁA��ԋ߂�����|�C���g��T��
		for (int i = 0; i < size; i++)
		{
			CVector point = mPatrolPoints[i];
			CVector vec = point - Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// ��ԋ߂�����|�C���g�������́A
			// ���݈�ԋ߂�����|�C���g��肳��ɋ߂��ꍇ�́A
			// ����|�C���g�̔ԍ���u��������
			if (nearIndex < 0 || dist < nearDist)
			{
				nearIndex = i;
				nearDist = dest;
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

}

// �ҋ@��Ԃ̍X�V����
void CEnemyA::UpdateIdle()
{
	// �v���C���[������͈͓��ɓ�������A�ǐՏ�Ԃֈڍs
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// �ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// �ҋ@���Ԃ��o�߂�����A�����Ԃֈڍs
		ChangeState(EState::ePatrol);
	}
}

// ���񒆂̍X�V����
void CEnemyA::UpdatePatrol()
{
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}
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
		if (MoveTo(mPatrolPoints[mNextPatrolIndex], WALK_SPEED))
		{
			mStateStep++;
		}
		break;
		// �X�e�b�v2�F�ړ���̑ҋ@
	case 2:
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangePatrolPoint();
			mStateStep = 1;
		}
		break;
	}
}

// �ǐՒ��̍X�V����
void CEnemyA::UpdateChase()
{
	// �v���C���[������͈͊O�ɏo����A����������Ԃɖ߂�
	if (!IsFoundPlayer())
	{
		ChangeState(EState::eLost);
		return;
	}
	// �v���C���[�ɍU���ł���Ȃ�΁A�U����Ԃֈڍs
	if (CanAttackPlayer())
	{
		ChangeState(EState::eAttack);
		return;
	}

	ChangeAnimation(EAnimType::eRun);

	// �v���C���[�̍��W�֌����Ĉړ�����
	CPlayer* player = CPlayer::Instance();
	CVector playerPos = player->Position();
	mLostPlayerPos = playerPos; // �v���C���[���Ō�Ɍ������W���X�V
	if (MoveTo(playerPos, RUN_SPEED))
	{

	}
}

// �����������̍X�V����
void CEnemyA::UpdateLost()
{
	// �v���C���[������͈͓��ɓ�������A�ǐՏ�Ԃֈڍs
	if (IsFoundPlayer()) 
	{
		ChangeState(EState::eChase);
		return;
	}

	// �����A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eRun);
	// �v���C���[�����������ʒu�܂ňړ�
	if (MoveTo(mLostPlayerPos, RUN_SPEED))
	{
		// �ړ����I���΁A�ҋ@��Ԃֈړ�
		ChangeState(EState::eIdle);
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
			ChangeAnimation(EAnimType::eRightAttack);
			mStateStep++;
			break;
		// �X�e�b�v1 : �U�����̈ړ�����
		case 1:
		{
			// �U���A�j���[�V�������ړ��J�n�t���[���𒴂����ꍇ
			float frame = GetAnimationFrame();
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
				ChangeState(EState::eIdle);
			}
			break;
	}
	
}

// ��Ԃ̕�������擾
std::string CEnemyA::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:    return "�ҋ@";
	case EState::ePatrol:  return "����";
	case EState::eChase:   return "�ǐ�";
	case EState::eLost:    return "������";
	case EState::eAttack:  return "�U��";
	}
	return "";
}

CColor CEnemyA::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:    return CColor::white;
	case EState::ePatrol:  return CColor::green;
	case EState::eChase:   return CColor::red;
	case EState::eLost:    return CColor::yellow;
	case EState::eAttack:  return CColor::magenta;
	}
	return CColor::white;
}
