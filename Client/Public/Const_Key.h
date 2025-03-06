#pragma once

#include <string>
#include <winnt.h>

#define TIME60									TEXT("Timer_60")
#define AXIS_X									XMVectorSet(1.f, 0.f, 0.f, 0.f)
#define AXIS_Y									XMVectorSet(0.f, 1.f, 0.f, 0.f)
#define AXIS_Z									XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define AXIS_W									XMVectorSet(0.f, 0.f, 0.f, 1.f)


/* Texture */
static const std::wstring PRO_TEX_BACKGROUND	= TEXT("Prototype_Component_Texture_BackGround");
static const std::wstring PRO_TEX_LOGO1			= TEXT("Prototype_Component_Texture_Logo1");
static const std::wstring PRO_TEX_LOGO2			= TEXT("Prototype_Component_Texture_Logo2");
static const std::wstring PRO_TEX_TERRAIN		= TEXT("Prototype_Component_Texture_Terrain");
static const std::wstring PRO_TEX_SKY			= TEXT("Prototype_Component_Texture_Sky");


/* Component */
static const std::wstring PRO_COM_FSM			= TEXT("Prototype_Component_FSM");
static const std::wstring PRO_COM_VI_RECT		= TEXT("Prototype_Component_VIBuffer_Rect");
static const std::wstring PRO_COM_VI_CUBE		= TEXT("Prototype_Component_VIBuffer_Cube");
static const std::wstring PRO_COM_VI_TERRAIN	= TEXT("Prototype_Component_VIBuffer_Terrain");


/* Model */
static const std::wstring PRO_MODEL_FIONA		= TEXT("Prototype_Component_Model_Fiona");
static const std::wstring PRO_MODEL_FORK		= TEXT("Prototype_Component_Model_ForkLift");
static const std::wstring PRO_MODEL_STRIFE		= TEXT("Prototype_Component_Model_Test");
static const std::wstring PRO_MODEL_LGUN		= TEXT("Prototype_Component_Model_Gun_Left");
static const std::wstring PRO_MODEL_RGUN		= TEXT("Prototype_Component_Model_Gun_Right");


/* Shader */
static const std::wstring PRO_SHADER_POS		= TEXT("Prototype_Component_Shader_VtxPosTex");
static const std::wstring PRO_SHADER_NOR		= TEXT("Prototype_Component_Shader_VtxNorTex");
static const std::wstring PRO_SHADER_CUBE		= TEXT("Prototype_Component_Shader_VtxCube");
static const std::wstring PRO_SHADER_MESH		= TEXT("Prototype_Component_Shader_VtxMesh");
static const std::wstring PRO_SHADER_ANIM		= TEXT("Prototype_Component_Shader_VtxAnimMesh");


/* GameObject */
static const std::wstring PRO_OBJ_BACK			= TEXT("Prototype_GameObject_BackGround");
static const std::wstring PRO_OBJ_TERRAIN		= TEXT("Prototype_GameObject_Terrain");
static const std::wstring PRO_OBJ_SKY			= TEXT("Prototype_GameObject_Sky");
static const std::wstring PRO_OBJ_MONSTER		= TEXT("Prototype_GameObject_Monster");
static const std::wstring PRO_OBJ_PLAYER		= TEXT("Prototype_GameObject_Player");
static const std::wstring PRO_OBJ_BODY			= TEXT("Prototype_GameObject_Player_Body");
static const std::wstring PRO_OBJ_CAM_FREE		= TEXT("Prototype_GameObject_Camera_Free");
static const std::wstring PRO_OBJ_CAM_3RD		= TEXT("Prototype_GameObject_TP_Camera");
static const std::wstring PRO_OBJ_WEAPON		= TEXT("Prototype_GameObject_Weapon");


//static const std::wstring PRO_OBJ_BACK = TEXT("Prototype_Component_Texture_BackGround");
//static const std::wstring PRO_OBJ_BACK = TEXT("Prototype_Component_Texture_BackGround");
//static const std::wstring PRO_OBJ_BACK = TEXT("Prototype_Component_Texture_BackGround");
//static const std::wstring PRO_OBJ_BACK = TEXT("Prototype_Component_Texture_BackGround");






enum PLAYER_ANIMLIST
{
	AIM_IDLE = 0,
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