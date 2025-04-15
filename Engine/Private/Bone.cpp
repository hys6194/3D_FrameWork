#include "Bone.h"
#include "GameObject.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
    strcpy_s(m_szName, pAINode->mName.data);
    
    m_iParentBoneIndex = iParentBoneIndex;

    memcpy(&m_matTransform, &pAINode->mTransformation, sizeof(_float4x4));

    XMStoreFloat4x4(&m_matTransform,
        XMMatrixTranspose(XMLoadFloat4x4(&m_matTransform)));

    XMStoreFloat4x4(&m_matCombinedTransform,
        XMMatrixIdentity());

    return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<class CBone*>& Bones, const _float4x4* pPreTransformMatrix)
{
    if (-1 == m_iParentBoneIndex)
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) *
            XMLoadFloat4x4(pPreTransformMatrix));

    else
       XMStoreFloat4x4(&m_matCombinedTransform,
           XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));

}

void CBone::Update_Combine_RootMatrix(const vector<class CBone*>& Bones, const _float4x4* pPreTransformMatrix, _uint iCurKeyFrameIndex,CGameObject* pObject)
{
    if (-1 == m_iParentBoneIndex)
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) *
            XMLoadFloat4x4(pPreTransformMatrix));


    // 원래 이렇게 하면 안됨
    // 다음에는 함수 만들 때 루트 본의 이름을 받아오고 이름을 확인한 후 그 뼈의 매트릭스에 접근하는 것으로 하자
    else if (1 == m_iParentBoneIndex)
    {
        _float4x4 f4PreTransform;
        _float4 vPreTrans, vDelta, vNonTrans {0.f,0.f,0.f,1.f};
    
        // 루트 본의 매트릭스를 제일 부모의 매트릭스에 곱하기 전의 로컬(?) 매트릭스 꺼내오기
        XMStoreFloat4x4(&f4PreTransform,
            XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));

        memcpy(&vPreTrans, &f4PreTransform.m[3][0], sizeof(_float4));

        memcpy(&f4PreTransform.m[3][0], &vNonTrans, sizeof(_float4));

        // 이동량 계산
        XMStoreFloat4(&vDelta, XMVectorSetW(XMLoadFloat4(&vPreTrans) - XMLoadFloat4(&m_vPreDelta), 1.f));
        
        // 만약 원점 근처에서 시작한 경우
        if (0 == iCurKeyFrameIndex &&
            XMVector4NearEqual(XMLoadFloat4(&vNonTrans), XMLoadFloat4(&vDelta)
                , XMVectorSet(1.f, 1.f, 1.f, 1.f)))
        {
            m_vCurDelta = vDelta;
            m_vPreDelta = vPreTrans;
        }
        
        // 원점에서 많이 벗어난 경우 -> 원점 시작으로 판단하지 않음
        else if (0 == iCurKeyFrameIndex &&
            !XMVector4NearEqual(XMLoadFloat4(&vNonTrans), XMLoadFloat4(&vDelta)
                , XMVectorSet(3.f, 3.f, 3.f, 3.f)))
        {
            m_vCurDelta = vNonTrans;
            m_vPreDelta = vDelta;
        }
            
        else if(0 != iCurKeyFrameIndex)
        {
            m_vCurDelta = vDelta;
            m_vPreDelta = vPreTrans;
        }
        // 변화량을 담아서 다른 클래스에서 사용할 수 있게 저장
        // 추후 Transform에서 원하는 값만 가져와서 사용
        //m_vCurDelta = vDelta;

        // 이전 값을 저장
        //m_vPreDelta = vPreTrans;

        // 회전량 죽임
        _vector vScale, vRotation, vTranslation;

        _matrix matPreTransform = {};

        // 원본 행렬 분해 (스케일, 회전, 위치)
        XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(&f4PreTransform));

        vRotation = XMQuaternionIdentity();

        _matrix matNoRotation = XMMatrixAffineTransformation(vScale, XMVectorZero(), vRotation, vTranslation);

        _matrix matFixRotation = XMMatrixRotationX(XMConvertToRadians(90.f));

        matPreTransform = matFixRotation * matNoRotation;

        XMStoreFloat4x4(&m_matCombinedTransform, matPreTransform);




        //_float4 fDebug{};
        //XMStoreFloat4(&fDebug, XMLoadFloat4(&vDelta));
        //TCHAR debugMessage[256];
        //_stprintf_s(debugMessage, _T("Debug_Value: x = %.6f, y = %.6f, z = %.6f, w = %.6f\n"), 
        //    vDelta.x, vDelta.y, vDelta.z, vDelta.w);
        //OutputDebugString(debugMessage);

//        //TCHAR debugMessage1[256];
        //_stprintf_s(debugMessage1, _T("Debug_Value: x = %.6f\n"),
        //    fAngle);
        //OutputDebugString(debugMessage1);
    
    }


    else
    {   
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));
    }

}

HRESULT CBone::Initialize(ifstream& _InStream)
{
    _uint iReadByte = { };
    _char szBoneName[MAX_PATH] = { };

    _InStream.read(reinterpret_cast<char*>(&iReadByte), sizeof(_uint));
    _InStream.read(reinterpret_cast<char*>(szBoneName), sizeof(_char) * iReadByte);
    strcpy_s(m_szName, szBoneName);

    _InStream.read(reinterpret_cast<char*>(&m_matTransform), sizeof(_float4x4));
    _InStream.read(reinterpret_cast<char*>(&m_iParentBoneIndex), sizeof(_int));

    return S_OK;
}

_bool CBone::Save_Bone(ofstream& _OpenStream)
{
    if (!_OpenStream)
        return false;

    _uint iSize = sizeof(m_szName);
    _OpenStream.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));
    _OpenStream.write(m_szName, iSize);

    _OpenStream.write(reinterpret_cast<const char*>(&m_matTransform), sizeof(_float4x4));

    _OpenStream.write(reinterpret_cast<const char*>(&m_iParentBoneIndex), sizeof(_int));

    return true;
}

CBone* CBone::Create(ifstream& _InStream)
{
    CBone* pInstance = new CBone();

    if (FAILED(pInstance->Initialize(_InStream)))
    {
        MSG_BOX("Failed To Created : CBone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CBone* CBone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
    CBone* pInstance = new CBone();

    if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
    {
        MSG_BOX("Failed To Created : Bone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CBone* CBone::Clone()
{
    return new CBone(*this);
}


void CBone::Free()
{
}
