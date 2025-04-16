//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CEnemyA.h"
#include "CEnemyManager.h"
#include "CInput.h"
#include "CCamera.h"
#include "CGameCamera2.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CField.h"
#include "CDebugFieldOfView.h"

#include "CEnemyManager.h"
#include "CEnemyA.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;



#define PLAYER_CAP_UP    13.5f     // �v���C���[�̍���
#define PLAYER_CAP_DWON   2.5f     // �v���C���[�̒�
#define PLAYER_WIDTH      2.0f     // �v���C���[�̕�

#define ATTACK1_CAP_UP		80.0f	// �U���R���C�_�[1�̏�
#define ATTACK1_CAP_DWON	0.0f	// �U���R���C�_�[1�̉�
#define ATTACK2_CAP_UP		0.0f	// �U���R���C�_�[2�̏�
#define ATTACK2_CAP_DWON	-30.0f	// �U���R���C�_�[2�̉�
#define ATTACK_START_FRAME	24.0f	// �U���J�n�t���[��
#define ATTACK_END_FRAME	50.0f	// �U���I���t���[��
#define ATTACK_LENGTH		10.0f	// �U���͈͂̋���
#define ATTACK_ANGLE		30.0f	// �U���͈͂̊p�x

#define MOVE_SPEED 0.375f * 2.0f
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f  
#define EVA_MOVE_SPEED 200.0f  // ������̈ړ����x
#define EVA_MOVE_START   8.0f  // ������̈ړ��J�n�t���[�� 
#define EVA_MOVE_END    24.0f  // ������̈ړ��I���t���[��
#define EVA_WAIT_TIME    0.1f  // ����I�����̑ҋ@����

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

#define PLAYER_ANIM_PATH "Character\\New Player\\anim\\"
// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",								true,	0.0f,	1.0f	},	// �퓬����T�|�[�Y
	{ PLAYER_ANIM_PATH"AttackIdle.x",	true,	170.0f,	1.0f	},	// �퓬���̑ҋ@

	{ PLAYER_ANIM_PATH"AttackWalk.x",	true,	34.0f,	10.0f	},	// ���s
	{ PLAYER_ANIM_PATH"BackWalk.x",		true,	39.0f,	1.0f	},	// ���������s
	{ PLAYER_ANIM_PATH"LeftWalk.x",		true,	40.0f,	1.0f	},	// ���������s
	{ PLAYER_ANIM_PATH"RightWalk.x",	true,	35.0f,	1.0f	},	// �E�������s

	{ PLAYER_ANIM_PATH"UpAttackS.x",	false,	54.0f,	1.0f	},	// ���U��
	{ PLAYER_ANIM_PATH"UpAttackM.x",	false,	69.0f,	0.1f	},	// ����U��
	{ PLAYER_ANIM_PATH"UpAttackL.x",	false,	92.0f,	1.0f	},	// ����U��

	{ PLAYER_ANIM_PATH"DwonAttackS.x",	false,	50.0f,	1.0f	},	// �㉺�U��
	{ PLAYER_ANIM_PATH"DwonAttackM.x",	false,	55.0f,	1.0f	},	// �����U��
	{ PLAYER_ANIM_PATH"DwonAttackL.x",	false,	90.0f,	1.0f	},	// �����U��

	{ PLAYER_ANIM_PATH"RightAttackS.x",	false,	60.0f,	1.0f	},	// ��E�U��
	{ PLAYER_ANIM_PATH"RightAttackM.x",	false,	65.0f,	1.5f	},	// ���E�U��
	{ PLAYER_ANIM_PATH"RightAttackL.x",	false,	86.0f,	1.0f	},	// ���E�U��

	{ PLAYER_ANIM_PATH"LeftAttackS.x",	false,	50.0f,	1.0f	},	// �㍶�U��
	{ PLAYER_ANIM_PATH"LeftAttackM.x",	false,	60.0f,	1.0f	},	// �����U��
	{ PLAYER_ANIM_PATH"LeftAttackL.x",	false,	99.0f,	1.0f	},	// �����U��

	{ PLAYER_ANIM_PATH"Defense.x",		true,	43.0f,	1.0f	},  // �h��
	{ PLAYER_ANIM_PATH"Evasion.x",		false,	50.0f,	1.0f	},  // ���

	{ PLAYER_ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// �W�����v�J�n
	{ PLAYER_ANIM_PATH"jump.x",			true,	1.0f,	1.0f	},	// �W�����v��
	{ PLAYER_ANIM_PATH"jump_end.x",		false,	26.0f,	1.0f	},	// �W�����v�I��
	{ PLAYER_ANIM_PATH"jump_Attack.x",	false,	47.0f,	1.0f	},	// �W�����v�U��
};


// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mSelectAttackPower(EAttackPower::eAttackM)
	, mMode(EMode::eBattle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mpRideObject(nullptr)
	, mpLockOnTarget(nullptr)
	, mIsGrounded(false)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mIsLockOn(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

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
	ChangeAnimation(EAnimType::eAttackTPose);

	// �J�v�Z���R���C�_�[�쐬
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, PLAYER_CAP_DWON, 0.0f),
		CVector(0.0f, PLAYER_CAP_UP, 0.0f),
		PLAYER_WIDTH, true
	);
	mpBodyCol->SetCollisionTags({ ETag::eEnemy, ETag::eField, ETag::eWall});
	mpBodyCol->SetCollisionLayers
	({ ELayer::eField, ELayer::eWall,ELayer::eAttackCol,ELayer::eEnemy });


	// �U���p�̃R���C�_�P�i���̐n�̕����j
	mpAttackCol1 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK1_CAP_DWON),
		CVector(0.0f, 0.0f, ATTACK1_CAP_UP),
		0.7f, true
	);
	mpAttackCol1->SetCollisionTags({ ETag::eEnemy });
	mpAttackCol1->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall});
	mpAttackCol1->Rotate(CVector(-4.5f, 14.1f, 0.0f));
	mpAttackCol1->SetEnable(false);

	// �U���p�̃R���C�_�Q�i���̎�����̕����j
	mpAttackCol2 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK2_CAP_UP),
		CVector(0.0f, 0.0f, ATTACK2_CAP_DWON),
		0.8, true
	);
	mpAttackCol2->SetCollisionTags({ ETag::eEnemy });
	mpAttackCol2->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall});
	mpAttackCol2->Rotate(CVector(-4.5f, 14.1f, 0.0f));
	mpAttackCol2->SetEnable(false);

	// �U���p�̃R���C�_�R�i���̕����j
	mpAttackCol3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		15.0, true
	);
	mpAttackCol3->SetCollisionTags({ ETag::eEnemy });
	mpAttackCol3->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall});
	mpAttackCol3->Translate(0.0f, 0.0f, -12.0f);
	mpAttackCol3->SetEnable(false);

	// ����͈͂̃f�o�b�O�\�����쐬
	mpDebugAttack = new CDebugFieldOfView(this, mAttackAngle, mAttackLength);

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


}

CPlayer::~CPlayer()
{
	// �R���C�_�[��j��
	SAFE_DELETE(mpBodyCol);

	// �o�H�T���p�̃m�[�h��j��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
	}

	spInstance = nullptr;
}


bool CPlayer::IsAttacking() const
{
	return mState == EState::eAttack
		|| mState == EState::eAttackWait;
}

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

void CPlayer::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CXCharacter::AttackEnd();

	// �U���R���C�_�[���I�t
	mpAttackCol1->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
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
	case EAttackDir::eDown:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eDwonAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eDwonAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eDwonAttackL;
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

void CPlayer::ChangeEvasion()
{
	// �L�[�̓��͏�񂩂�����������߂�
	mEvaDir = CalcMoveVec(true);
	// �����Ԃ֐؂�ւ�
	ChangeState(EState::eEvasion);
}

void CPlayer::ChangeAttack()
{
	
	if (mState == EState::eIdle)
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
	else if (mState == EState::eJump
		|| mState == EState::eJumpStart
		|| mState == EState::eJumpEnd)
	{
		if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eUp;
		}
	}
	
	// �����͂ŁA���U��
	else if (mState == EState::eDefense)
	{
		if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eDown;
		}
	}
}


//void CPlayer::LockOnTarget()
//{
//	// ���b�N�I���J�n
//	if (mIsLockOn)
//	{
//		// ���b�N�I������G���擾
//		CEnemyBase* target = CEnemyManager::Instance()->FindLockOnTarget(45, 300);
//		mpLockOnTarget = target;
//		// ���b�N�I������G�����݂���
//		if (mpLockOnTarget != nullptr )
//		{
//				if (CInput::PushKey('B'))
//				{
//					// TODO:���b�N�I������G��ύX
//				}
//				// ���b�N�I������
//				target->Position();
//		}
//		// ���b�N�I������G�����݂��Ȃ�������A
//		// �o�g�����[�h�t���O���I�t�ɂ���
//		else
//		{
//			mIsLockOn = false;
//		}
//	}
//	else
//	{
//		mpLockOnTarget = nullptr;
//	}
//}


// �_���[�W�ʂ��v�Z���ĕԂ�
void CPlayer::CalcDamage(CCharaBase* taker, int* outDamage, float* outStan) const
{
	
	if (mSelectAttackPower == EAttackPower::eAttackS)
	{
		// �_���[�W��^����
		*outDamage = 1;
	}
	else if (mSelectAttackPower == EAttackPower::eAttackM)
	{
		*outDamage = 2;
	}
	else if (mSelectAttackPower == EAttackPower::eAttackL)
	{
		*outDamage = 3;
	}

	// �p���B�o���邩�ǂ����A���f����
	if (taker->CheckParry(mAttackDir, mAttackPower))
	{
		if (mSelectAttackPower == EAttackPower::eAttackS)
		{
			*outDamage *= 2;
			// ���ݓx�����Z����
			*outStan = 10.0f;
		}
		else if (mSelectAttackPower == EAttackPower::eAttackM)
		{
			*outDamage *= 2;
			*outStan = 50.0f;
		}
		else if (mSelectAttackPower == EAttackPower::eAttackL)
		{
			*outDamage *= 2;
			*outStan = 100.0f;
		}
	}
	
}

// ��퓬���̑ҋ@���
void CPlayer::UpdateIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
}

// �퓬���̑ҋ@���
void CPlayer::UpdateAttackIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{
		////���N���b�N�ōU��
		//if (CInput::PushKey(VK_LBUTTON))
		//{
		//	mMoveSpeed = CVector::zero;
		//	ChangeState(EState::eAttack);
		//	mAttackPower = mSelectAttackPower;
		//	mAttackDir = EAttackDir::eLeft;
		//	ChangeAttack();
		//}
		//else if (CInput::PushKey(VK_RBUTTON))
		//{
		//	mMoveSpeed = CVector::zero;
		//	ChangeState(EState::eAttack);
		//	mAttackPower = mSelectAttackPower;
		//	mAttackDir = EAttackDir::eRight;
		//}
		if (CInput::Key(VK_SHIFT))
		{
			ChangeState(EState::eDefense);
		}
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		else if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
		// �V�t�g�ŉ��
		else if (CInput::Key('F'))
		{
			ChangeEvasion();
		}
		// �܂��g���Ȃ�
		//else if (CInput::PushKey('R'))
		//{
		//	mIsLockOn = !mIsLockOn;
		//	LockOnTarget();
		//}
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
			// �U���I��
			AttackEnd();
			mStateStep++;
		}
		break;
	case 3:
		// �U���A�j���[�V�������I��������A
		if (IsAnimationFinished())
		{
			// �ҋ@��Ԃֈڍs
			ChangeState(EState::eIdle);
			ChangeAnimation(EAnimType::eAttackIdle);
		}
		break;
	}
}

// �U���I���҂�
//void CPlayer::UpdateAttackWait()
//{
//	// �a��SE���Đ����Ă��Ȃ����A�A�j���[�V������25%�ȏ�i�s������A
//	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
//	{
//		// �a��SE���Đ�
//		mpSlashSE->Play();
//		mIsPlayedSlashSE = true;
//	}
//
//	// �a���G�t�F�N�g�𐶐����Ă��Ȃ����A�A�j���[�V������35%�ȏ�i�s������A
//	if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.35f)
//	{
//		// �a���G�t�F�N�g�𐶐����āA���ʕ����֔�΂�
//		CSlash* slash = new CSlash
//		(
//			this,
//			Position() + CVector(0.0f, 10.0f, 0.0f),
//			VectorZ(),
//			300.0f,
//			100.0f
//		);
//		// �a���G�t�F�N�g�̐F�ݒ�
//		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));
//
//		mIsSpawnedSlashEffect = true;
//	}
//
//	// �U���A�j���[�V�������I��������A
//	if (IsAnimationFinished())
//	{
//		// �ҋ@��Ԃֈڍs
//		mState = EState::eIdle;
//		mAttackWay = EAttackWay::eIdle;
//		ChangeAnimation(EAnimType::eAttackIdle);
//	}
//}

// ���
void CPlayer::UpdateEvasion()
{
	mMoveSpeed = CVector::zero;

	switch (mStateStep)
	{
	case 0:
		
		ChangeAnimation(EAnimType::eEvasion);
		mStateStep++;
		break;
	case 1:
	{
		float frame = GetAnimationFrame();
		if (frame >= EVA_MOVE_START)
		{
			if (frame < EVA_MOVE_END)
			{
				// ������̈ړ����x�����߂�
				mMoveSpeed = mEvaDir * EVA_MOVE_SPEED * Times::DeltaTime();
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
			mStateStep++;
		}
		break;
	case 3:
		if (mElapsedTime < EVA_WAIT_TIME)
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

// �h��
void CPlayer::UpdateDefense()
{
	mMoveSpeed = CVector::zero;
	ChangeAnimation(EAnimType::eDefense);
	if (CInput::PullKey(VK_RBUTTON))
	{
		mState = EState::eIdle;
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
		ChangeState(EState::eIdle);
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

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
			mMoveSpeed += move * MOVE_SPEED;

		// �ҋ@��Ԃł���΁A���s�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle && mMode == EMode::eBattle)
		{
			if (CInput::Key('W'))
			{
				ChangeAnimation(EAnimType::eAttackWalk);
			}
			else if (CInput::Key('S'))
			{
				ChangeAnimation(EAnimType::eAttackBackWalk);
			}
			else if (CInput::Key('A'))
			{
				ChangeAnimation(EAnimType::eAttackLeftWalk);
			}
			else if (CInput::Key('D'))
			{
				ChangeAnimation(EAnimType::eAttackRightWalk);
			}
		}
		else if (mState == EState::eIdle && mMode == EMode::eNotBattle)
		{
			ChangeAnimation(EAnimType::eAttackWalk);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eAttackIdle);
		}
	}
}

bool CPlayer::IsAttackRange() const
{
	// �G�����݂��Ȃ��ꍇ�́A�͈͊O�Ƃ���
	CEnemyManager* enemy = CEnemyManager::Instance();
	if (enemy == nullptr) return false;

	// �v���C���[���W�̎擾
	CVector enemyPos = enemy->Position();
	// �������g�̍��W���擾
	CVector pos = Position();
	// ���g����v���C���[�܂ł̃x�N�g�������߂�
	CVector vec = pos - playerPos;
	vec.Y(0.0f);

	// 1: �U���͈͂̊p�x�������߂�
	// �x�N�g���𐳋K�����ĕ����v�f�݂̂ɂ��邽��
	// �������P�ɂ���
	CVector dir = pos.Normalized();
	// ���g�̐��ʕ����̃x�N�g�����擾
	CVector forward = VectorZ();
	// �v���C���[�܂ł̃x�N�g����
	// ���g�̐��ʕ����̃x�N�g���̓��ς����߂Ċp�x���o��
	float dot = CVector::Dot(dir, forward);
	// ����p�x�̃��W�A�������߂�
	float angleR = Math::DegreeToRadian(mAttackAngle);
	// ���߂����ςƎ���p�x�ŁA����͈͂����f����
	if (dot < cosf(angleR)) return false;

	// 2: ���싗���������߂�
	// �I���܂ł̋����ƍU���͈͂̋����ŁA�U���͈͓������f����
	float dist = Length();
	if (dist > mAttackLength) return false;

	return true;
}

// ���|�[�V�����u���[�̍X�V�����A�g���ĂȂ�
/*
// ���[�V�����u���[�̍X�V����
void CPlayer::UpdateMotionBlur()
{
	// ���[�V�����u���[�̎c�莞�Ԃ��c���Ă��Ȃ���΁A�������Ȃ�
	if (mMotionBlurRemainTime <= 0.0f) return;
	// ���݂̃J�������擾���A���݂��Ȃ���Ώ������Ȃ�
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// �J�����̌����Ɣ��Ε����փu���[���|���邽�߁A
	// ���]�����J�����̌������擾
	CVector camDir = -currentCamera->VectorZ();

	// �c�莞�Ԃ���o�ߎ��Ԃ̊������擾�i�o�ߎ��Ԃ̊��� = 1 - �c�莞�Ԃ̊����j
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// �u���[�̕����T�C���J�[�u�Ōo�ߎ��Ԃɍ��킹�đ���������
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// ���[�V�����u���[�̃p�����[�^��ݒ�
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// �c�莞�Ԃ��o�ߎ��ԕ�����������
	mMotionBlurRemainTime -= Times::DeltaTime();
	// �c�莞�Ԃ��Ȃ��Ȃ�΁A
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// ���[�V�����u���[���I�t�ɂ���
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
}
*/

// �X�V
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (CInput::PushKey('T'))
	{
		mIsBattleMode = !mIsBattleMode;
	}
	// �o�g�����[�h���ǂ���
	if (mIsBattleMode)
	{
		mMode = EMode::eBattle;
	}
	else
	{
		mMode = EMode::eNotBattle;
	}

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
		// �ҋ@���
	case EState::eIdle:
		if (mMode == EMode::eBattle)
		{
			UpdateAttackIdle();
		}
		else
		{
			UpdateIdle();
		}
		break;
		// �U��
	case EState::eAttack:
		UpdateAttack();
		break;
		// �U���I���҂�
	//case EState::eAttackWait:
	//	UpdateAttackWait();
		break;
		// �h��
	case EState::eDefense:
		UpdateDefense();
		break;
		// ���
	case EState::eEvasion:
		UpdateEvasion();
		break;
		// �W�����v�J�n
	case EState::eJumpStart:
		UpdateJumpStart();
		break;
		// �W�����v��
	case EState::eJump:
		UpdateJump();
		break;
		// �W�����v�I��
	case EState::eJumpEnd:
		UpdateJumpEnd();
		break;
	}

	// �ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd
		|| mState == EState::eDefense)
	{
		ChangeAttack();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// �ړ�
	Position(Position() + moveSpeed);

	// ���b�N�I���ƈړ��̎d���ƃo�g�����[�h
	if (!(mMode == EMode::eBattle))
	{
		// �v���C���[���ړ������֌�����
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.15f); // 0.125f
		Rotation(CQuaternion::LookRotation(forward));
	}
	else
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

	// ������e�ہA���[�V�����u���[�Ȃǂ̏����A�g���ĂȂ�
	/* 
	// �E�N���b�N�Œe�۔���
	if (CInput::PushKey(VK_RBUTTON))
	{
		 //�e�ۂ𐶐�
		new CBullet
		(
			// ���ˈʒu
			Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 20.0f,
			VectorZ(),	// ���˕���
			1000.0f,	// �ړ�����
			1000.0f		// �򋗗�
		);
	}

	 �uE�v�L�[�ŉ��̔��˂��I���I�t����
	if (CInput::PushKey('E'))
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
		else
		{
			mpFlamethrower->Stop();
		}
	}

	 �uB�v�L�[����������A���[�V�����u���[�J�n
	if (CInput::PushKey('B'))
	{
		// ���[�V�����u���[���|���Ă���Œ��ł���΁A
		// �V�������[�V�����u���[���|�������Ȃ�
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
	}

	 ���[�V�����u���[�̍X�V����
	UpdateMotionBlur();

	*/

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

	CDebugPrint::Print("FPS:%f\n \n", Times::FPS());

	CDebugPrint::Print("���v���C���[�̏��\n");
	CDebugPrint::Print("�@Grounded�F%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("�@�I�𒆂̍U���̋����F%d\n", (int)mSelectAttackPower);

	CDebugPrint::Print("�@�U���̋����F%s\n", GetAttackPowerStr().c_str());
	CDebugPrint::Print("�@�U���̕����F%s\n", GetAttackDirStr().c_str());
	CDebugPrint::Print("�@\n");

	mIsGrounded = false;
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
			//adjust.Y(0.0f);

			// �����߂��x�N�g���̕����W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
	else if (self == mpAttackCol1 || self == mpAttackCol2 || self == mpAttackCol3)
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
			CalcDamage(chara, &damage, &stan);

			// �_���[�W��^����
			chara->TakeDamage(damage, stan, this);

		}
	}
}

// �`��
void CPlayer::Render()
{
	CXCharacter::Render();
}
