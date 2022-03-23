#version 430

#extension GL_ARB_shading_language_include : enable
#include "/Materials/Common/Lighting/Lighting"

#include "/Materials/Common/BRDF/reflectance/phong"


layout(std140) uniform CPU
{	
	vec4 color;
float kd;
float ks;
float spec_pow;

};


in vec3 V;
in vec3 N;
in vec3 pointPosition;
in vec3 pos;


layout (location = 0) out vec4 Color;



void main()
{
	Color = vec4(0.0, 0.0, 0.0, 1.0);
 for(int i = 0 ; i < nbLights.x; ++i){
	vec3 L = normalize(Lights[i].pos.xyz - pointPosition);
	float diffuse_value = 1;
vec4 diffuse_composante = color * max(0, dot(N,L)) * diffuse_value * kd;
	float specular_value = phongBRDF(N,V,L,spec_pow);
	vec4 specular_composante = color * specular_value * ks;
Color += specular_composante + diffuse_composante;
}

}