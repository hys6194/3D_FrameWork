
#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4 g_vLightDir;
float4 g_vLightDiffuse;
float4 g_vLightAmbient;
float4 g_vLightSpecular;

texture2D g_DiffuseTexture;
//texture2D g_DiffuseTexture[2];
texture2D g_MaskTexture;
texture2D g_BrushTexture;

float4 g_vMtrlAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
float4 g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

float4 g_vCamPosition;


float4 g_vBrushPos = float4(40.f, 0.f, 20.f, 1.f);
float  g_fBrushRange = 5.f;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};



PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord * 30.f);
    
    //float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
    float fShade = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));
    
    vector vLook = In.vWorldPos - g_vCamPosition;
    vector vReflect = reflect(normalize(g_vLightDir), In.vNormal);
    
    float fSpecular = pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 50.f);
    
    Out.vColor = g_vLightDiffuse * vDiffuse * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
        + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    Out.vColor.a = 0.5f;
    
    return Out;
}

PS_OUT PS_MAIN1(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord * 30.f);
    
    float fShade = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));
    
    vector vLook = In.vWorldPos - g_vCamPosition;
    vector vReflect = reflect(normalize(g_vLightDir), In.vNormal);
    
    float fSpecular = pow(saturate(normalize(vReflect)), 50.f);
    
    Out.vColor = g_vLightDiffuse * vDiffuse * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
        + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}

//PS_OUT PS_MAIN2(PS_IN In)
//{
//    // 스플래팅 셰이더
//    PS_OUT Out = (PS_OUT) 0;
    
//    vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);
//    vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexcoord * 30.f);
    
//    vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
//    vector vBrush = 0.f;
        
//    if (g_vBrushPos.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fBrushRange &&
//        g_vBrushPos.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fBrushRange)
//    {
//        float2 vTexcoord;
        
//        vTexcoord.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
//        vTexcoord.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);
        
//        vBrush = g_BrushTexture.Sample(LinearSampler, vTexcoord);
//    }
    
//    vector vMtrlDiffuse = vDestDiffuse * vMask + vSourDiffuse * (1.f - vMask) + vBrush;
    
//    //float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
//    float fShade = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));
    
//    vector vLook = In.vWorldPos - g_vCamPosition;
//    vector vReflect = reflect(normalize(g_vLightDir), In.vNormal);
    
//    float fSpecular = pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 50.f);
    
//    Out.vColor = g_vLightDiffuse * vMtrlDiffuse * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
//        + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

//    return Out;
//}


PS_OUT PS_MAIN3(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector v1 = { 1.f, 1.f, 1.f, 1.f };
    Out.vColor = v1;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass DefaultPass1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN1();
    }

    pass DefaultPass2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN1();

    }

    pass DefaultPass3
    {
        SetRasterizerState(RS_Wireframe);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN3();

    }
}



