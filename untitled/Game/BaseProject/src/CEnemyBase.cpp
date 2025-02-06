#include "CEnemyBase.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CColliderCapsule.h"
#include "CGaugeUI3D.h"
#include "CEnemyManager.h"

#define GRAVITY 0.0625f;

// �R���X�g���N�^
CEnemyBase::CEnemyBase()
	:CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	,mState(0)
	,mStateStep(0)
	,mElapsedTime(0.0f)
	,mMoveSpeed(CVector::zero)
	,mMoveSpeedY(0.0f)
	,mIsGrounded(false)
	,mGroundNormal(CVector::up)
	,mpBodyCol(nullptr)
	,mpHpGauge(nullptr)
{
	// �G�l�~�[�Ǘ��N���X�Ɏ��g��ǉ�
	CEnemyManager::Instance()->Add(this);
	//�@TODO:HP�Q�[�W���쐬
}

//�f�X�g���N�^
CEnemyBase::~CEnemyBase()
{
	// �G�l�~�[�Ǘ��N���X���玩�g����菜��
	CEnemyManager::Instance()->Remove(this);

	// �R���C�_�[�폜
	SAFE_DELETE(mpBodyCol);

	// HP�Q�[�W�����݂�����A�ꏏ�ɍ폜����
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

//�I�u�W�F�N�g���폜��`����֐�
void CEnemyBase::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�̂�HP�Q�[�W�ł����
	// HP�Q�[�W�̃|�C���^����ɂ���
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

bool CEnemyBase::IsState(int state)
{
	if ((int)mState == state) return true;
	return false;
}

// �G�̏�����
void CEnemyBase::InitEnemy(std::string path, const std::vector<AnimData>* pAnimData)
{
	// �A�j���[�V�����f�[�^�e�[�u����ݒ�
	mpAnimData = pAnimData;

	// ���f���f�[�^��ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>(path);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = mpAnimData->size();
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = (*mpAnimData)[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}

	// CXCharacter�̏�����
	Init(model);
}

// �Փˏ���
void CEnemyBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̂��Փ˂���
	if (self == mpBodyCol)
	{
		// �t�B�[���h�ƏՓ˂���
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
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A�V��ƏՓ˂���
			else
			{
				// �W�����v�ȂǂœV��ɉ�����Փ˂������@�i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// �v���C���[�ƏՓ˂����ꍇ
		else if (other->Layer() == ELayer::ePlayer)
		{
			// �������̂݉����߂�����
			// �����߂����x�N�g����Y�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
	
}

// �A�j���[�V�����؂�ւ�
void CEnemyBase::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// ��Ԑ؂�ւ�
void CEnemyBase::ChangeState(int state)
{
	// ������Ԃ̏ꍇ�͐؂�ւ��Ȃ�
	if (state == mState) return;

	// ��Ԃ�ύX���āA��Ԋ֘A�̕ϐ���������
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CEnemyBase::ChangeAttackType(int attacktype)
{
	if (attacktype == mAttackType) return;

	mAttackType = attacktype;
}


// �X�V
void CEnemyBase::Update()
{
	// �d��
	mMoveSpeedY -= GRAVITY;

	// �ړ�
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;

	// TODO:HP�Q�[�W���X�V

}

// �`��
void CEnemyBase::Render()
{
	CXCharacter::Render();
}



