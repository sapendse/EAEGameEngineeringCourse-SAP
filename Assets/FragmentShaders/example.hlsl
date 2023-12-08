/*
	This is an example of a fragment shader
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;
uniform float3 g_pointLightPosition;
uniform float4 g_parameters_PointLight;
uniform float g_attenuationFactor;

//Sampler
sampler2D g_diffuseSampler;
sampler2D g_normalMapping;

// Shader program
//===============

void main( in const float4 i_color : COLOR0,
in const float2 i_texcoord : TEXCOORD0,
in const float3 i_world_position : TEXCOORD1,
in float3 i_tangent : TANGENT,
in float3 i_binormal : BINORMAL,
in float3 i_normal : NORMAL0,
out float4 o_color : COLOR0 )
{
	i_normal = normalize( i_normal );
	i_tangent = normalize( i_tangent );
	i_binormal = normalize( i_binormal );

	// We could also use the interpolated value from the vertex shader:
	float4 color = i_color;
	float4 texture_sample = tex2D( g_diffuseSampler, i_texcoord );
	float4 mappingNormal = tex2D( g_normalMapping, i_texcoord );
	float4 diffuseColour = texture_sample + color;

	
	float3 texture_normal = mappingNormal.xyz;

	texture_normal = ( texture_normal * 2.0 ) - 1.0;

	float3x3 rotation_Matrix_Normals;

	rotation_Matrix_Normals[0].xyz = i_tangent;
	rotation_Matrix_Normals[1].xyz = i_binormal;
	rotation_Matrix_Normals[2].xyz = i_normal;
	
	i_normal = mul( texture_normal, rotation_Matrix_Normals );

	float3 lightColor = g_parameters_PointLight.xyz;
	float lightIntensity = g_parameters_PointLight.w;
	float3 lightDir = normalize( g_pointLightPosition - i_world_position );
	float dotProd = dot( lightDir, i_normal );
	float3 diffuseLight = (lightColor * lightIntensity) * saturate( dotProd );

	float3 ambientLight = 0.2;
	o_color = float4 ( ( diffuseLight+ ambientLight ), 1.0f ) * diffuseColour;
}
