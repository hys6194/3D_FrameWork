#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };
	enum class MODELTYPE { TYPE_NONANIM, TYPE_ANIM };
	enum  MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB };
	enum class MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z };
	enum TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_END };
	enum COLL_OPT { OP_IMPACT, OP_TARGET, OP_DETECT, OP_END };
	enum TRAIL_TYPE { TR_NORMAL, TR_SWORD, TR_MOTION, TR_END };
}
#endif // Engine_Enum_h__
