#include "CApplication.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CCamera.h"


void CApplication::Start()
{
	//3D�t�@�C���̓ǂݍ���
	mModelX.Load(MODEL_FILE);
	mKnight.Load(MODEL_KNIGHT);
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 1:�ړ�
	mKnight.SeparateAnimationSet(0, 1530, 1830, "idle1"); // 2:�ҋ@
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 3:�_�~�[
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 4:�_�~�[
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 5:�_�~�[
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 6:�_�~�[
	mKnight.SeparateAnimationSet(0, 440, 520, "attack1"); // 7:Attack1
	mKnight.SeparateAnimationSet(0, 520, 615, "attack2"); // 8:Attack2
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 9:�_�~�[
	mKnight.SeparateAnimationSet(0, 10, 80, "walk"); // 10:�_�~�[
	mKnight.SeparateAnimationSet(0, 1160, 1260, "death1"); // 11:�_�E��


	//�L�����N�^�[�Ƀ��f����ݒ�
	mXPlayer.Init(&mModelX);
	mFont.Load("FontG.png", 1, 4096 / 64);
	//�G�̏����ݒ�
	mXEnemy.Init(&mKnight);
	//�G�̔z�u
	mXEnemy.Position(CVector(7.0f, 0.0f, 0.0f));

	mXEnemy.ChangeAnimation(2, true, 200);
}

void CApplication::Update()
{

	//�L�����N�^�[�N���X�̍X�V
	mXPlayer.Update();
	//�G�̍X�V
	mXEnemy.Update();

	//�J�����̃p�����[�^�[���쐬����
	CVector e, c, u; //���_�A�����_�A�����
	//���_�����߂�
	e = CVector(1.0f, 2.0f, 10.0f);
	//�����_�����߂�
	c = CVector();
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f);
	//�J�����̐ݒ�
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//X���{��]
	if (mInput.Key('K')) {
		mMatrix = mMatrix * CMatrix().RotateX(1);
	}
	//X���|��]
	if (mInput.Key('I')) {
		mMatrix = mMatrix * CMatrix().RotateX(-1);
	}
	//Y���{��]
	if (mInput.Key('L')) {
		mMatrix = mMatrix * CMatrix().RotateY(1);
	}
	//Y���|��]
	if (mInput.Key('J')) {
		mMatrix = mMatrix * CMatrix().RotateY(-1);
	}
	//�s��ݒ�
	glMultMatrixf(mMatrix.M());

	//���_�ɃA�j���[�V������K�p����
	mModelX.AnimateVertex();

	//���f���`��
	mXPlayer.Render();
	                  //�`��̓J�����ݒ�̌�ŁI�I�I�I�I//
	//�R���C�_�̕`��
	CCollisionManager::Instance()->Render();
	//�G�`��
	mXEnemy.Render();

	//2D�`��J�n
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D RPOGRAMMING");

	//2D�̕`��I��
	CCamera::End();

	//�Փˏ���
	CCollisionManager::Instance()->Collision();

}

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

