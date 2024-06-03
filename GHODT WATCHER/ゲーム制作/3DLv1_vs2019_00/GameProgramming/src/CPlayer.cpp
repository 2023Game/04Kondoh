//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CApplication.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //Y����]���x
#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //X����]���x
#define VELOCITYZ CVector(0.0f,0.0f,0.2f)  //�ړ����x(�O��)
#define VELOCITYX CVector(0.2f,0.0f,0.0f)  //�ړ����x(���E)
#define VELOCITYY CVector(0.0f,0.2f,0.0f)  //�W�����v����
#define GRAVITY 0.05    //�d��

CPlayer::CPlayer()
	:mLine(this, &mMatrix, CVector(0.0f, 0.0f, 1.0f), CVector(0.0f, 0.0f, -1.0f))
	, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -1.3f, 0.0f))
	, mLine3(this, &mMatrix, CVector(1.5f, 0.0f, 0.0f), CVector(-1.5f, 0.0f, 0.0f))
	, mLine4(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, -1.2f, 0.0f))
	,JumpV(0)
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
			//Y���̉�]�l������
			mPosition = mPosition + VELOCITYX * mMatrixRotate;
		}
		//�E�L�[���͂ŉE�ɐi��
		if (mInput.Key('D')) {
			//Y���̉�]�l�𑝉�
			mPosition = mPosition - VELOCITYX * mMatrixRotate;
		}

		//��L�[���͂őO�i
		if (mInput.Key('W')) {
			//Z�������̒l����]�����ړ�������
			mPosition = mPosition + VELOCITYZ * mMatrixRotate;
		}
		//���L�[���͂Ō��
		if (mInput.Key('S'))
		{
			mPosition = mPosition - VELOCITYZ * mMatrixRotate;
		}
		
		//�X�y�[�X�L�[���͂ŃW�����v
		if (mInput.Key(VK_SPACE)) {
			JumpV = 0.5;
			mPState = EPState::EJUMP;
		}

		break;

	case EPState::EJUMP:
		mPosition = mPosition + CVector(0.0f, JumpV, 0.0f);
		JumpV -= GRAVITY;

	}
	//JumpV -= GRAVITY;
	//mPosition = mPosition + CVector(0.0f, JumpV, 0.0f);

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
			//�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//�ʒu�X�V
				mPosition = mPosition + adjust;
				mPState = EPState::EMOVE;
				JumpV = 0;
				//�s��X�V
				CTransform::Update();
			}
			if (CCollider::CollisionTriangleLine(o, &mLine4, &adjust))
			{
				//�ʒu�X�V
				mPosition = mPosition + adjust;
				mPState = EPState::EMOVE;
				JumpV = 0;
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
	mLine4.ChangePriority();
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();

	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mLine4, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}

CPlayer* CPlayer::spInstance = nullptr;

CPlayer* CPlayer::Instance()
{
	return spInstance;
}
