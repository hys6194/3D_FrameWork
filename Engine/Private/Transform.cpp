#include "Transform.h"
#include "Shader.h"
#include "Bone.h"

#include "Navigation.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }
{
}

CTransform::CTransform(const CTransform& Prototype)
    :CComponent{ Prototype }
    , m_f4WorldMatrix{ Prototype.m_f4WorldMatrix }
{
}

HRESULT CTransform::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_f4WorldMatrix, XMMatrixIdentity());

    return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
    if(nullptr != pArg)
    {
        TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

        m_fSpeedPerSec = pDesc->fSpeedPerSec;
        m_fRotationPerSec = pDesc->fRotationPerSec;
    }

    return S_OK;
}

_float3 CTransform::Update_Scale()
{
    return _float3(
        XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
        XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
        XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
    
}

HRESULT CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNavigation)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POS);

    // Look 벡터 가져오기
    _vector vLook = Get_State(STATE_LOOK);

    // Look 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Go_Backward(_float fTimeDelta, CNavigation* pNavigation)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POS);

    // Look 벡터 가져오기
    _vector vLook = Get_State(STATE_LOOK);

    // Look 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Go_Right(_float fTimeDelta, CNavigation* pNavigation)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POS);

    // Right 벡터 가져오기
    _vector vRight = Get_State(STATE_RIGHT);

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 Position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Go_Left(_float fTimeDelta, CNavigation* pNavigation)
{
    // 위치 가져오기
    _vector vPos = Get_State(STATE_POS);

    // Right 벡터 가져오기
    _vector vRight = Get_State(STATE_RIGHT);

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Jump(_float fTimeDelta)
{
    _vector vPos = Get_State(STATE_POS);
    

    return S_OK;
}

HRESULT CTransform::LookAt(_vector vAt)
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

HRESULT CTransform::Dash(_float4 fDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);
    _vector vLook = Get_State(STATE_LOOK);
    _vector vDelta = XMVectorSet(fDelta.z, 0.f, fDelta.z, 0.f);

    vPos += XMVector4Normalize(vLook) * vDelta;

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Avoid(_float4 fDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);
    _vector vRight = Get_State(STATE_RIGHT);
    _vector vDelta = XMVectorSet(fDelta.x * 2, 0.f, fDelta.x * 2, 0.f);

    vPos += XMVector4Normalize(vRight) * vDelta;

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
    _matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fTimeDelta * m_fRotationPerSec);

    _vector		vRight = Get_State(STATE_RIGHT);
    _vector		vUp = Get_State(STATE_UP);
    _vector		vLook = Get_State(STATE_LOOK);

    Set_State(STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
    Set_State(STATE_UP, XMVector4Transform(vUp, RotationMatrix));
    Set_State(STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

_bool CTransform::Turn_ToTarget(_fvector vAxis, _float fTimeDelta, _vector vTargetToDir)
{
    _vector		vRight         = Get_State(STATE_RIGHT);
    _vector		vUp            = Get_State(STATE_UP);
    _vector		vLook          = Get_State(STATE_LOOK);

    _vector vAxisBase = vAxis;

    _float      fY = XMVectorGetY(XMVector3Cross(vLook, vTargetToDir));
    _float      fDot = acosf(XMVectorGetX(XMVector3Dot(vLook, vTargetToDir)));

    if (0 > fY)
        vAxisBase = XMVectorSetY(vAxis, -1.f);

    _matrix		RotationMatrix = XMMatrixRotationAxis(vAxisBase, fTimeDelta * m_fRotationPerSec);
                           
    Set_State(STATE_RIGHT,     XMVector4Transform(vRight,   RotationMatrix));
    Set_State(STATE_UP,        XMVector4Transform(vUp,      RotationMatrix));
    Set_State(STATE_LOOK,      XMVector4Transform(vLook,    RotationMatrix));


    // 방향과 타겟으로 향한 벡터와 비슷하다면 종료하게 끔
    if (XMVector4NearEqual(vLook, vTargetToDir, XMVectorSet(0.01f, 0.f, 0.01f, 0.f)))
        return true;
    else
        return false;
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
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

HRESULT CTransform::Move_Straight(_float fTimeDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);

    // 움직일 벡터 설정
    _vector vDir = AXIS_Z;

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Move_Backward(_float fTimeDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);

    // 움직일 벡터 설정
    _vector vDir = AXIS_Z;

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos -= XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Move_Right(_float fTimeDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);

    // 움직일 벡터 설정
    _vector vDir = AXIS_X;

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Move_Left(_float fTimeDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);

    // 움직일 벡터 설정
    _vector vDir = AXIS_X;

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos -= XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Move_Left_Up(_float fTimeDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);

    // 움직일 벡터 설정
    _vector vDir = -AXIS_XMZ;

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Move_Right_Up(_float fTimeDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);

    // 움직일 벡터 설정
    _vector vDir = AXIS_XZ;

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Move_Left_Down(_float fTimeDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);

    // 움직일 벡터 설정
    _vector vDir = -AXIS_XZ;

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

HRESULT CTransform::Move_Right_Down(_float fTimeDelta, CNavigation* pNavigation)
{
    _vector vPos = Get_State(STATE_POS);

    // 움직일 벡터 설정
    _vector vDir = AXIS_XMZ;

    // Right 벡터의 방향으로 fSpeedPerSec의 값만큼 fTimeDelta 초 만큼 이동한다
    vPos += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

    // 계산한 Vector를 position에 대입한다

    if (nullptr == pNavigation ||
        true == pNavigation->isMove(vPos))
        Set_State(STATE_POS, vPos);

    return S_OK;
}

void CTransform::SetUp_Scaled(_float fScaleX, _float fScaleY, _float fScaleZ)
{
    _vector			vRight = Get_State(STATE_RIGHT);
    _vector			vUp = Get_State(STATE_UP);
    _vector			vLook = Get_State(STATE_LOOK);

    Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * fScaleX);
    Set_State(STATE_UP, XMVector3Normalize(vUp) * fScaleY);
    Set_State(STATE_LOOK, XMVector3Normalize(vLook) * fScaleZ);
}

HRESULT CTransform::Bind_SR(const _char* pConstantName, class CShader* pShader)
{
    if (nullptr == pShader)
        return E_FAIL;

    return pShader->Bind_Matrix(pConstantName, &m_f4WorldMatrix);
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTransform* pInstance = new CTransform(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Transform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
    CComponent* pInstance = new CTransform(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Transform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTransform::Free()
{
    __super::Free();
}