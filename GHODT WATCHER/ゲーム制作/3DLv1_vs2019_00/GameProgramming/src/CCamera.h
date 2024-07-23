#ifndef CCAMERA_H
#define CCAMERA_H
#include "CCharacter3.h"
#include "CColliderLine.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CPlayer.h"
#include "CModel.h"
#include "CMatrix.h"
#include <vector>

/*
* �J�����N���X
* ��ʂɕ\������G���A��ݒ肷��
*/
class CCamera : public CCharacter3 {
public:
	CMatrix CameraMatrix();
	void Render();
	static void Start(double left, double right, double bottom, double top);
	void Update();
	////CBillBoard(�ʒu,��,����,�D��x)
	//CCamera(CVector pos, float w, float h, int priority);
	//CCamera(�ʒu�A��]�A�X�P�[��)
	CCamera(const CVector& pos, const CVector& rot, const CVector& scale);
	CCamera();

	static const CMatrix& ModelViewInverse();
	//�C���X�^���X�̃|�C���g�̎擾
	static CCamera* Instance();

	//�Փˏ���
	void Collision();
	//�Փˏ���
	//Collision(�R���C�_�P,�R���C�_�Q)
	void Collision(CCollider* m, CCollider* o);

	CVector e, c, u; //���_,�����_,�����
	float VX;  //�c���_�ړ��̒l
	float VY;  //�����_�ړ��̒l
	float CZ;
	float ShootTime;

private:
	enum class ECamera
	{
		View1,  //��l��
		View2,  //��l��
		View3,  //�O�l��
	};
	ECamera mCamera;
	int ViewNumber;
	CVector mEye;
	
	//�O��̃}�E�X���W
	int mMx, mMy;

	CInput mInput;
	CPlayer *mpPlayer;

	static CMatrix mModelViewInverse;
	//�J�����̃C���X�^���X
	static CCamera* spInstance;
	CColliderLine mLine;
	CCollider mCollider;
};

#endif // !CCAMERA_H
