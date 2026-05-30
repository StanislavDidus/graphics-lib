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
   float2 texcoord = input.TexCoord;

    if(input.Flip == 1)
    {
        texcoord.x = input.UV.z - (texcoord.x - input.UV.x) + input.UV.x;
    }
    else if(input.Flip == 2)
    {
        texcoord.y = input.UV.w - (texcoord.y - input.UV.y) + input.UV.y;
    }
    else if(input.Flip == 3)
    {
        texcoord.x = input.UV.z - (texcoord.x - input.UV.x) + input.UV.x;
        texcoord.y = input.UV.w - (texcoord.y - input.UV.y) + input.UV.y;
    }
    
    return input.Color / 255.0f * Texture.Sample(Sampler, texcoord);
}