#pragma once

class CInteractObject;

class CDoubleDoors : public CObjectBase
{
public:
	CDoubleDoors(CVector pos, CVector angle, CVector size);
	~CDoubleDoors();

	void AddInputObjs(CInteractObject* sw);

protected:
	// �X�C�b�`�������Ĕ����J�����ǂ���
	bool IsSwitchOn() const;

private:

	// ���͌n�̃I�u�W�F�N�g�̃��X�g
	std::list<CInteractObject*> mpInputObjs;
};