#include "StrifeState_Idle.h"
#include "Body_Player.h"
#include "Model.h"
#include "Player.h"

CStrifeState_Idle::CStrifeState_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
    : CState{ pDevice , pContext, pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT CStrifeState_Idle::Enter_State()
{
    Set_CurAnimation();

    return S_OK;
}

void CStrifeState_Idle::PriorityUpdate_State(_float fTimeDelta)
{
    m_iKeyState = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerKeyState();

	if (m_iKeyState & CPlayer::KEY_SHIFT)
	{
        dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_DASH);
        return;
	}

    else if (m_iKeyState & CPlayer::KEY_LB)
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_SHOOT);
        return;
    }


	// 아무 키나 눌렀을 때
    if (m_iKeyState & CPlayer::KEY_UP ||
             m_iKeyState & CPlayer::KEY_DOWN ||
             m_iKeyState & CPlayer::KEY_LEFT ||
             m_iKeyState & CPlayer::KEY_RIGHT)
	{
        dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_RUN);
        return;
	}

    
}

void CStrifeState_Idle::Update_State(_float fTimeDelta)
{   
    Update_Animation(fTimeDelta);
}

void CStrifeState_Idle::LateUpdate_State(_float fTimeDelta)
{

}

HRESULT CStrifeState_Idle::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CStrifeState_Idle::Set_CurAnimation()
{
    m_pModelCom = dynamic_cast<CBody_Player*>(m_pAnimOwner)->Get_Model();

    m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::IDLE, true);
}

void CStrifeState_Idle::Update_Animation(_float fTimeDelta)
{
    if (0 != m_pModelCom->Get_PreAnimIndex()
        && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_pModelCom->Play_Animation(fTimeDelta);
}

void CStrifeState_Idle::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::IDLE);
}

void CStrifeState_Idle::Check_KeyInput()
{
    if (m_pGameInstance->Key_Pressing(DIK_DOWN))
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Insert_KeyState(CPlayer::KEY_DOWN);
    }
    else if (!m_pGameInstance->Key_Pressing(DIK_DOWN))
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Delete_KeyState(!CPlayer::KEY_DOWN);
    }

    if (m_pGameInstance->Key_Pressing(DIK_UP))
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Insert_KeyState(CPlayer::KEY_UP);
    }
    else if (!m_pGameInstance->Key_Pressing(DIK_UP))
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Delete_KeyState(!CPlayer::KEY_UP);
    }

    if (m_pGameInstance->Key_Pressing(DIK_LEFT))
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Insert_KeyState(CPlayer::KEY_LEFT);
    }
    else if (!m_pGameInstance->Key_Pressing(DIK_LEFT))
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Delete_KeyState(!CPlayer::KEY_LEFT);
    }

    if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Insert_KeyState(CPlayer::KEY_RIGHT);
    }
    else if (!m_pGameInstance->Key_Pressing(DIK_RIGHT))
    {
        dynamic_cast<CPlayer*>(m_pOwner)->Delete_KeyState(!CPlayer::KEY_RIGHT);
    }
}

CStrifeState_Idle* CStrifeState_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
{
    CStrifeState_Idle* pInstance = new CStrifeState_Idle(pDevice, pContext, pOwner, pAnimOwner);

    if (nullptr == pOwner)
    {
        MSG_BOX("Failed To Created : StrifeState_Idle");
        Safe_Release(pInstance);
        return nullptr;
    }

    //pInstance->Set_Owner(pOwner);
    //Safe_AddRef(m_pOwner);

    return pInstance;
}

void CStrifeState_Idle::Free()
{
    __super::Free();
}
