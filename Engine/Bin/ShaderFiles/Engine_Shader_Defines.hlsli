sampler LinearSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler PointSampler = sampler_state
{
    filter = min_mag_mip_point;
    AddressU = WRAP;
    AddressV = WRAP;
};

RasterizerState RS_Default
{
    FillMode = SOLID;
    CullMode = BACK;
    FrontCounterClockwise = false;
};

RasterizerState RS_Wireframe
{
    FillMode = WIREFRAME;
    CullMode = BACK;
    FrontCounterClockwise = false;
};

RasterizerState RS_Cull_None
{
    FillMode = SOLID;
    CullMode = FRONT;
    FrontCounterClockwise = false;
};

DepthStencilState DSS_Default
{
    DepthEnable = true;
    DepthWriteMask = ALL;
    DepthFunc = LESS_EQUAL;
};

DepthStencilState DSS_None
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
};


BlendState BS_Default
{
    BlendEnable[0] = false;
};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = true;

    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = ADD;
};
