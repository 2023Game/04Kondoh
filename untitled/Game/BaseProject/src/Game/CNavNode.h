#pragma once

// �o�H�T���m�[�h�N���X
class CNavNode
{
public:
	// �R���X�g���N�^
	CNavNode(const CVector& pos, bool isDestNode = false);
	// �f�X�g���N�^
	~CNavNode();

	// �m�[�h�̍��W���擾
	const CVector& GetPos() const;
	// �Օ����`�F�b�N��\���p�ɁA������ɂ������m�[�h�̍��W��Ԃ�
	CVector GetOffsetPos() const;
	// �m�[�h�̍��W��ݒ�
	void SetPos(const CVector& pos);

	// �m�[�h�̐F�ݒ�
	void SetColor(const CColor& color);
	// �m�[�h��`��i�f�o�b�O�p�j
	void Render();

private:
	// �o�H�T�����Ɍo�R�ł��Ȃ��ړI�n��p�̃m�[�h���ǂ���
	bool mIsDestNode;
	CVector mPosition;  // �m�[�h�̍��W

	// �f�o�b�N�֘A
	CColor mColor;      // �m�[�h�̐F
};