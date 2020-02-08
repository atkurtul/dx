struct psin
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
    float2 tex : TEXCOORD;
};

Texture2D txt : register(t0);
SamplerState sample;
float4 main(psin input) : SV_TARGET
{
    float4 s = txt.Sample(sample, input.tex);
    s.xyz *= input.col;
    return s;
}