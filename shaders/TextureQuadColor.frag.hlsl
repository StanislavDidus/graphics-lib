Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

struct Input
{
    float2 TexCoord : TEXCOORD0;
    float4 Color : TEXCOORD1;
    uint Flip : TEXCOORD2;
    float4 UV : TEXCOORD3;
};

float4 main(Input input) : SV_TARGET0
{
    float2 uv = input.TexCoord;
    return input.Color / 255.0f * Texture.Sample(Sampler, uv);
}