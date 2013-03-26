// Programmed by Bryce Mehring

// Variables Start

// Animation
uniform extern float2 gOffset; 

// Transformations
uniform extern float4x4 gViewProj;
uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldViewProj;
uniform extern float4x4 gWorldInverseTranspose;

// Textures
extern texture gTex;
uniform extern float gXScale;
uniform extern float gYScale;
uniform extern int gSpriteWidth;
uniform extern int gSpriteHeight;
uniform extern int gCurrentFrame;

// Color
uniform extern float4 gColor;
// Variables End

struct VSIn
{
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
};

struct VSStreamIn
{
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float dx : TEXCOORD1;
	float dy : TEXCOORD2;
};

struct VSOut
{
	float4 posH : POSITION0;
	float2 tex : TEXCOORD0;
	float3 interpPos : TEXCOORD1;
};

struct TextVSOut
{
	float4 posH : POSITION0;
	float2 tex : TEXCOORD0;
};


sampler TexS = sampler_state
{
	texture = <gTex>;
	MinFilter = linear;
	MagFilter = linear;
	MipFilter = linear;

	AddressU = wrap;
	AddressV = wrap;

	MaxAnisotropy = 16;
};

sampler TextSampler = sampler_state
{
	texture = <gTex>;
	MinFilter = Anisotropic;
	MagFilter = linear;
	MipFilter = point;
	

	AddressU = wrap;
	AddressV = wrap;

	MaxAnisotropy = 16;
};


VSOut SpriteVS(VSIn IN)
{
	VSOut OUT = (VSOut)0;

	OUT.posH = mul(float4(IN.pos,1.0f),gWorldViewProj);
	OUT.tex = IN.tex;

	return OUT;
}

TextVSOut TextVS(in uniform float fOffset, float3 posL : POSITION0, float2 tex : TEXCOORD0)
{
	TextVSOut OUT = (TextVSOut)0;


	OUT.posH = mul(float4(posL,1.0f),gWorldViewProj);
	OUT.tex = tex;

	posL -= fOffset;

	return OUT;
}


VSOut SpriteAnimationVS(VSStreamIn IN)
{
	VSOut OUT = (VSOut)0;

	OUT.posH = mul(float4(IN.pos,1.0f),gWorldViewProj);
	OUT.tex = IN.tex;

	OUT.tex.x *= IN.dx;
	OUT.tex.y *= IN.dy;

	return OUT;
}

float4 ColorSpritePS(float2 tex : TEXCOORD0, float3 interpPos : TEXCOORD1) : COLOR
{
	float4 texColor = tex2D(TexS,tex);

	//texColor.r *= 0.8*cos(texColor.r+interpPos.x);
	//texColor.g *= 0.8*sin(texColor.g+2.0*interpPos.x);
	//texColor.b *= 0.8*sin(texColor.b+3.0*interpPos.y);

	//texColor.rgb = 1 - texColor.xyz;
	
	return texColor;
}


float4 SpritePS(float2 tex : TEXCOORD0) : COLOR
{
	float4 color = tex2D(TexS,tex);
	return color * gColor;
}

float4 TextPS(float2 tex : TEXCOORD0) : COLOR
{
	float4 color = tex2D(TextSampler,tex);
	return color * gColor;
}

void ClampWrap(inout float3 vec)
{
	if(vec.x < -50.0f)
	{
		vec.x = 50.0f;
	}
	else if(vec.x > 50.0f)
	{
		vec.x = -50.0f;
	}

	if(vec.y < -50.0f)
	{
		vec.y = 50.0f;
	}
	else if(vec.y > 50.0f)
	{
		vec.y = -50.0f;
	}
}


TextVSOut LineVS(float3 posL : POSITION0)
{
	TextVSOut OUT = (TextVSOut)0;

	OUT.posH = mul(float4(posL,1.0f),gWorldViewProj);

	return OUT;
}

float4 LinePS() : COLOR
{
	return gColor;
}


// Tech Start
technique Sprite
{
	pass p1
	{
		VertexShader = compile vs_2_0 SpriteVS();
		PixelShader = compile ps_2_0 SpritePS();
	}
}

technique ColorAnimatedSprite
{
	pass p1
	{
		VertexShader = compile vs_2_0 SpriteAnimationVS();
		PixelShader = compile ps_2_0 ColorSpritePS();	
		
		AlphaTestEnable = true;
		AlphaFunc = GreaterEqual;
		AlphaRef = 40;	
	}
}

technique TextTech
{
	pass p1
	{
		VertexShader = compile vs_2_0 TextVS(0);
		PixelShader = compile ps_2_0 TextPS();
		
		AlphaTestEnable = true;
		AlphaFunc = GreaterEqual;
		AlphaRef = 50;		
	}
	/*pass p2
	{
		VertexShader = compile vs_2_0 TextVS(0.5f);
		PixelShader = compile ps_2_0 TextPS();
	}*/
}

technique LineTech
{
	pass p1
	{
		VertexShader = compile vs_2_0 LineVS();
		PixelShader = compile ps_2_0 LinePS();
	}
}
// Tech End