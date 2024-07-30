#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMatrix.h"

class CXCharacter {
public:
	//����������
	void Init(CModelX* model);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framesize);
	//�X�V����
	void Update(CMatrix& m);
	//�`�揈��
	void Render();
	//�A�j���[�V�����̍Đ��I������
	//ture:�I���@false:�Đ���
	bool IsAnimationFinished();
	int AnimationIndex();  //�A�j���[�V�����ԍ��̎擾

protected:
	CModelX* mpModel;     //���f���f�[�^
	int mAnimationIndex;    //�A�j���[�V����
	bool mAnimationLoopFlg;   //true:�A�j���[�V����
	float mAnimationFrame;   //�A�j���[�V�����̍Đ��t���[��
	float mAnimationFrameSize;  //�A�j���[�V�����̍Đ��t���[����

};

#endif // !CXCHARACTER_H

