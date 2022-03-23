/*
*	Authors: G. THOIN
*	Master 2 ISICG
*/

#version 440 core

uniform sampler2DArray GBuffers;

uniform sampler2D DOSampler;

in vec2 texCoord;

layout(std140) uniform CPU
{	
	int doBlurSize;
    float VStep;
};

layout (location = 0) out vec3 DO;

/* void main()
{
	DO = vec3( 0.0 );
	vec2 offsetCoords;
	float halfBlurSize = floor(doBlurSize / 2.0);

	for(float y = -halfBlurSize; y <= halfBlurSize; ++y)
	{
		offsetCoords = texCoord.xy + vec2(0.0, (y * VStep));
		if(offsetCoords.y >= 0.0 && offsetCoords.y <= 1.0)
			DO += textureLod(DOSampler, offsetCoords, 0).rgb;
	}

	DO = DO / doBlurSize;
} */

void main() {
float weights[9] =
	{
		0.013519569015984728,
		0.047662179108871855,
		0.11723004402070096,
		0.20116755999375591,
		0.240841295721373,
		0.20116755999375591,
		0.11723004402070096,
		0.047662179108871855,
		0.013519569015984728
	};

	float indices[9] = {-4, -3, -2, -1, 0, +1, +2, +3, +4};

	vec2 stepB = vec2(0,VStep);

	vec3 normal[9];

	normal[0] = textureLod(GBuffers, vec3(texCoord.xy + indices[0]*stepB, 4), 0).xyz;
	normal[1] = textureLod(GBuffers, vec3(texCoord.xy + indices[1]*stepB, 4), 0).xyz;
	normal[2] = textureLod(GBuffers, vec3(texCoord.xy + indices[2]*stepB, 4), 0).xyz;
	normal[3] = textureLod(GBuffers, vec3(texCoord.xy + indices[3]*stepB, 4), 0).xyz;
	normal[4] = textureLod(GBuffers, vec3(texCoord.xy + indices[4]*stepB, 4), 0).xyz;
	normal[5] = textureLod(GBuffers, vec3(texCoord.xy + indices[5]*stepB, 4), 0).xyz;
	normal[6] = textureLod(GBuffers, vec3(texCoord.xy + indices[6]*stepB, 4), 0).xyz;
	normal[7] = textureLod(GBuffers, vec3(texCoord.xy + indices[7]*stepB, 4), 0).xyz;
	normal[8] = textureLod(GBuffers, vec3(texCoord.xy + indices[8]*stepB, 4), 0).xyz;

	float total_weight = 1.0;
	float discard_threshold = 0.85;

	int i;

	for( i=0; i<9; ++i )
	{
		if( dot(normal[i], normal[4]) < discard_threshold )
		{
			total_weight -= weights[i];
			weights[i] = 0;
		}
	}

	//

	vec3 res = vec3(0);

	for( i=0; i<9; ++i )
	{
		res += textureLod(DOSampler, texCoord.xy + indices[i]*stepB, 0).xyz * weights[i];
	}

	res /= total_weight;

	DO = res;
}