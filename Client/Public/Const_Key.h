#pragma once

#include <string>
#include <winnt.h>

#define TIME60									TEXT("Timer_60")
#define AXIS_X									XMVectorSet(1.f, 0.f, 0.f, 0.f)
#define AXIS_Y									XMVectorSet(0.f, 1.f, 0.f, 0.f)
#define AXIS_Z									XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define AXIS_W									XMVectorSet(0.f, 0.f, 0.f, 1.f)

////float4 µð¹ö±ë
//_float4 fDebug{};
//XMStoreFloat4(&fDebug, µð¹ö±ëÇÒº¤ÅÍ´ëÀÔ);
//TCHAR debugMessage[256];
//_stprintf_s(debugMessage, _T("Debug_Value: x = %.6f, y = %.6f, z = %.6f, w = %.6f\n"),
//	fDebug.x, fDebug.y, fDebug.z, fDebug.w);
//OutputDebugString(debugMessage);

//// float µð¹ö±ë
//TCHAR debugMessage[256];
//_stprintf_s(debugMessage, _T("Debug_Value: %.6f\n"), 
// /* µð¹ö±ë_º¯¼ö_³Ö±â*/);
//OutputDebugString(debugMessage);



#define		VK_MAX		0xff



/* Texture */
static const std::wstring PRO_TEX_SKY					= TEXT("Prototype_Component_Texture_Sky");
static const std::wstring PRO_TEX_MASK					= TEXT("Prototype_Component_Texture_Mask");
static const std::wstring PRO_TEX_BRUSH					= TEXT("Prototype_Component_Texture_Brush");
static const std::wstring PRO_TEX_LOGO1					= TEXT("Prototype_Component_Texture_Logo1");
static const std::wstring PRO_TEX_LOGO2					= TEXT("Prototype_Component_Texture_Logo2");
static const std::wstring PRO_TEX_TERRAIN				= TEXT("Prototype_Component_Texture_Terrain");
static const std::wstring PRO_TEX_BACKGROUND			= TEXT("Prototype_Component_Texture_BackGround");


/* Component */
static const std::wstring PRO_COM_FSM					= TEXT("Prototype_Component_FSM");
static const std::wstring PRO_COM_NAVI					= TEXT("Prototype_Component_Navigation");
static const std::wstring PRO_COM_VI_CELL				= TEXT("Prototype_Component_VIBuffer_Cell");
static const std::wstring PRO_COM_VI_CUBE				= TEXT("Prototype_Component_VIBuffer_Cube");
static const std::wstring PRO_COM_VI_RECT				= TEXT("Prototype_Component_VIBuffer_Rect");
static const std::wstring PRO_COM_VI_GUIDE				= TEXT("Prototype_Component_VIBuffer_Guide");
static const std::wstring PRO_COM_VI_TERRAIN			= TEXT("Prototype_Component_VIBuffer_Terrain");

/* Collider */
static const std::wstring PRO_COM_COLL						= TEXT("Prototype_Component_Collider");
static const std::wstring PRO_COM_COLL_AABB					= TEXT("Prototype_Component_Collider_AABB");
static const std::wstring PRO_COM_COLL_OBB					= TEXT("Prototype_Component_Collider_OBB");
static const std::wstring PRO_COM_COLL_SPHERE				= TEXT("Prototype_Component_Collider_Sphere");

/* Model */
static const std::wstring PRO_MODEL_STRIFE				= TEXT("Prototype_Component_Model_Test");
static const std::wstring PRO_MODEL_FIONA				= TEXT("Prototype_Component_Model_Fiona");
static const std::wstring PRO_MODEL_GHOUL				= TEXT("Prototype_Component_Model_Ghoul"); 
static const std::wstring PRO_MODEL_FLOOR1				= TEXT("Prototype_Component_Model_Floor 1");
static const std::wstring PRO_MODEL_FLOOR2				= TEXT("Prototype_Component_Model_Floor 2");
static const std::wstring PRO_MODEL_FLOOR3				= TEXT("Prototype_Component_Model_Floor 3");
static const std::wstring PRO_MODEL_STAIR1				= TEXT("Prototype_Component_Model_Stair 1");
static const std::wstring PRO_MODEL_STAIR2				= TEXT("Prototype_Component_Model_Stair 2");
static const std::wstring PRO_MODEL_FORK				= TEXT("Prototype_Component_Model_ForkLift");
static const std::wstring PRO_MODEL_LGUN				= TEXT("Prototype_Component_Model_Gun Left");
static const std::wstring PRO_MODEL_RGUN				= TEXT("Prototype_Component_Model_Gun Right");
static const std::wstring PRO_MODEL_ROCK1				= TEXT("Prototype_Component_Model_Desert Rock1");
static const std::wstring PRO_MODEL_ROCK2				= TEXT("Prototype_Component_Model_Desert Rock2");
static const std::wstring PRO_MODEL_ROCK_TALL			= TEXT("Prototype_Component_Model_Desert RockTall");
static const std::wstring PRO_MODEL_FENCE				= TEXT("Prototype_Component_Model_Fence Lg A");
static const std::wstring PRO_MODEL_DAM_END				= TEXT("Prototype_Component_Model_Dam EndCap A");
static const std::wstring PRO_MODEL_DAM_END_ALT			= TEXT("Prototype_Component_Model_Dam_EndCap_A_Alt");
static const std::wstring PRO_MODEL_DAM_END_ALCOVE		= TEXT("Prototype_Component_Model_Dam MasterAlcove G");
static const std::wstring PRO_MODEL_DAM_TRIM			= TEXT("Prototype_Component_Model_Dam Trim");
static const std::wstring PRO_MODEL_GROUND_DECAL		= TEXT("Prototype_Component_Model_GroundDecal Lg A");
static const std::wstring PRO_MODEL_PILLAR_BASE			= TEXT("Prototype_Component_Model_Pillar Base");
static const std::wstring PRO_MODEL_BENT_DARK			= TEXT("Prototype_Component_Model_Bent Dark");
static const std::wstring PRO_MODEL_BENT_LIGHT			= TEXT("Prototype_Component_Model_Bent Light");
static const std::wstring PRO_MODEL_RUBBLESTONE			= TEXT("Prototype_Component_Model_Rubble DesertStone");
static const std::wstring PRO_MODEL_FORTRESS_WALL		= TEXT("Prototype_Component_Model_Fortress Wall 01");
static const std::wstring PRO_MODEL_PILLER_A_DARK		= TEXT("Prototype_Component_Model_PillarA Dark");
static const std::wstring PRO_MODEL_PILLER_A_LIGHT		= TEXT("Prototype_Component_Model_PillarA Light");
static const std::wstring PRO_MODEL_PILLER_B_DARK		= TEXT("Prototype_Component_Model_PillarB Dark");
static const std::wstring PRO_MODEL_PILLER_B_LIGHT		= TEXT("Prototype_Component_Model_PillarB Light");
static const std::wstring PRO_MODEL_RUIN_CNR_A			= TEXT("Prototype_Component_Model_Ruin CNR A");
static const std::wstring PRO_MODEL_RUIN_FLOOR			= TEXT("Prototype_Component_Model_Ruin Floor");
static const std::wstring PRO_MODEL_WALL_TOP			= TEXT("Prototype_Component_Model_Ruin Wall Top");
static const std::wstring PRO_MODEL_WODDEN_CIRCLE		= TEXT("Prototype_Component_Model_Wooden Circle C");
static const std::wstring PRO_MODEL_SPIKE_E				= TEXT("Prototype_Component_Model_FortressSpike E");
static const std::wstring PRO_MODEL_SPIKE_A				= TEXT("Prototype_Component_Model_FortressSpike A");
static const std::wstring PRO_MODEL_PILLAR_ALCOVE		= TEXT("Prototype_Component_Model_Pillar Alcove A");

/* Shader */
static const std::wstring PRO_SHADER_CELL				= TEXT("Prototype_Component_Shader_VtxCell");
static const std::wstring PRO_SHADER_CUBE				= TEXT("Prototype_Component_Shader_VtxCube");
static const std::wstring PRO_SHADER_MESH				= TEXT("Prototype_Component_Shader_VtxMesh");
static const std::wstring PRO_SHADER_NOR				= TEXT("Prototype_Component_Shader_VtxNorTex");
static const std::wstring PRO_SHADER_POS				= TEXT("Prototype_Component_Shader_VtxPosTex");
static const std::wstring PRO_SHADER_ANIM				= TEXT("Prototype_Component_Shader_VtxAnimMesh");

/* GameObject */
static const std::wstring PRO_OBJ_SKY					= TEXT("Prototype_GameObject_Sky");
static const std::wstring PRO_OBJ_GHOUL					= TEXT("Prototype_GameObject_Ghoul");
static const std::wstring PRO_OBJ_BULLET				= TEXT("Prototype_GameObject_Bullet"); 
static const std::wstring PRO_OBJ_PLAYER				= TEXT("Prototype_GameObject_Player");
static const std::wstring PRO_OBJ_WEAPON				= TEXT("Prototype_GameObject_Weapon");
static const std::wstring PRO_OBJ_FLOOR1				= TEXT("Prototype_GameObject_Floor 1");
static const std::wstring PRO_OBJ_STAIR1				= TEXT("Prototype_GameObject_Stair 1");
static const std::wstring PRO_OBJ_STAIR2				= TEXT("Prototype_GameObject_Stair 2");
static const std::wstring PRO_OBJ_MONSTER				= TEXT("Prototype_GameObject_Monster");
static const std::wstring PRO_OBJ_TERRAIN				= TEXT("Prototype_GameObject_Terrain");
static const std::wstring PRO_OBJ_L_GUN					= TEXT("Prototype_GameObject_Gun Left");
static const std::wstring PRO_OBJ_L_FIST				= TEXT("Prototype_GameObject_Fist Left");
static const std::wstring PRO_OBJ_R_FIST				= TEXT("Prototype_GameObject_Fist Right");
static const std::wstring PRO_OBJ_CAM_3RD				= TEXT("Prototype_GameObject_TP Camera");
static const std::wstring PRO_OBJ_R_GUN					= TEXT("Prototype_GameObject_Gun Right");
static const std::wstring PRO_OBJ_BACK					= TEXT("Prototype_GameObject_BackGround");
static const std::wstring PRO_OBJ_CAM_FREE				= TEXT("Prototype_GameObject_Camera Free");
static const std::wstring PRO_OBJ_BODY					= TEXT("Prototype_GameObject_Player Body");
static const std::wstring PRO_OBJ_GHOUL_BODY			= TEXT("Prototype_GameObject_Ghoul Body");
static const std::wstring PRO_OBJ_ROCK1					= TEXT("Prototype_GameObject_Desert Rock1");
static const std::wstring PRO_OBJ_ROCK2					= TEXT("Prototype_GameObject_Desert Rock2");
static const std::wstring PRO_OBJ_ROCK_TALL				= TEXT("Prototype_GameObject_Desert RockTall");
static const std::wstring PRO_OBJ_SPIKE_A				= TEXT("Prototype_GameObject_FortressSpike A");
static const std::wstring PRO_OBJ_SPIKE_E				= TEXT("Prototype_GameObject_FortressSpike E");

static const std::wstring PRO_OBJ_GROUND_DECAL			= TEXT("Prototype_GameObject_GroundDecal LG A");
static const std::wstring PRO_OBJ_DAM_END				= TEXT("Prototype_GameObject_Dam EndCap A");
static const std::wstring PRO_OBJ_DAM_END_ALT			= TEXT("Prototype_GameObject_Dam_EndCap_A_Alt");
static const std::wstring PRO_OBJ_DAM_END_ALCOVE		= TEXT("Prototype_GameObject_Dam MasterAlcove G");
static const std::wstring PRO_OBJ_FENCE					= TEXT("Prototype_GameObject_Fence Lg A");
static const std::wstring PRO_OBJ_PILLAR_BASE			= TEXT("Prototype_GameObject_Pillar Base");
static const std::wstring PRO_OBJ_BENT_DARK				= TEXT("Prototype_GameObject_Bent Dark");
static const std::wstring PRO_OBJ_BENT_LIGHT			= TEXT("Prototype_GameObject_Bent Light");
static const std::wstring PRO_OBJ_RUBBLESTONE			= TEXT("Prototype_GameObject_Rubble DesertStone");
static const std::wstring PRO_OBJ_FORTRESS_WALL			= TEXT("Prototype_GameObject_Fortress Wall 01");
static const std::wstring PRO_OBJ_PILLER_A_DARK			= TEXT("Prototype_GameObject_PillarA Dark");
static const std::wstring PRO_OBJ_PILLER_A_LIGHT		= TEXT("Prototype_GameObject_PillarA Light");
static const std::wstring PRO_OBJ_PILLER_B_DARK			= TEXT("Prototype_GameObject_PillarB Dark");
static const std::wstring PRO_OBJ_PILLER_B_LIGHT		= TEXT("Prototype_GameObject_PillarB Light");
static const std::wstring PRO_OBJ_RUIN_CNR_A			= TEXT("Prototype_GameObject_Ruin CNR A");
static const std::wstring PRO_OBJ_RUIN_FLOOR			= TEXT("Prototype_GameObject_Ruin Floor");
static const std::wstring PRO_OBJ_WALL_TOP				= TEXT("Prototype_GameObject_Ruin Wall Top");
static const std::wstring PRO_OBJ_DAM_TRIM				= TEXT("Prototype_GameObject_Dam Trim");
static const std::wstring PRO_OBJ_WODDEN_CIRCLE			= TEXT("Prototype_GameObject_Wooden Circle C");
static const std::wstring PRO_OBJ_PILLAR_ALCOVE			= TEXT("Prototype_GameObject_Pillar Alcove A");


/* Sockets */
static const std::wstring SOCKET_LEFT_HAND				= TEXT("Socket_Strife_Hand L");
static const std::wstring SOCKET_RIGHT_HAND				= TEXT("Socket_Strife_Hand R");
static const std::wstring SOCKET_HOLSTER_LEFT			= TEXT("Socket_Strife_Holster L");
static const std::wstring SOCKET_HOLSTER_RIGHT			= TEXT("Socket_Strife_Holster R");

static const std::wstring SOCKET_GHOUL_LEFT_HAND		= TEXT("Socket_Ghoul_Hand L");
static const std::wstring SOCKET_GHOUL_RIGHT_HAND		= TEXT("Socket_Ghoul_Hand R");

/* Layers */
static const std::wstring LAYER_DESERT_ROCK_1			= TEXT("Layer_Desert_Rock1");
static const std::wstring LAYER_DESERT_ROCK_2			= TEXT("Layer_Desert_Rock2");


//============================================Clone============================================


/* Component */
static const std::wstring COM_FSM						= TEXT("Component_FSM");
static const std::wstring COM_NAVI						= TEXT("Component_Navigation");
static const std::wstring COM_VI_CELL					= TEXT("Component_VIBuffer_Cell");
static const std::wstring COM_VI_CUBE					= TEXT("Component_VIBuffer_Cube");
static const std::wstring COM_VI_RECT					= TEXT("Component_VIBuffer_Rect");
static const std::wstring COM_VI_GUIDE					= TEXT("Component_VIBuffer_Guide");
static const std::wstring COM_VI_TERRAIN				= TEXT("Component_VIBuffer_Terrain");

/* Collider */
static const std::wstring COM_COLL						= TEXT("Component_Collider");
static const std::wstring COM_COLL_AABB					= TEXT("Component_Collider_AABB");
static const std::wstring COM_COLL_OBB					= TEXT("Component_Collider_OBB");
static const std::wstring COM_COLL_SPHERE				= TEXT("Component_Collider_Sphere");


static const std::wstring PRO_MODEL_RUIN_WALL_CNR_B		= TEXT("Prototype_Component_Model_Wall Cnr B");
static const std::wstring PRO_MODEL_PILLAR_DECO_A		= TEXT("Prototype_Component_Model_Pillar Base Deco A");

static const std::wstring PRO_OBJ_RUIN_WALL_CNR_B		= TEXT("Prototype_GameObject_Wall Cnr B");
static const std::wstring PRO_OBJ_PILLAR_DECO_A			= TEXT("Prototype_GameObject_Pillar Base Deco A");




//static const std::wstring PRO_OBJ_BACK = TEXT("Bone_Strife_Holster_L");
//static const std::wstring PRO_OBJ_BACK = TEXT("Prototype_Component_Texture_BackGround");
//static const std::wstring PRO_OBJ_BACK = TEXT("Prototype_Component_Texture_BackGround");

//Bone_Strife_Holster_L
//Bone_Strife_Holster_R


#define KEY_PUSHED (m_iKeyState & CPlayer::KEY_UP) || (m_iKeyState & CPlayer::KEY_DOWN) || (m_iKeyState & CPlayer::KEY_LEFT) || (m_iKeyState & CPlayer::KEY_RIGHT)

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
