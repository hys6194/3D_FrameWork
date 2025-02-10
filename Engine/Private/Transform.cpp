#include "Transform.h"

Transform::Transform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component{ m_pDevice, m_pContext}
{
}

Transform::Transform(const Transform& Prototype)
    :Component{ Prototype }
{
}

HRESULT Transform::Initialize_Prototype()
{
    return S_OK;
}

HRESULT Transform::Initialize(void* pArg)
{
    TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

    m_fSpeedPerSec = pDesc->fSpeedPerSec;
    m_fRotationPerSec = pDesc->fRotationPerSec;

    return S_OK;
}

_float3 Transform::Update_Scale()
{
    return _float3(
        XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
        XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
        XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
    
}

HRESULT Transform::Go_Straight(_float fTimeDelta)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POSITION);

    // Look 벡터 가져오기
    _vector vLook = Get_State(STATE_LOOK);

    // Look 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다
    Set_State(STATE_POSITION, vPos);

    return S_OK;
}

HRESULT Transform::Go_Backward(_float fTimeDelta)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POSITION);

    // Look 벡터 가져오기
    _vector vLook = Get_State(STATE_LOOK);

    // Look 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다
    Set_State(STATE_POSITION, vPos);

    return S_OK;
}

HRESULT Transform::Go_Right(_float fTimeDelta)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POSITION);

    // Right 벡터 가져오기
    _vector vRight = Get_State(STATE_RIGHT);

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 Position에 대입한다
    Set_State(STATE_POSITION, vPos);

    return S_OK;
}

HRESULT Transform::Go_Left(_float fTimeDelta)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POSITION);

    // Right 벡터 가져오기
    _vector vRight = Get_State(STATE_RIGHT);

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다
    Set_State(STATE_POSITION, vPos);

    return S_OK;
}

HRESULT Transform::LookAt(_vector vAt)
{
    // 카메라에서 쓰이는 함수

    _float3 vScale = Update_Scale();
    _vector vPos = Get_State(STATE_POSITION);
    _vector vRight = Get_State(STATE_RIGHT);
    _vector vUp = Get_State(STATE_UP);
    _vector vLook = Get_State(STATE_LOOK);

    Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
    Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
    Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);

    return S_OK;
}

Transform* Transform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Transform* pInstance = new Transform(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Transform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* Transform::Clone(void* pArg)
{
    Component* pInstance = new Transform(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Transform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Transform::Free()
{
    __super::Free();
}