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
//�w�i���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\StageNo_09.obj","res\\StageNo_09.mtl"

#define MOS_POS_X 400  //�}�E�X���W��X�␳
#define MOS_POS_Y 300  //�}�E�X���W��Y�␳


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

	new CEnemy(CVector(11.0f, -2.0f, -50.0), CVector(), CVector(0.2f, 0.2f, 0.2f));
	mModel.Load(MODEL_OBJ);
	mModelBG.Load(MODEL_BACKGROUND);
	CMatrix matrix;
	matrix.Print();
	mBackGround.Model(&mModelBG);
	mBackGround.Scale(CVector(2.5f, 2.5f, 2.5f));
	mPlayer.Model(&mModel);
	mPlayer.Position(CVector(11.0f, -3.0f, -53.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));
	mPlayer.Scale(CVector(0.2f, 0.2f, 0.2f));
	//�r���{�[�h�̐���
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//�w�i���f������O�p�R���C�_�𐶐�
    //�e�C���X�^���X�Ɛe�s��
	mBackGround.ColliderMesh();

	spUi = new CUi();  //UI�N���X�̐���

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



	//�^�X�N�}�l�[�W���[�̍X�V
	CTaskManager::Instance()->Update();
	//�R���W�����}�l�[�W���̏Փˏ���
//	CCollisionManager::Instance()->Collision();
	CTaskManager::Instance()->Collision();


	//�^�X�N���X�g�̍폜
	CTaskManager::Instance()->Delete();
	//�^�X�N�}�l�[�W���[�̕`��
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();

	//spUi->Render();   //UI�̕`��
}
/*
CTaskManager CApplication::mTaskManager;
CTaskManager* CApplication::TaskManager()
{
	return &mTaskManager;
}
*/


CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi;   //�C���X�^���X�̃|�C���^��Ԃ�
}

CApplication::~CApplication()
{
	delete spUi;   //�C���X�^���XUi�̍폜
}