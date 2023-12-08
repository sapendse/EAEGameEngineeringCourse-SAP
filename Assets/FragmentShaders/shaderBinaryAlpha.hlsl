/*
	This is an example of a fragment shader
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;
uniform float3 g_pointLightPosition;
uniform float4 g_parameters_PointLight;
uniform float g_attenuationFactor;
uniform float specExpo;
uniform float g_alphaValue;
uniform float4x4 g_worldToViewTransform;

//Sampler
//sampler2D g_diffuseSampler;
sampler2D g_binarySampler;

// Shader program
//===============

void main( in const float4 i_color : COLOR0,
in const float2 i_texcoord : TEXCOORD0,
in const float3 i_world_position : TEXCOORD1,
in const float3 i_normal : NORMAL,
out float4 o_color : COLOR0 )
{
	float atteFac= g_attenuationFactor;
	float specExponent = specExpo;

	// We could also use the interpolated value from the vertex shader:
	float4 color = i_color;
	//float4 texture_sample = tex2D( g_diffuseSampler, i_texcoord );
	float4 texture_bin = tex2D( g_binarySampler, i_texcoord );
	clip( texture_bin[3] - g_alphaValue );
	float4 diffuseColour = texture_bin + color;

	float3 lightColor = g_parameters_PointLight.xyz;
	float lightIntensity = g_parameters_PointLight.w;
	float3 lightDir = normalize( g_pointLightPosition - i_world_position );
	float lightDis = length( g_pointLightPosition - i_world_position );
	
	float dotProd = dot( lightDir, i_normal );

	float3 specLightDir = reflect( lightDir, i_normal );
	float3 viewDir = normalize( g_worldToViewTransform[3].xyz - i_world_position );
	float specLight = pow( saturate( dotProd ), specExponent );
	specLight*= dot( lightDir, i_normal );
	specLight *= lightIntensity;
	
	lightIntensity = 1 / (1 + atteFac * (lightDis * lightDis ) ) * lightIntensity;
	float3 diffuseLight = (lightColor * lightIntensity) * saturate( dotProd );
	float ambientLight= 0.2;	
	diffuseLight += ambientLight;
	

	o_color = ( float4( diffuseLight,  1.0 ) * ( diffuseColour ) + specLight );

}
