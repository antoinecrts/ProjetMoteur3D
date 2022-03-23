/*
*	Authors: G. THOIN
*	Master 2 ISICG
*/

#version 440 core

uniform sampler2D DepthSampler;

in vec2 texCoord;

layout(std140) uniform CPU
{	
	float zNear;		// Camera Near Clipping Plane
	float zFar;			// Camera Far Clipping Plane
	float FocalNear;	// Camera Near Focal Distance
	float FocalFar;		// Camera Far Focal Distance
};

layout (location = 0) out float Focal;

void main()
{	
	float farFocal = FocalFar / zFar;
	float nearFocal = FocalNear / zFar;

	float depth = texture2D(DepthSampler,texCoord).x;
	/*
	if(depth < nearFocal || depth > farFocal)
	{
		Focal = (1.0);
	}
	if(depth == nearFocal || depth == farFocal)
	{
		Focal = (0.5);
	}
*/
	if(depth > nearFocal && depth < farFocal)
	{
		Focal = (0.0);
	}
	else
		Focal = smoothstep(0.0,1.0,abs(farFocal - depth));
}