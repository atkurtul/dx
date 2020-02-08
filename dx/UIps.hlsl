
struct psin
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D txt : register(t0);
SamplerState sample;

float4 main(psin input) : SV_TARGET
{
    float a = txt.Sample(sample, input.tex).a;
    return float4(1, 1, 1, a);
}
