#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H
//�Փˏ����͈͂�藣��Ă���R���C�_�͏Փˏ������Ȃ�
#define COLLISIONRANGE 200

#include "CTaskManager.h"

class CCollisionManager : public CTaskManager
{
public:
	//�Փˏ���
    //Collision(�R���C�_,�͈�)
    //�R���C�_�̗D��x�{�͈́`�D��x-�͈͂܂ł̃R���C�_�ƏՓ˔�������s����
	void CCollisionManager::Collision(CCollider* c, int range);
	//�Փˏ���
	void Collision();
	//�C���X�^���X�̎擾
	static CCollisionManager* Instance();

private:
	//�}�l�[�W���̃C���X�^���X
	static CCollisionManager* mpInstance;

};

#endif // !CCOLLISIONMSNSGER_H
