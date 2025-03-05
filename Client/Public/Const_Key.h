#pragma once

#include <string>
#include <winnt.h>

#define TIME60			TEXT("Timer_60")
#define AXIS_X			XMVectorSet(1.f, 0.f, 0.f, 0.f)
#define AXIS_Y			XMVectorSet(0.f, 1.f, 0.f, 0.f)
#define AXIS_Z			XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define AXIS_W			XMVectorSet(0.f, 0.f, 0.f, 1.f)

static const std::wstring PRO_TEX_BACKGROUND = TEXT("Prototype_Component_Texture_BackGround");
static const std::wstring PRO_TEX_LOGO1 = TEXT("Prototype_Component_Texture_Logo1");
static const std::wstring PRO_TEX_TERRAIN = TEXT("Prototype_Component_Texture_Terrain");
static const std::wstring PRO_TEX_SKY = TEXT("Prototype_Component_Texture_Sky");


static const std::wstring PRO_COM_FSM = TEXT("Prototype_Component_FSM");
static const std::wstring PRO_COM_VI_RECT = TEXT("Prototype_Component_VIBuffer_Rect");
static const std::wstring PRO_COM_VI_CUBE = TEXT("Prototype_Component_VIBuffer_Cube");
static const std::wstring PRO_COM_VI_TERRAIN = TEXT("Prototype_Component_VIBuffer_Terrain");


static const std::wstring PRO_MODEL_FIONA = TEXT("Prototype_Component_Model_Fiona");
static const std::wstring PRO_MODEL_FORK = TEXT("Prototype_Component_Model_ForkLift");
static const std::wstring PRO_MODEL_STRIFE = TEXT("Prototype_Component_Model_Test");
static const std::wstring PRO_MODEL_LGUN = TEXT("Prototype_Component_Model_Gun_Left");
static const std::wstring PRO_MODEL_RGUN = TEXT("Prototype_Component_Model_Gun_Right");
//static const std::wstring PRO_COM_VI_MESH = TEXT("Prototype_Component_Texture_BackGround");
//static const std::wstring PRO_COM_VI_MESH = TEXT("Prototype_Component_Texture_BackGround");
//static const std::wstring PRO_COM_VI_MESH = TEXT("Prototype_Component_Texture_BackGround");
//static const std::wstring PRO_COM_VI_MESH = TEXT("Prototype_Component_Texture_BackGround");