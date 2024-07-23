#include "CCamera.h"
#include "glut.h"
#include "CApplication.h"

#define MOUSE_X_LIN 1910  //�}�E�X���W�͈̔�
#define MOUSE_Y_LIN 1070   //�}�E�X���W�͈̔�
#define PRIORITY 0  //�D��x


void CCamera::Start(double left, double right, double bottom, double top)
{
	//���f���r���[�s��̑ޔ�
	glPushMatrix();
	//���f���r���[�s��̏�����
	glLoadIdentity();
	//�v���W�F�N�V�����s��֐؂�ւ�
	glMatrixMode(GL_PROJECTION);
	//�v���W�F�N�V�����s��̑ޔ�
	glPushMatrix();
	//�v���W�F�N�V�����s��̏�����
	glLoadIdentity();
	//�\���G���A�̐ݒ�
	gluOrtho2D(left, right, bottom, top);
	//Depth�e�X�g�I�t
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
}

CCamera::CCamera()
	:ViewNumber(1)
	, VX(0.0f)
	, VY(0.0f)
	, CZ(10.0f)
	, mEye(1.0f, 2.0f, 3.0f)
	, mLine(this, &mMatrix, CVector(0.0f, -0.1f, 0.0f), CVector(0.0f, -0.1f, 10.0f))
	, mCollider(this, &mMatrix, CVector(), 1.5f)
{
	mCamera = ECamera::View1;
	CInput::GetMousePosD(&mMx, &mMy);
	//mScale = CVector(0.1f, 0.1f, 0.1f);
	//�C���X�^���X�̐ݒ�
	spInstance = this;
}

//CPlayer(�ʒu�A��]�A�X�P�[��)
CCamera::CCamera(const CVector& pos, const CVector& rot, const CVector& scale)
	:CCamera()
{
	CTransform::Update(pos, rot, scale); //�s��̍X�V
}

void CCamera::Update() {

	mpPlayer = CPlayer::Instance();
	mPosition = mpPlayer->Position() + CVector(0.0f, 1.1f, 0.0f);
	mRotation = CVector(VX, VY, 0.0f);


#pragma region [DisPlay]  
	int mx, my;  //�}�E�X�J�[�\�����W�擾�p
	//�}�E�X�J�[�\�����W�̎擾
	CInput::GetMousePosD(&mx, &my);

	//X�������̈ړ�
	//�O����E�ł���ΉE�ֈړ�
	if (mMx + 2 < mx)
	{
		VY -= 4.5f;
		mpPlayer->Rotation(CVector(0.0f, VY, 0.0f));
	}
	else if (mMx - 2 > mx)
	{
		VY += 4.5f;
		mpPlayer->Rotation(CVector(0.0f, VY, 0.0f));
	}

	//Y�������̈ړ�
	//�O���艺�ɂ���Ή��ֈړ�
	if (mMy + 2 < my)
	{
		VX += 4.5f;
	}
	else if (mMy - 2 > my)
	{
		VX -= 4.5f;
	}

	//���E�͈̔͂��o��Ɣ��Β[�ֈړ�������
	if (mx >= MOUSE_X_LIN)
	{
		mx = 1;
	}
	else if (mx <= 0)
	{
		mx = MOUSE_X_LIN - 1;
	}

	//�㉺�͈̔͂��o��Ɣ��Β[�ֈړ�������
	if (my >= MOUSE_Y_LIN)
	{
		my = 1;
	}
	else if (my <= 0)
	{
		my = MOUSE_Y_LIN - 1;
	}

	//���W��ޔ�����
	mMx = mx;
	mMy = my;

	//�}�E�X�J�[�\�����W�̍X�V
	CInput::SetMousePosD(mx, my);

#pragma endregion 

	if (VX > 91)
	{
		VX = 90;
	}
	else if (VX < -46)
	{
		VX = -45;
	}


	if (ViewNumber == 1)
	{
		mCamera = ECamera::View1;
	}
	else if (ViewNumber == 2)
	{
		mCamera = ECamera::View2;
	}
	else if (ViewNumber == 3)
	{
		mCamera = ECamera::View3;
	}

	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	CTransform::Update();
}

CMatrix CCamera::mModelViewInverse;

const CMatrix& CCamera::ModelViewInverse()
{
	return mModelViewInverse;
}

CCamera* CCamera::spInstance = nullptr;

CCamera* CCamera::Instance() 
{ 
	return spInstance;
}


//�Փˏ���
//Collision(�R���C�_�P,�R���C�_�Q)
void CCamera::Collision(CCollider* m, CCollider* o) {
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->Type()) {
	case CCollider::EType::ELINE:
		//����̃R���C�_���O�p�`�R���C�_�̎�
		if (o->Type() == CCollider::EType::ETRIANGLE) {
			CVector adjust; //�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			//if (CCollider::CollisionTriangleLine(o, m, &adjust))
			//{
			//}
		}
		break;
	case CCollider::EType::ESPHERE:
		if (o->Type() == CCollider::EType::ETRIANGLE) {
			CVector adjust; //�����p�x�N�g��
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
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
void CCamera::Collision()
{
	//�����R���C�_�̗D��x�ύX
	mLine.ChangePriority();
	//���R���C�_�̗D��x�ύX
	mCollider.ChangePriority();

	//�����R���C�_�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	//���R���C�_�[�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

void CCamera::Render()
{

#pragma region [mCamera]

	//�J�����̃p�����[�^�[���쐬����
	//e, c, u; //���_,�����_,�����

	switch (mCamera)
	{
	case ECamera::View1:

		//���_�ړ��i���j
		if (mInput.Key(VK_RIGHT))
		{
			VY = VY + 5.0f;
			mpPlayer->Rotation(CVector(0.0f, VY, 0.0f));
		}
		else if (mInput.Key(VK_LEFT))
		{
			VY = VY - 5.0f;
			mpPlayer->Rotation(CVector(0.0f, VY, 0.0f));
		}
		else if (mInput.Key(VK_DOWN))
		{
			VX = VX - 5.0f;
		}
		else if (mInput.Key(VK_UP))
		{
			VX = VX + 5.0f;
		}

		//���_�����߂�
		e = mPosition;
		//�����_�����߂�
		c = mPosition + CVector(0.0f, -1.0f, CZ) * CMatrix().RotateX(VX) * CMatrix().RotateY(VY);
		//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f);

		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

		if (mInput.Key('Y'))
		{
			ViewNumber = 2;
		}
		else if (mInput.Key('P'))
		{
			ViewNumber = 3;
		}

		break;

	case ECamera::View2:
		break;
	case ECamera::View3:

		//���_�ړ��i���j
		if (mInput.Key(VK_RIGHT))
		{
			VY = VY + 5.0f;
		}
		else if (mInput.Key(VK_LEFT))
		{
			VY = VY - 5.0f;
		}
		else if (mInput.Key(VK_DOWN))
		{
			VX = VX - 5.0f;
		}
		else if (mInput.Key(VK_UP))
		{
			VX = VX + 5.0f;
		}

		//���_�����߂�
		e = mpPlayer->Position() + CVector(0.0f, 0.0f, -3.0f) * mpPlayer->MatrixRotate() * CMatrix().RotateX(VX) * CMatrix().RotateY(VY);
		//�����_�����߂�
		c = mpPlayer->Position();
		//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f) * mpPlayer->MatrixRotate();

		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

		if (mInput.Key('T'))
		{
			ViewNumber = 1;
		}
		else if (mInput.Key('Y'))
		{
			ViewNumber = 2;
		}
		break;
	}
#pragma endregion

	CTransform::Update();

}

CMatrix CCamera::CameraMatrix()
{
	return mMatrix;
}

