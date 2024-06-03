#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CMiss.h"
#include <vector>
#include "CCharacterManager.h"

#include "CGame.h"
#include "CSound.h"
#include "CVector.h"
#include "CModel.h"
#include "CCharacter3.h"
#include "CPlayer.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CEnemy3.h"
#include "CBackGround.h"

class CApplication
{
private:
	enum class ECamera
	{
		View1,  //��l��
		View2,  //��l��
		View3,  //�O�l��
	};
	ECamera mCamera;
	float VX;  //�c���_�ړ��̒l
	float VY;  //�����_�ړ��̒l

	//�O��̃}�E�X���W
	int mMx, mMy;

	int ViewNumber;
	static CUi* spUi;    //UI�N���X�̃|�C���^
	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;
	//�O�p�R���C�_�Q�쐬
	// CColliderTriangle mColliderTriangle2;
	//�O�p�R���C�_�쐬
	// CColliderTriangle mColliderTriangle;
	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;
	//C5���f��
	CModel mModelC5;  //C5�̃��f��
	CModel mModel;    //�v���C���[�̃��f��
	CModel mModelBG;  //�X�e�[�W�̃��f��
	CBackGround mBackGround; //�w�i���f��
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	CGame* mpGame;
	static CCharacterManager mCharacterManager;
	enum class EGame
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EGame mGame;
//  CCharacter mRectangle;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
//  CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	CMiss* mpMiss;
    //CCharacter�̃|�C���^�̉ϒ��z��
//  std::vector<CCharacter*> mCharacters;
	CVector mEye;
//	CCharacter3 mCharacter;
	CPlayer mPlayer;

public:
	~CApplication();
	static CUi* Ui();   //UI�N���X�̃C���X�^���X���擾
	static const CMatrix& ModelViewInverse();
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};