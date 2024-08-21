#pragma once
#include "CTexture.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>

#include "CSound.h"
#include "CVector.h"
#include "CModel.h"
#include "CCharacter3.h"
#include "CPlayer.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CEnemy3.h"
#include "CUi.h"
#include "CModelX.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"

class CApplication
{
private:
	CModelX mKnight;
	//�G�̃C���X�^���X
	CXEnemy mXEnemy;
	//�L�����N�^�̃C���X�^���X
	CXPlayer mXPlayer;
	CXCharacter mCharacter;
	CMatrix mMatrix;
	CModelX mModelX;
	static CUi* spUi;    //UI�N���X�̃|�C���^
	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;
	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;
	//C5���f��
	CModel mModelC5;
	CModel mBackGround; //�w�i���f��
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
	CInput mInput;
	CFont mFont;
    //CCharacter�̃|�C���^�̉ϒ��z��
    //std::vector<CCharacter*> mCharacters;
	CVector mEye;
	CModel mModel;
	CPlayer mPlayer;

public:
	~CApplication();
	static CUi* Ui();   //UI�N���X�̃C���X�^���X���擾
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};