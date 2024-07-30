//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CApplication.h"
#include "CCamera.h"
#include "CMatrix.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //Y����]���x
#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //X����]���x

#define RanZ CVector(0.0f,0.0f,0.2f)  //�ړ����x(�O��)
#define RanX CVector(0.2f,0.0f,0.0f)  //�ړ����x(���E)
#define JRanZ CVector(0.0f,0.0f,0.1f)  //
#define JRanX CVector(0.1f,0.0f,0.0f)

#define VELOCITYY CVector(0.0f,0.2f,0.0f)  //�W�����v����
#define GRAVITY 0.01    //�d��

CPlayer::CPlayer()
	: mPLine(this, &mMatrix, CVector(0.0f, 0.0f, 1.5f), CVector(0.0f, 0.0f, -1.5f))
	, mPLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -1.1f, 0.0f))
	, mPLine3(this, &mMatrix, CVector(2.0f, 0.5f, 0.0f), CVector(-2.0f, 0.5f, 0.0f))
	, mPLine4(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), CVector(0.0f, -1.1f, 0.0f))
	, JumpV(0)
	, ShootTime(30)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;
	//�v���C���[�s���X�e�[�g::�ҋ@
	mPState = EPState::EMOVE;
}

//CPlayer(�ʒu�A��]�A�X�P�[��)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale); //�s��̍X�V
}

//�X�V����
void CPlayer::Update() {

	switch (mPState) {
	case EPState::EIDEL:

		break;

	case EPState::EMOVE:

		//���L�[���͂ō��ɐi��
		if (mInput.Key('A')) {

			mPosition = mPosition + RanX * mMatrixRotate;
		}
		//�E�L�[���͂ŉE�ɐi��
		if (mInput.Key('D')) {

			mPosition = mPosition - RanX * mMatrixRotate;
		}

		//��L�[���͂őO�i
		if (mInput.Key('W')) {

			mPosition = mPosition + RanZ * mMatrixRotate;
		}
		//���L�[���͂Ō��
		if (mInput.Key('S'))
		{
			mPosition = mPosition - RanZ * mMatrixRotate;
		}

		
		//�X�y�[�X�L�[���͂ŃW�����v
		if (mInput.Key(VK_SPACE)) {
			JumpV = 0.2;
			mPState = EPState::EJUMP;
		}

		if (mInput.Key(VK_LBUTTON))
		{
			if (ShootTime >= 30)
			{
				CBullet* bullet = new CBullet();
				bullet->Set(0.1f, 1.5f);
				bullet->Position(CVector(0.0f, 0.0f, 1.0f) * CCamera::Instance()->CameraMatrix());
				bullet->Rotation(CCamera::Instance()->Rotation());
				bullet->Update();
				ShootTime = 0;
			}
			ShootTime++;
		}
		else
		{
				ShootTime = 30;
		}

		mPosition = mPosition + CVector(0.0f, JumpV, 0.0f);

		break;

	case EPState::EJUMP:
		mPosition = mPosition + CVector(0.0f, JumpV, 0.0f);
		JumpV -= GRAVITY;
		break;

	case EPState::ESHOOT:
		mPState = EPState::EMOVE;

		break;

	}

	//�ϊ��s��̍X�V
	CTransform::Update();
}

void CPlayer::Collision(CCollider* m, CCollider* o) {

	//���g�̃R���C�_�^�C�v�̔���
	switch (m->Type()) {

	case CCollider::EType::ELINE:
		//����̃R���C�_���O�p�`�R���C�_�̎�
		if (o->Type() == CCollider::EType::ETRIANGLE) {
			CVector adjust; //�����p�x�N�g��

			if (CCollider::CollisionTriangleLine(o, &mPLine2, &adjust) && JumpV != 0)
			{
				mPState = EPState::EMOVE;
				JumpV = 0;
				//CTransform::Update();
			}

			//�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//�ʒu�X�V
				mPosition = mPosition + adjust;
				//�s��X�V
				CTransform::Update();
			}
		}
		break;
	}
}

//�Փˏ���
void CPlayer::Collision()
{
	//�R���C�_�̗D��x�ύX
	mPLine4.ChangePriority();
	mPLine.ChangePriority();
	mPLine2.ChangePriority();
	mPLine3.ChangePriority();

	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mPLine4, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mPLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mPLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mPLine3, COLLISIONRANGE);

}

CPlayer* CPlayer::spInstance = nullptr;

CPlayer* CPlayer::Instance()
{
	return spInstance;
}
