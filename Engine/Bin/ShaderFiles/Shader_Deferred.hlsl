
#include "Engine_Shader_Defines.hlsli"

matrix      g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix      g_ViewMatrixInv, g_ProjMatrixInv;
texture2D   g_Texture;

vector      g_vLightDir;
vector      g_vLightPos;
float       g_fLightRange;
vector      g_vLightDiffuse;
vector      g_vLightAmbient;
vector      g_vLightSpecular;
texture2D   g_NormalTexture;

texture2D   g_DiffuseTexture;
texture2D   g_ShadeTexture;
texture2D   g_SpecularTexture;
vector      g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector      g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector      g_vCamPosition;

texture2D   g_DepthTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In)
{
   
    VS_OUT Out = (VS_OUT) 0;
    

    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    return Out;
}

struct PS_OUT_LIGHT
{
    float4 vShade : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out;
    
    float4 vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
    float4 vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 300.f;
    // 픽셀 포맷이 0과 1사이의 정규화된 값만 저장할 수 있어서
    // 노말 렌더 타겟을 그릴 때 0과 1의 사이의 값으로 저장했음
    // 이를 다시 -1과 1사이의 값으로ㄴ 보정해야 함
    // 0 == -1, 1 == 1
    float4 vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    vector vShade = saturate(max(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal)), 0.f) +
        (g_vLightAmbient * g_vMtrlAmbient));
    
    Out.vShade = vShade * g_vLightDiffuse;
    
    vector vReflect = reflect(normalize(g_vLightDir), vNormal);
    vector vWorldPos;
    
    /* 투영공간상의 좌표를구했다.*/
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x; /* 0.0f ~ 1.f */
    vWorldPos.w = 1.f;
    
    /* 뷰공간상의 좌표를구했다.*/
    vWorldPos *= fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    
    /* 월드공간상의 좌표를구했다.*/
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vLook = vWorldPos - g_vCamPosition;
    
    float fSpecular = pow(saturate(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f)), 50.f);
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out;
    
    float4 vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
    float4 vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 300.f;
    float4 vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    vector vWorldPos;
    
    /* 투영공간상의 좌표를구했다.*/
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    
    // 왜 x에 담겨져있는지 다시 고민
    vWorldPos.z = vDepthDesc.x; /* 0.0f ~ 1.f */
    vWorldPos.w = 1.f;
    
    /* 뷰공간상의 좌표를구했다.*/
    vWorldPos *= fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    
    /* 월드공간상의 좌표를구했다.*/
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    
    float4 vLightDir = vWorldPos - g_vLightPos;
    
    float fDistance = length(vLightDir);
    
    /* 1 ~ 0.f */
    float fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);
    
    vector vShade = saturate(max(dot(normalize(vLightDir) * -1.f, normalize(vNormal)), 0.f) +
        (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
    
    Out.vShade = vShade * g_vLightDiffuse;
    
    vector vReflect = reflect(normalize(vLightDir), vNormal);
    
  
    vector vLook = vWorldPos - g_vCamPosition;
    
    float fSpecular = pow(saturate(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f)), 50.f) * fAtt;
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
    PS_OUT Out;
    
    float4 vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (0.0f == vDiffuse.a)
        discard;
    
    float4 vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vDiffuse * vShade + vSpecular;
    
    return Out;
}


/* 하드웨어 장치의 지원여부에 따라 다른 버젼의 셰이더를 빌드할 수 있도록 추가적으로 테크니커를 만들수 있다.*/ 
technique11 DefaultTechnique
{
    /* 내가 적용하고 싶은 셰이더 기법들의 캡슐화 */
    /* 이 패스에서 사용할 여러가지 셰이더의 진입점 함수를 지정한다. */

    /* 일반적으로 렌더링한다 : 빛연산 + 스펙큘러 + 림라이트 + 그림자 + ssao */ 
    pass Debug
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Light_Directional
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
    }

    pass Light_Point
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
    }

    pass Deferred
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
    }



}

