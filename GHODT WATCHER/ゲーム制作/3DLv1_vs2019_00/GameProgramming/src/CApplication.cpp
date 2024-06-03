#include "CApplication.h"
#include "CRectangle.h"
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include <math.h>

//�N���X��static�ϐ�
CTexture CApplication::mTexture;
CCharacterManager CApplication::mCharacterManager;

#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��
//f14���f��
#define MODEL_OBJ "res\\BoxMan_No3.obj","res\\BoxMan_No3.mtl"
//�G�A���@���f��
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"
//�w�i���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\StageNo_09.obj","res\\StageNo_09.mtl"

#define MOS_POS_X 400  //�}�E�X���W��X�␳
#define MOS_POS_Y 300  //�}�E�X���W��Y�␳
#define MOUSE_X_LIN 1910  //�}�E�X���W�͈̔�
#define MOUSE_Y_LIN 1070   //�}�E�X���W�͈̔�

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	/*
	mColliderTriangle2.Set(nullptr, nullptr
		, CVector(50.0f, 0.0f, 50.0f)
		, CVector(50.0f, 0.0f, -50.0f)
		, CVector(-50.0f, 0.0f, 50.0f));
	//�O�p�R���C�_�̍쐬
	mColliderTriangle.Set(nullptr, nullptr
		, CVector(-50.0f, 0.0f, -50.0f)
		, CVector(-50.0f, 0.0f, 50.0f)
		, CVector(50.0f, 0.0f, -50.0f));
�@�@*/

//	new CEnemy3(CVector(-5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
//	new CEnemy3(CVector(5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
//	new CEnemy(&mModelC5, CVector(30.0f, 10.0f, -130.0f),
//		CVector(), CVector(0.1f, 0.1f, 0.1f));
	//�G�@�̃C���X�^���X�쐬
	new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -100.0f),
		CVector(), CVector(0.1f, 0.1f, 0.1f));
	//C5���f���̓ǂݍ���
	//mModelC5.Load(MODEL_C5);
	mEye = CVector(1.0f, 2.0f, 3.0f);
	mModel.Load(MODEL_OBJ);
	mModelBG.Load(MODEL_BACKGROUND);
	CMatrix matrix;
	matrix.Print();
	//mCharacter.Model(&mModel);
	//mCharacter.Scale(CVector(0.1f, 0.1f, 0.1f));
	mBackGround.Model(&mModelBG);
	mBackGround.Scale(CVector(2.5f, 2.5f, 2.5f));
	mPlayer.Model(&mModel);
	mPlayer.Position(CVector(11.0f, -3.5f, -53.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));
	mPlayer.Scale(CVector(0.2f, 0.2f, 0.2f));
	//�r���{�[�h�̐���
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//�w�i���f������O�p�R���C�_�𐶐�
    //�e�C���X�^���X�Ɛe�s��
//	mColliderMesh.Set(nullptr, nullptr, &mModelBG);
	mBackGround.ColliderMesh();

	spUi = new CUi();  //UI�N���X�̐���

	mCamera = ECamera::View1;
	int ViewNumber = 1;
	VX = 0.0f;
	VY = 0.0f;

	CInput::GetMousePosD(&mMx, &mMy);
}

void CApplication::Update()
{
#pragma region [Window]  

	/*
	int mx, my;  //�}�E�X�J�[�\�����W�擾�p
	//�}�E�X�J�[�\�����W�̎擾
	CInput::GetMousePos(&mx, &my);
	

	//�Q�[����ʒ��S����̍��W�֕ϊ�
	mx -= MOS_POS_X;
	my = MOS_POS_Y;

	//�������������ֈړ�������
	if (abs(mx) > abs(my))
	{
		//X���ňړ�
		if (mx < 0)
		{   //���ֈړ�
			VY += 5.0f;
		}
		else
		{   //�E�ֈړ�
			VY -= 5.0f;
		}
	}
	else
	{
		//Y���ňړ�
		if (my < 0)
		{   //���ֈړ�
			VX -= 5.0f;
		}
		else
		{   //��ֈړ�
			VX += 5.0f;
		}
	}
	*/

#pragma endregion 

#pragma region [DisPlay]  
	int mx, my;  //�}�E�X�J�[�\�����W�擾�p
	//�}�E�X�J�[�\�����W�̎擾
	CInput::GetMousePosD(&mx, &my);

	//X�������̈ړ�
	//�O����E�ł���ΉE�ֈړ�
	if (mMx + 2 < mx)
	{
		VY -= 4.5f;
		mPlayer.Rotation(CVector(0.0f, VY, 0.0f));
	}
	else if (mMx - 2 > mx)
	{
		VY += 4.5f;
		mPlayer.Rotation(CVector(0.0f, VY, 0.0f));
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




	//�^�X�N�}�l�[�W���[�̍X�V
	CTaskManager::Instance()->Update();
	//�R���W�����}�l�[�W���̏Փˏ���
//	CCollisionManager::Instance()->Collision();
	CTaskManager::Instance()->Collision();

	//�J�����̃p�����[�^�[���쐬����
	CVector e, c, u; //���_,�����_,�����

#pragma region [mCamera]

	switch (mCamera)
	{
	case ECamera::View1:

		//���_�ړ��i���j
		if (mInput.Key(VK_RIGHT))
		{
			VY = VY + 5.0f;
			mPlayer.Rotation(CVector(0.0f, VY, 0.0f));
		}
		else if (mInput.Key(VK_LEFT))
		{
			VY = VY - 5.0f;
			mPlayer.Rotation(CVector(0.0f, VY, 0.0f));
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
		e = mPlayer.Position() + CVector(0.0f, 0.7f, 0.0f);
		//�����_�����߂�
		c = mPlayer.Position() + CVector(0.0f, 3.0f, 10.0f) * CMatrix().RotateX(VX) * CMatrix().RotateY(VY);
		//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f);

		if (VX > 91)
		{
			VX = 90;
		}
		else if (VX < -46)
		{
			VX = -45;
		}

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
		//���_�̐ݒu
		//gluLookAt(���_X,���_Y,���_Z,���SX,���SY,���SZ,���X,���Y,���Z)
		gluLookAt(mEye.X(), mEye.Y(), mEye.Z(), mEye.X(), mEye.Y(), mEye.Z(), 0.0f, 1.0f, 0.0f);

		if (mInput.Key('J'))
		{
			mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
		}
		if (mInput.Key('L'))
		{
			mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
		}

		if (mInput.Key('I'))
		{
			mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
		}
		if (mInput.Key('K'))
		{
			mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
		}

		if (mInput.Key('M'))
		{
			mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
		}
		if (mInput.Key('O'))
		{
			mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
		}

		if (mInput.Key('T'))
		{
			ViewNumber = 1;
		}
		else if (mInput.Key('P'))
		{
			ViewNumber = 3;
		}
		break;
	case ECamera::View3:
		//���_�����߂�
		e = mPlayer.Position() + CVector(0.0f, 0.0f, -3.0f) * mPlayer.MatrixRotate();
		//�����_�����߂�
		c = mPlayer.Position();
		//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
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

	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

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

#pragma endregion

	//mBackGround.Render();
	//�^�X�N���X�g�̍폜
	CTaskManager::Instance()->Delete();
	//�^�X�N�}�l�[�W���[�̕`��
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();

	spUi->Render();   //UI�̕`��
}
/*
CTaskManager CApplication::mTaskManager;
CTaskManager* CApplication::TaskManager()
{
	return &mTaskManager;
}
*/

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi;   //�C���X�^���X�̃|�C���^��Ԃ�
}

CApplication::~CApplication()
{
	delete spUi;   //�C���X�^���XUi�̍폜
}