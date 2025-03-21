
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

/* 모델 전체의 뼈정보(x) */
/* 특정 메시에게 영향을 주는 뼈들의 정보(o) */
float4x4 g_BoneMatrices[512];

float4      g_vLightDir;
float4      g_vLightDiffuse;
float4      g_vLightAmbient;
float4      g_vLightSpecular;

Texture2D   g_DiffuseTexture;
float4      g_vMtrlAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
float4      g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

float4      g_vCamPosition;

sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;    
    float3 vTangent : TANGENT;
    
    uint4 vBlendIndex0 : BLENDINDEX0;
    uint4 vBlendIndex1 : BLENDINDEX1;
    float4 vBlendWeight0 : BLENDWEIGHT0;
    float4 vBlendWeight1 : BLENDWEIGHT1;

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
    VS_OUT Out = (VS_OUT)0;    

    float weightSum = 1.f;
   
    uint indices[8];
    indices[0] = In.vBlendIndex0.x;
    indices[1] = In.vBlendIndex0.y;
    indices[2] = In.vBlendIndex0.z;
    indices[3] = In.vBlendIndex0.w;
    indices[4] = In.vBlendIndex1.x;
    indices[5] = In.vBlendIndex1.y;
    indices[6] = In.vBlendIndex1.z;
    indices[7] = In.vBlendIndex1.w;
    
    float weights[8];
    weights[0] = In.vBlendWeight0.x;
    weights[1] = In.vBlendWeight0.y;
    weights[2] = In.vBlendWeight0.z;
    weights[3] = In.vBlendWeight0.w;
    weights[4] = In.vBlendWeight1.x;
    weights[5] = In.vBlendWeight1.y;
    weights[6] = In.vBlendWeight1.z;
    for (int j = 0; j < 7; ++j)
    {
        weightSum -= weights[j];
    }
    weights[7] = weightSum;

    float3 vPosModel = float3(0.f, 0.f, 0.f);
    float3 vNormal = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < 8; ++i)
    {
        float4x4 BoneMatrix = g_BoneMatrices[indices[i]];
        vPosModel += weights[i] * mul(float4(In.vPosition, 1.f), BoneMatrix);
        vNormal += weights[i] * mul(In.vNormal, (float3x3) BoneMatrix);
    }
     
    float4x4 matWV, matWVP;    
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);    
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(float4(vPosModel, 1.0f), matWVP);
    Out.vNormal = normalize(mul(float4(vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    
    Out.vWorldPos = mul(float4(vPosModel, 1.f), g_WorldMatrix);
    
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
    
    float4 vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vDiffuse.a < 0.3f)
        discard;
    
    //float fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
    float fShade = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));
    
    float4 vLook = In.vWorldPos - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), In.vNormal);
    
    float fSpecular = pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 50.f);
    
    Out.vColor = g_vLightDiffuse * vDiffuse * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
        + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}

technique11 DefaultTechnique
{ 
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}



