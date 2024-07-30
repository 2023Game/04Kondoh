#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMatrix.h"
#include "CCharacter3.h"

class CXCharacter : public CCharacter3 {
public:
	//�f�t�H���g�R���X�g���N�^
	CXCharacter();
	//����������
	void Init(CModelX* model);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framesize);
	//�X�V����
	void Update();
	//�X�V����
	//materix: �ړ��A��]�A�g��k���̍s��
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

