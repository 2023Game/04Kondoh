#pragma once
#include <initializer_list>

// �Փ˔��背�C���[
enum class ELayer
{
	eNone = -1,
	eTest,
	eField,
	eWall,
	ePlayer,
	eEnemy,
	eInteractObj,	// ���ׂ�I�u�W�F�N�g
	eInteractSearch,// ���ׂI�u�W�F�N�g�T�m�p
	eObj,	// ���ʂ̃I�u�W�F�N�g

	eAttackCol,	// �U�����C���[
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;
