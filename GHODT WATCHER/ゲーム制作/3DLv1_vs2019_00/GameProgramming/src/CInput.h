#ifndef CINPUT_H
#define CINPUT_H
#include <Windows.h>
#include "GLFW/glfw3.h"

class CInput
{
public:
	/*
	������
	Init(�E�B���h�E�|�C���^)
	*/
	static void Init(GLFWwindow* w);
	/*
	GetMousePos(X���W�|�C���^,Y���W�|�C���^)
	�E�B���h�E�̍�������_�Ƃ����A
	�}�E�X�J�[�\���̍��W���A�����ɐݒ�
	*/
	static void GetMousePos(int* px, int* py);

	/*
	�}�E�X�J�[�\���̍��W���擾����
	GetMousePos(X���W�|�C���^,Y���W�|�C���^)
	�f�B�X�v���C�̍�������_(0,0)�Ƃ����A
	�}�E�X�J�[�\���̍��W���A�����ɐݒ肷��
	��ʉE������X���v���X
	��ʉ�������Y���v���X
	*/
	static void GetMousePosD(int* px, int* py);

	/*
	�}�E�X�J�[�\���̍��W��ݒ肷��
	SetCursorPosD(X���W,Y���W)
	�f�B�X�v���C�̍��オ���_(0,0)
	*/
	static void SetMousePosD(int x, int y);

	static void Window(GLFWwindow* pwindow);
	CInput();
	//bool Key(����)
	//�߂�l
	//true:�����̃L�[��������Ă���
	//false:�����̃L�[��������Ă��Ȃ�
	bool Key(char key);
private:
	static GLFWwindow* spWindow;	//�E�B���h�E�̃|�C���^
};


#endif // !CINPUT_H