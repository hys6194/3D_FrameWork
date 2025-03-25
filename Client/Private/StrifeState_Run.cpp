#include "StrifeState_Run.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"

#include "GameInstance.h"

CStrifeState_Run::CStrifeState_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
    : CState{ pDevice , pContext, pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT CStrifeState_Run::Enter_State()
{
    Set_CurAnimation();

    return S_OK;
}

void CStrifeState_Run::PriorityUpdate_State(_float fTimeDelta)
{
    m_iKeyState = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerKeyState();

    if(m_iKeyState & CPlayer::KEY_SHIFT)
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_DASH);
        return;
    }

    else if (m_iKeyState & CPlayer::KEY_LB)
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_SHOOT);
        return;
    }

    PlayerMove(fTimeDelta);
  
}

void CStrifeState_Run::Update_State(_float fTimeDelta)
{   
    Update_Animation(fTimeDelta);
}

void CStrifeState_Run::LateUpdate_State(_float fTimeDelta)
{
    /*if (false == m_pModelCom->Get_Interpolate())
        m_pModelCom->Reset_PreAnimation();*/
}

HRESULT CStrifeState_Run::Exit_State()
{
    Set_PreAnimation();
    return S_OK;
}

void CStrifeState_Run::Set_CurAnimation()
{
    m_pModelCom = dynamic_cast<CBody_Player*>(m_pAnimOwner)->Get_Model();

    m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::RUN, true);
}

void CStrifeState_Run::Update_Animation(_float fTimeDelta)
{
    if (0 != m_pModelCom->Get_PreAnimIndex()
        && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_pModelCom->Play_Animation(fTimeDelta);
}

void CStrifeState_Run::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();

    m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::RUN);
}

void CStrifeState_Run::PlayerMove(_float fTimeDelta)
{
    switch (m_iKeyState)
    {

    case (CPlayer::KEY_DOWN | CPlayer::KEY_LEFT):
        m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(-135.f));
        m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case (CPlayer::KEY_UP | CPlayer::KEY_LEFT):
        m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(-45.f));
        m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case(CPlayer::KEY_UP | CPlayer::KEY_RIGHT):
        m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(45.f));
        m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case(CPlayer::KEY_RIGHT | CPlayer::KEY_DOWN):
        m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(135.f));
        m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case CPlayer::KEY_DOWN:
        m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(180.f));
        m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case CPlayer::KEY_LEFT:
        m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(-90.f));
        m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case CPlayer::KEY_RIGHT:
        m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(90.f));
        m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case CPlayer::KEY_UP:
        m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(0.f));
        m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    default:
        dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_IDLE);
        break;
    }
}

CStrifeState_Run* CStrifeState_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
{
    CStrifeState_Run* pInstance = new CStrifeState_Run(pDevice, pContext, pOwner, pAnimOwner);

    if (nullptr == pOwner)
    {
        MSG_BOX("Failed To Created : StrifeState_Run");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CStrifeState_Run::Free()
{
    __super::Free();
}
