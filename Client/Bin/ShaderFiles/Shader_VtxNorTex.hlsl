// 빛 연산을 처리하는 hlsl 파일
// 추후에 pass를 늘려서 빛연산 처리 방식을 달리한다

matrix      g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// 빛의 방향
float4      g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);

// 빛 색
float4      g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);

float4      g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
float4      g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);

texture2D   g_DiffuseTexture;
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
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

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

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    // 퐁 쉐이딩
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    
    
    return Out;
}

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    // 그림자 없이 출력
    //Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord * 30.f);
    
    // 퐁 셰이딩
    // vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord * 30.f);
    // 
    // float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);
    // 
    // Out.vColor = g_vLightDiffuse * vDiffuse * fShade;
    
    vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord * 30.f);
    
    // 0보다 작으면 0 return, 1 보다 크면 1 return
    float fShade = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));
    
    vector vLook = In.vWorldPos - g_vCamPosition;
    vector vReflect = reflect(normalize(g_vLightDir), In.vNormal);
    
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



