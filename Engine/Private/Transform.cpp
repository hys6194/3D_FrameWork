#include "Transform.h"
#include "Shader.h"

Transform::Transform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component{ pDevice, pContext }
{
}

Transform::Transform(const Transform& Prototype)
    :Component{ Prototype }
    , m_f4WorldMatrix{ Prototype.m_f4WorldMatrix }
{
}

HRESULT Transform::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_f4WorldMatrix, XMMatrixIdentity());

    return S_OK;
}

HRESULT Transform::Initialize(void* pArg)
{
    if(nullptr != pArg)
    {
        TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

        m_fSpeedPerSec = pDesc->fSpeedPerSec;
        m_fRotationPerSec = pDesc->fRotationPerSec;
    }

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
    _vector vPos = Get_State(STATE_POS);

    // Look 벡터 가져오기
    _vector vLook = Get_State(STATE_LOOK);

    // Look 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다
    Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT Transform::Go_Backward(_float fTimeDelta)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POS);

    // Look 벡터 가져오기
    _vector vLook = Get_State(STATE_LOOK);

    // Look 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다
    Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT Transform::Go_Right(_float fTimeDelta)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POS);

    // Right 벡터 가져오기
    _vector vRight = Get_State(STATE_RIGHT);

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 Position에 대입한다
    Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT Transform::Go_Left(_float fTimeDelta)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POS);

    // Right 벡터 가져오기
    _vector vRight = Get_State(STATE_RIGHT);

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다
    Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT Transform::Jump(_float fTimeDelta)
{
    _vector vPos = Get_State(STATE_POS);
    

    return S_OK;
}

HRESULT Transform::LookAt(_vector vAt)
{
    // 카메라에서 쓰이는 함수

    _float3 vScale = Update_Scale();
    _vector vPos = Get_State(STATE_POS);
    _vector vRight = Get_State(STATE_RIGHT);
    _vector vUp = Get_State(STATE_UP);
    _vector vLook = Get_State(STATE_LOOK);

    Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
    Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
    Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);

    return S_OK;
}

void Transform::Turn(_fvector vAxis, _float fTimeDelta)
{
    _matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

    _vector		vRight = Get_State(STATE_RIGHT);
    _vector		vUp = Get_State(STATE_UP);
    _vector		vLook = Get_State(STATE_LOOK);

    Set_State(STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
    Set_State(STATE_UP, XMVector4Transform(vUp, RotationMatrix));
    Set_State(STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void Transform::Rotation(_fvector vAxis, _float fRadian)
{
    _matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

    _float3		vScaled = Update_Scale();

    _vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
    _vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
    _vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

    Set_State(STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
    Set_State(STATE_UP, XMVector4Transform(vUp, RotationMatrix));
    Set_State(STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void Transform::SetUp_Scaled(_float fScaleX, _float fScaleY, _float fScaleZ)
{
    _vector			vRight = Get_State(STATE_RIGHT);
    _vector			vUp = Get_State(STATE_UP);
    _vector			vLook = Get_State(STATE_LOOK);

    Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * fScaleX);
    Set_State(STATE_UP, XMVector3Normalize(vUp) * fScaleY);
    Set_State(STATE_LOOK, XMVector3Normalize(vLook) * fScaleZ);
}

HRESULT Transform::Bind_SR(const _char* pConstantName, class Shader* pShader)
{
    if (nullptr == pShader)
        return E_FAIL;

    return pShader->Bind_Matrix(pConstantName, &m_f4WorldMatrix);
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