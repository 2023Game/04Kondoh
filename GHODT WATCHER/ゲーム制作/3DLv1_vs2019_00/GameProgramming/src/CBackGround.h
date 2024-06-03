#ifndef CBACKGROUND_H
#define CBACKGROUND_H
//�L�����N�^�[�N���X�C���N���[�h
#include "CCharacter3.h"
#include "CColliderMesh.h"


/*
�L�����N�^�[�N���X�`��
*/
class CBackGround : public CCharacter3 {
public:
	static CBackGround* Instance();

    CBackGround();
	//CBackGround(�ʒu,��],�X�P�[��)
	CBackGround(const CVector& pos, const CVector& rot, const CVector& scale);
	//�X�V����
	void Update();
	void ColliderMesh();
private:
	static CBackGround* spInstance;
	CColliderMesh mColliderMesh;
};

#endif // !CBACKGROUND_H

