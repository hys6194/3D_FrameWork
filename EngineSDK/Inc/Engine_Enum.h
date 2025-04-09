#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };
	enum class MODELTYPE { TYPE_NONANIM, TYPE_ANIM };
	enum  MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB };
	enum class MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z };
	enum COLL_TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_END };
}
#endif // Engine_Enum_h__
