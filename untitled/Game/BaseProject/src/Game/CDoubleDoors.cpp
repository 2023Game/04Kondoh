#include "CDoubleDoors.h"
#include "CInteractObject.h"


CDoubleDoors::CDoubleDoors(CVector pos, CVector angle, CVector size)
	: CObjectBase(ETag::eWall, ETaskPriority::eBackground)
{

}

CDoubleDoors::~CDoubleDoors()
{
}

bool CDoubleDoors::IsSwitchOn() const
{
	for (CInteractObject* sw : mpInputObjs)
	{
		if (!sw->IsOnInteractObj()) return false;
	}
	return true;
}
