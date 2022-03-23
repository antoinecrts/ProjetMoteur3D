#version 430

#extension GL_NV_bindless_texture : require

layout(std140) uniform CPU
{
	sampler2D lightmap;
};

in vec3 texCoord;
in vec3 fpos;
in vec4 tangent;
in vec3 normal;





layout (location = 0) out vec4 opos;
layout (location = 1) out vec4 onormal;
layout (location = 2) out vec4 otex;
layout (location = 3) out vec4 otangent;


void main()
{
	opos = vec4(fpos,1.0);
	onormal.xyz = normalize(normal);
	otex.xyz = texCoord;
	otangent = tangent;
	onormal.w = texture(lightmap,texCoord.xy).x;
	
}