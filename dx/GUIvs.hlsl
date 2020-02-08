
cbuffer cb : register(b1)
{
    float2 ws;
}

cbuffer cb : register(b2)
{
    float4 xf;
}

cbuffer cb : register(b3)
{
    float3 color;
}

struct vsout
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
    float2 tex : TEXCOORD;
};


vsout main(float2 input : POSITION, float2 tex : TEXCOORD) 
{
    vsout output;
    output.pos      = float4(((xf.zw * input.xy + xf.xy) * 2 - ws) * ws - 1, 0, 1);
    output.col      = color;
    output.tex      = tex;
    return output;
}