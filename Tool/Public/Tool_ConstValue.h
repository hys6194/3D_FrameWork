#pragma once

#include <string>
#include <winnt.h>

#define TIME60									TEXT("Timer_60")
#define AXIS_X									XMVectorSet(1.f, 0.f, 0.f, 0.f)
#define AXIS_Y									XMVectorSet(0.f, 1.f, 0.f, 0.f)
#define AXIS_Z									XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define AXIS_W									XMVectorSet(0.f, 0.f, 0.f, 1.f)

#define	VK_MAX									0xff

/* Texture */
static const std::wstring PRO_TEX_SKY			= TEXT("Prototype_Component_Texture_Sky");
static const std::wstring PRO_TEX_MASK			= TEXT("Prototype_Component_Texture_Mask");
static const std::wstring PRO_TEX_BRUSH			= TEXT("Prototype_Component_Texture_Brush");
static const std::wstring PRO_TEX_LOGO1			= TEXT("Prototype_Component_Texture_Logo1");
static const std::wstring PRO_TEX_LOGO2			= TEXT("Prototype_Component_Texture_Logo2");
static const std::wstring PRO_TEX_TERRAIN		= TEXT("Prototype_Component_Texture_Terrain");
static const std::wstring PRO_TEX_BACKGROUND	= TEXT("Prototype_Component_Texture_BackGround");


/* Component */
static const std::wstring PRO_COM_FSM			= TEXT("Prototype_Component_FSM");
static const std::wstring PRO_COM_NAVI			= TEXT("Prototype_Component_Navigation");
static const std::wstring PRO_COM_VI_CELL		= TEXT("Prototype_Component_VIBuffer_Cell");
static const std::wstring PRO_COM_VI_CUBE		= TEXT("Prototype_Component_VIBuffer_Cube");
static const std::wstring PRO_COM_VI_RECT		= TEXT("Prototype_Component_VIBuffer_Rect");
static const std::wstring PRO_COM_VI_GUIDE		= TEXT("Prototype_Component_VIBuffer_Guide");
static const std::wstring PRO_COM_VI_TERRAIN	= TEXT("Prototype_Component_VIBuffer_Terrain");

/* Collider */
static const std::wstring PRO_COM_COLL_OBB		= TEXT("Prototype_Component_Collider_OBB");
static const std::wstring PRO_COM_COLL_AABB		= TEXT("Prototype_Component_Collider_AABB");
static const std::wstring PRO_COM_COLL_SPHERE	= TEXT("Prototype_Component_Collider_Sphere");

/* Model */
static const std::wstring PRO_MODEL_STRIFE		= TEXT("Prototype_Component_Model_Test");
static const std::wstring PRO_MODEL_FIONA		= TEXT("Prototype_Component_Model_Fiona");
static const std::wstring PRO_MODEL_FLOOR1		= TEXT("Prototype_Component_Model_Floor 1");
static const std::wstring PRO_MODEL_FLOOR2		= TEXT("Prototype_Component_Model_Floor 2");
static const std::wstring PRO_MODEL_FLOOR3		= TEXT("Prototype_Component_Model_Floor 3");
static const std::wstring PRO_MODEL_STAIR1		= TEXT("Prototype_Component_Model_Stair 1");
static const std::wstring PRO_MODEL_STAIR2		= TEXT("Prototype_Component_Model_Stair 2");
static const std::wstring PRO_MODEL_FORK		= TEXT("Prototype_Component_Model_ForkLift");
static const std::wstring PRO_MODEL_LGUN		= TEXT("Prototype_Component_Model_Gun Left");
static const std::wstring PRO_MODEL_RGUN		= TEXT("Prototype_Component_Model_Gun Right");
static const std::wstring PRO_MODEL_ROCK1		= TEXT("Prototype_Component_Model_Desert Rock1");
static const std::wstring PRO_MODEL_ROCK2		= TEXT("Prototype_Component_Model_Desert Rock2");

/* Shader */
static const std::wstring PRO_SHADER_CELL		= TEXT("Prototype_Component_Shader_VtxCell");
static const std::wstring PRO_SHADER_CUBE		= TEXT("Prototype_Component_Shader_VtxCube");
static const std::wstring PRO_SHADER_MESH		= TEXT("Prototype_Component_Shader_VtxMesh");
static const std::wstring PRO_SHADER_NOR		= TEXT("Prototype_Component_Shader_VtxNorTex");
static const std::wstring PRO_SHADER_POS		= TEXT("Prototype_Component_Shader_VtxPosTex");
static const std::wstring PRO_SHADER_ANIM		= TEXT("Prototype_Component_Shader_VtxAnimMesh");

/* GameObject */
static const std::wstring PRO_OBJ_SKY			= TEXT("Prototype_GameObject_Sky");
static const std::wstring PRO_OBJ_PLAYER		= TEXT("Prototype_GameObject_Player");
static const std::wstring PRO_OBJ_WEAPON		= TEXT("Prototype_GameObject_Weapon");
static const std::wstring PRO_OBJ_FLOOR1		= TEXT("Prototype_GameObject_Floor 1");
static const std::wstring PRO_OBJ_STAIR1		= TEXT("Prototype_GameObject_Stair 1");
static const std::wstring PRO_OBJ_STAIR2		= TEXT("Prototype_GameObject_Stair 2");
static const std::wstring PRO_OBJ_MONSTER		= TEXT("Prototype_GameObject_Monster");
static const std::wstring PRO_OBJ_TERRAIN		= TEXT("Prototype_GameObject_Terrain");
static const std::wstring PRO_OBJ_L_GUN			= TEXT("Prototype_GameObject_Gun Left");
static const std::wstring PRO_OBJ_CAM_3RD		= TEXT("Prototype_GameObject_TP Camera");
static const std::wstring PRO_OBJ_R_GUN			= TEXT("Prototype_GameObject_Gun Right");
static const std::wstring PRO_OBJ_BACK			= TEXT("Prototype_GameObject_BackGround");
static const std::wstring PRO_OBJ_CAM_FREE		= TEXT("Prototype_GameObject_Camera Free");
static const std::wstring PRO_OBJ_BODY			= TEXT("Prototype_GameObject_Player Body");
static const std::wstring PRO_OBJ_ROCK1			= TEXT("Prototype_GameObject_Desert Rock1");
static const std::wstring PRO_OBJ_ROCK2			= TEXT("Prototype_GameObject_Desert Rock2");

/* Sockets */
static const std::wstring SOCKET_LEFT_HAND		= TEXT("Socket_Strife_Hand L");
static const std::wstring SOCKET_RIGHT_HAND		= TEXT("Socket_Strife_Hand R");
static const std::wstring SOCKET_HOLSTER_LEFT	= TEXT("Socket_Strife_Holster L");
static const std::wstring SOCKET_HOLSTER_RIGHT	= TEXT("Socket_Strife_Holster R");

/* Layers */
static const std::wstring LAYER_DESERT_ROCK_1	= TEXT("Layer_Desert_Rock1");
static const std::wstring LAYER_DESERT_ROCK_2	= TEXT("Layer_Desert_Rock2");


//============================================Clone============================================


/* Component */
static const std::wstring COM_FSM				= TEXT("Component_FSM");
static const std::wstring COM_NAVI				= TEXT("Component_Navigation");
static const std::wstring COM_VI_CELL			= TEXT("Component_VIBuffer_Cell");
static const std::wstring COM_VI_CUBE			= TEXT("Component_VIBuffer_Cube");
static const std::wstring COM_VI_RECT			= TEXT("Component_VIBuffer_Rect");
static const std::wstring COM_VI_GUIDE			= TEXT("Component_VIBuffer_Guide");
static const std::wstring COM_VI_TERRAIN		= TEXT("Component_VIBuffer_Terrain");

/* Collider */
static const std::wstring COM_COLL_OBB			= TEXT("Component_Collider_OBB");
static const std::wstring COM_COLL_AABB			= TEXT("Component_Collider_AABB");
static const std::wstring COM_COLL_SPHERE		= TEXT("Component_Collider_Sphere");


//static const std::wstring PRO_OBJ_BACK = TEXT("Bone_Strife_Holster_L");
//static const std::wstring PRO_OBJ_BACK = TEXT("Prototype_Component_Texture_BackGround");
//static const std::wstring PRO_OBJ_BACK = TEXT("Prototype_Component_Texture_BackGround");

//Bone_Strife_Holster_L
//Bone_Strife_Holster_R

enum PLAYER_ANIMLIST
{
	SHOOT_RIGHT = 0,
	AIM_FIRE,
	AIM_FIRE_EMPTY,
	AIM_IDLE,
	AIM_IDLE_COMBAT,
	AIM_START,
	AIM_WALK,
	DASH,
	DASH_BACK,
	DASH_BACKEND,
	DASH_END,
	DASH_TO_BACK,
	DASH_TO_FRONT,
	DEATH,
	EMOTE,
	GHOOK_GIDLE,
	GHOOK_ALOOP,
	GHOOK_GPULL,
	GHOOK_ASTART,
	GHOOK_GSTART,
	GHOOK_AZIP,
	CHEST_OPEN,
	IDLE,
	JUMP,
	LAND,
	RUN,
	SWAP_IDLE,
	SWAP_START,
	ANIM_END
};