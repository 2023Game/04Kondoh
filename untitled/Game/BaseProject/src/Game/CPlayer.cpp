//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CEnemyA.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;



#define PLAYER_CAP_UP    13.5f     // �v���C���[�̍���
#define PLAYER_CAP_DWON   2.5f     // �v���C���[�̒�
#define PLAYER_WIDTH      2.0f     // �v���C���[�̕�
#define ATTACK1_CAP_UP     80.0f     // �U���R���C�_�[1�̏�
#define ATTACK1_CAP_DWON    0.0f     // �U���R���C�_�[1�̉�
#define ATTACK2_CAP_UP      0.0f     // �U���R���C�_�[2�̏�
#define ATTACK2_CAP_DWON  -30.0f     // �U���R���C�_�[2�̉�
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
	{ "",							        true,	0.0f	},	// �퓬����T�|�[�Y
	{ PLAYER_ANIM_PATH"AttackIdle.x",	    true,	170.0f	},	// �퓬���̑ҋ@

	{ PLAYER_ANIM_PATH"AttackWalk.x",	    true,	34.0f	},	// ���s
	{ PLAYER_ANIM_PATH"BackWalk.x",	        true,	39.0f	},	// ���������s
	{ PLAYER_ANIM_PATH"LeftWalk.x",	        true,	40.0f	},	// ���������s
	{ PLAYER_ANIM_PATH"RightWalk.x",	    true,	35.0f	},	// �E�������s

	{ PLAYER_ANIM_PATH"UpAttackS.x",	    false,	54.0f	},	// ���U��
	{ PLAYER_ANIM_PATH"UpAttackM.x",	    false,	69.0f	},	// ����U��
	{ PLAYER_ANIM_PATH"UpAttackL.x",	    false,	92.0f	},	// ����U��

	{ PLAYER_ANIM_PATH"DwonAttackS.x",	   false,	50.0f	},	// �㉺�U��
	{ PLAYER_ANIM_PATH"DwonAttackM.x",	   false,	55.0f	},	// �����U��
	{ PLAYER_ANIM_PATH"DwonAttackL.x",	   false,	90.0f	},	// �����U��

	{ PLAYER_ANIM_PATH"RightAttackS.x",	   false,	60.0f	},	// ��E�U��
	{ PLAYER_ANIM_PATH"RightAttackM.x",	   false,	65.0f	},	// ���E�U��
	{ PLAYER_ANIM_PATH"RightAttackL.x",	   false,	86.0f	},	// ���E�U��

	{ PLAYER_ANIM_PATH"LeftAttackS.x",	   false,	50.0f	},	// �㍶�U��
	{ PLAYER_ANIM_PATH"LeftAttackM.x",	   false,	60.0f	},	// �����U��
	{ PLAYER_ANIM_PATH"LeftAttackL.x",	   false,	99.0f	},	// �����U��

	{ PLAYER_ANIM_PATH"Defense.x",	        true,	43.0f	},  // �h��
	{ PLAYER_ANIM_PATH"Evasion.x",	       false,	50.0f	},  // ���

	{ PLAYER_ANIM_PATH"jump_start.x",	   false,	25.0f	},	// �W�����v�J�n
	{ PLAYER_ANIM_PATH"jump.x",		        true,	1.0f	},	// �W�����v��
	{ PLAYER_ANIM_PATH"jump_end.x",	       false,	26.0f	},	// �W�����v�I��
};


// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mAttackWay(EAttackWay::eIdle)
	, mAttackPower(EAttackPower::eAttackM)
	, mCurrAttackPower(EAttackPower::eAttackM)
	, mMode(EMode::eBattle)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mMotionBlurRemainTime(0.0f)
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

	//// �c�����̃R���C�_�[�쐬
	//mpColliderLine = new CColliderLine
	//(
	//	this, ELayer::ePlayer,
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	//);
	//mpColliderLine->SetCollisionLayers({ ELayer::eField });

	//float width = PLAYER_WIDTH * 0.5f;
	//float posY = PLAYER_HEIGHT * 0.5f;
	//// �������iX���j�̃R���C�_�[�쐬
	//mpColliderLineX = new CColliderLine
	//(
	//	this, ELayer::ePlayer,
	//	CVector(-width, posY, 0.0f),
	//	CVector( width, posY, 0.0f)
	//);
	//mpColliderLineX->SetCollisionLayers({ ELayer::eField });

	//// �������iZ���j�̃R���C�_�[�쐬
	//mpColliderLineZ = new CColliderLine
	//(
	//	this, ELayer::ePlayer,
	//	CVector(0.0f, posY, -width),
	//	CVector(0.0f, posY,  width)
	//);
	//mpColliderLineZ->SetCollisionLayers({ ELayer::eField });

	// �J�v�Z���R���C�_�[�쐬
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, PLAYER_CAP_DWON, 0.0f),
		CVector(0.0f, PLAYER_CAP_UP, 0.0f),
		PLAYER_WIDTH,true
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField, ELayer::eWall});

	// �U���p�̃R���C�_�P�i���̐n�̕����j
	mpAttackCollider1 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK1_CAP_DWON),
		CVector(0.0f, 0.0f, ATTACK1_CAP_UP),
		0.3f, true
	);
	mpAttackCollider1->SetCollisionTags({ ETag::eEnemy });
	mpAttackCollider1->SetCollisionLayers({ ELayer::eEnemy });
	mpAttackCollider1->Rotate(CVector(-4.5f, 14.1f, 0.0f));

	// �U���p�̃R���C�_�Q�i���̎�����̕����j
	mpAttackCollider2 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK2_CAP_UP),
		CVector(0.0f, 0.0f, ATTACK2_CAP_DWON),
		0.8, true
	);
	mpAttackCollider2->SetCollisionTags({ ETag::eEnemy });
	mpAttackCollider2->SetCollisionLayers({ ELayer::eEnemy });
	mpAttackCollider2->Rotate(CVector(-4.5f, 14.1f, 0.0f));

	// �U���p�̃R���C�_�R�i���̕����j
	mpAttackCollider3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		15.0, true
	);
	mpAttackCollider3->SetCollisionTags({ ETag::eEnemy });
	mpAttackCollider3->SetCollisionLayers({ ELayer::eEnemy });
	mpAttackCollider3->Translate(0.0f, 0.0f, -12.0f);


	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// ���̃{�[�����擾
	CModelXFrame* Sword = mpModel->FinedFrame("Armature_mixamorig_Sword_joint");
	const CMatrix& swordMTX = Sword->CombinedMatrix();
	// ���̃{�[�����擾
	CModelXFrame* Shield = mpModel->FinedFrame("Armature_mixamorig_Shield_joint");
	const CMatrix& shieldMTX = Shield->CombinedMatrix();
	// �U���p�̃R���C�_�[�����̍s��ɐݒ�
	mpAttackCollider1->SetAttachMtx(&swordMTX);
	mpAttackCollider2->SetAttachMtx(&swordMTX);
	mpAttackCollider3->SetAttachMtx(&shieldMTX);


}

CPlayer::~CPlayer()
{
	// �R���C�_�[��j��
	//SAFE_DELETE(mpColliderLine);
	//SAFE_DELETE(mpColliderLineX);
	//SAFE_DELETE(mpColliderLineZ);
	SAFE_DELETE(mpColliderCapsule);

	// �o�H�T���p�̃m�[�h��j��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
	}

	spInstance = nullptr;
}

// ���݂̏�Ԃ��擾
CPlayer::EState CPlayer::GetState()
{
	return mState;
}

// �U���̕������擾
CPlayer:: EAttackWay CPlayer::GetAttackWay()
{
	return mAttackWay;
}

// �U���̈З͂��擾
CPlayer::EAttackPower CPlayer::GetAttackPower()
{
	return mAttackPower;
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �U��������A�U���̈З͂ɂ����
// �؂�ւ���A�j���[�V�����̎�ނ��擾
CPlayer::EAnimType CPlayer::GetAttackAnimType() const
{
	switch (mAttackWay)
	{
	case EAttackWay::eUpAttack:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eUpAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eUpAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eUpAttackL;
		break;
	case EAttackWay::eDwonAttack:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eDwonAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eDwonAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eDwonAttackL;
		break;
	case EAttackWay::eLeftAttack:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eLeftAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eLeftAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eLeftAttackL;
		break;
	case EAttackWay::eRightAttack:
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
	mMoveSpeed = CVector::zero;
	ChangeState(EState::eAttack);
	mAttackPower = mCurrAttackPower;

	// ����͂ŁA��U��
	if (CInput::Key('W'))
	{
		mAttackWay = EAttackWay::eUpAttack;
	}
	// �����͂ŁA���U��
	else if (CInput::Key('S'))
	{
		mAttackWay = EAttackWay::eDwonAttack;
	}
	// �����͂ŁA���U��
	else if (CInput::Key('A'))
	{
		mAttackWay = EAttackWay::eLeftAttack;
	}
	// �E���͂ŁA�E�U��
	else if (CInput::Key('D'))
	{
		mAttackWay = EAttackWay::eRightAttack;
	}
	// �������͂����Ă��Ȃ���΁A��U��
	else
	{
		mAttackWay = EAttackWay::eUpAttack;
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
		// ���N���b�N�ōU��
		if (CInput::PushKey(VK_LBUTTON))
		{
			ChangeAttack();
		}
		// �E�N���b�N�Ŗh��
		else if (CInput::PushKey(VK_RBUTTON))
		{
			mState = EState::eDefense;
		}
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
		// �V�t�g�ŉ��
		else if (CInput::Key(VK_SHIFT))
		{
			ChangeEvasion();
		}
	}
}

// �U��
void CPlayer::UpdateAttack()
{
	// �v���C���[�̍U����Ԃɍ��킹�ăA�j���[�V������؂�ւ���
	EAnimType anim = GetAttackAnimType();
	ChangeAnimation(anim);

	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;

	// �a��SE�̍Đ��ς݃t���O��������
	mIsPlayedSlashSE = false;
	// �a���G�t�F�N�g�̐����ς݃t���O��������
	mIsSpawnedSlashEffect = false;
}

// �U���I���҂�
void CPlayer::UpdateAttackWait()
{
	// �a��SE���Đ����Ă��Ȃ����A�A�j���[�V������25%�ȏ�i�s������A
	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
	{
		// �a��SE���Đ�
		mpSlashSE->Play();
		mIsPlayedSlashSE = true;
	}

	// �a���G�t�F�N�g�𐶐����Ă��Ȃ����A�A�j���[�V������35%�ȏ�i�s������A
	if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.35f)
	{
		// �a���G�t�F�N�g�𐶐����āA���ʕ����֔�΂�
		CSlash* slash = new CSlash
		(
			this,
			Position() + CVector(0.0f, 10.0f, 0.0f),
			VectorZ(),
			300.0f,
			100.0f
		);
		// �a���G�t�F�N�g�̐F�ݒ�
		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));

		mIsSpawnedSlashEffect = true;
	}

	// �U���A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		mState = EState::eIdle;
		mAttackWay = EAttackWay::eIdle;
		ChangeAnimation(EAnimType::eAttackIdle);
	}
}

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
	//mMoveSpeed = CVector::zero;
	//float moveFrame = _MOVE_END - ATTACK_MOVE_START;
	//float percent = (frame - ATTACK_MOVE_START) / moveFrame;

}

//void CPlayer::UpEvasion()
//{
//	
//}
//
//void CPlayer::DwonEvasion()
//{
//	for (int i = 0; i < 10; i++)
//	{
//		mMoveSpeed += VectorZ() * EVA_MOVE_MINUS;
//	}
//}
//
//void CPlayer::LeftEvasion()
//{
//	for (int i = 0; i <= 20; i++)
//	{
//		mMoveSpeed += VectorX() * EVA_MOVE_MINUS;
//	}
//}
//
//void CPlayer::RightEvasion()
//{
//	for (int i = 0; i <= 10; i++)
//	{
//		mMoveSpeed += VectorX() * EVA_MOVE_PLUS;
//	}
//}

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
	mState = EState::eJump;

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// �W�����v��
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CPlayer::UpdateJumpEnd()
{
	// �W�����v�A�j���[�V�������I�����A
	// �n�ʂɐڒn������A�ҋ@��Ԃ֖߂�
	if (IsAnimationFinished() && mIsGrounded)
	{
		mState = EState::eIdle;
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


// �X�V
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (CInput::PushKey('T'))
	{
		IsBattleMode = !IsBattleMode;
	}

	if (IsBattleMode)
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
		int power = ((int)mCurrAttackPower + 1) % powerNum;
		mCurrAttackPower = (EAttackPower)power;
	}
	// �}�E�X�z�C�[�������ɃX�N���[������Ă�����A���݂̍U���З̓_�E��
	else if (WheelDelta < 0)
	{
		int power = ((int)mCurrAttackPower + powerNum - 1) % powerNum;
		mCurrAttackPower = (EAttackPower)power;
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
	case EState::eAttackWait:
		UpdateAttackWait();
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

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// �ړ�
	Position(Position() + moveSpeed);

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
		/*Rotation(0.0f, 90.0f, 0.0f);*/
		// TODO�F�v���C���[��G�Ɍ�����
		// TODO�F�G�Ɏ��_���{�^���������ă��b�N���邩�A�����œG�Ɍ����悤�ɂ���
	}


	//// �E�N���b�N�Œe�۔���
	//if (CInput::PushKey(VK_RBUTTON))
	//{
	//	 //�e�ۂ𐶐�
	//	new CBullet
	//	(
	//		// ���ˈʒu
	//		Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 20.0f,
	//		VectorZ(),	// ���˕���
	//		1000.0f,	// �ړ�����
	//		1000.0f		// �򋗗�
	//	);
	//}

	// �uE�v�L�[�ŉ��̔��˂��I���I�t����
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

	// �uP�v�L�[����������A�Q�[�����I��
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// �uB�v�L�[����������A���[�V�����u���[�J�n
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

	// ���[�V�����u���[�̍X�V����
	UpdateMotionBlur();

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �R���C�_�[�̍s���ύX
	mpAttackCollider1->Update();
	mpAttackCollider2->Update();
	mpAttackCollider3->Update();

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	//	CDebugPrint::Print("Mode:%d\n",(int)mMode);
	CDebugPrint::Print("Power:%d\n", (int)mCurrAttackPower);

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}


// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �c�����̏Փˏ���
	/*
	if (self == mpColliderLine)
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
	}

	// �������iX����Z���j�̏Փˏ���
	else if (self == mpColliderLineX || self == mpColliderLineZ)
	{
		if (other->Layer() == ELayer::eField)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕����W���ړ�
			Position(Position() + adjust * hit.weight);

		}
	}
	*/

	// �����蔻��
	if (self == mpColliderCapsule)
	{
		if (other->Layer() == ELayer::eField)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			//adjust.X(0.0f);
			//adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// �Փ˂����n�ʂ������V�䂩����ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			float sideX = CVector::Dot(normal, CVector::right);
			float sideZ = CVector::Dot(normal, CVector::forward);
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
}

// �`��
void CPlayer::Render()
{
	CXCharacter::Render();
}
