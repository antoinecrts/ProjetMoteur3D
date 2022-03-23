
#extension GL_ARB_shader_storage_buffer_object : enable

struct Light
{
  vec4 pos;
  vec4 color;
  vec4 direction;
  vec4 info;
  mat4 worldToLight;
  mat4 worldToLight2;
  
 // vec4 cutoff; // CutOff parameters (x : min val, y max value)
  
};

layout (binding=25) buffer LightingBuffer
{
	vec4 camPos;
	vec2 wSize;
	
	//bool useSSAO;
	int nbLights;	
	 Light Lights[];
};


uniform layout(binding = 10) sampler2D ssaoMap;


float computeSpecular(in vec4 lightDir,	in vec3 eyeDir ,in vec3 normal,float power)
{
	vec3 R = reflect(-lightDir.xyz,normal);
	return pow(max(0.0,dot(R,eyeDir.xyz)),power);
}
float computeDiffuse(in vec4 lightDir,	in vec3 normal)
{
	return max(0.0,dot(lightDir.xyz,normal));	
}

vec4 addLight(in Light L,in vec3 position, in vec3 normal,in vec4 ambiant, in vec4 diffuse,in vec4 specular,in vec4 coefs)
{
	vec4 color;
	vec4 lDir;
	lDir.xyz = L.pos.xyz - position;
	lDir.w = length(lDir);
	lDir.xyz = normalize(lDir.xyz);

	vec4 cPos;
	cPos.xyz = normalize(camPos.xyz-position);
		
	color += coefs.y*computeDiffuse(lDir,normal) * diffuse * L.color;
	color += coefs.z*computeSpecular(lDir,cPos.xyz,normal,coefs.w) * specular * L.color;

	return color;
}

vec4 addLightShadow(in Light L,in vec3 position, in vec3 normal,in vec4 ambiant, in vec4 diffuse,in vec4 specular,in vec4 coefs,in float shadow)
{
	vec4 color = vec4(0);
	vec4 lDir;
	lDir.xyz = L.pos.xyz - position;
	lDir.w = length(lDir);
	lDir.xyz = normalize(lDir.xyz);

	vec4 cPos;
	cPos.xyz = normalize(camPos.xyz-position);
	if(shadow!=1.0)
	{
		color +=coefs.y*computeDiffuse(lDir,normal) * diffuse * L.color;
		//color += coefs.z*computeSpecular(lDir,cPos.xyz,normal,coefs.w) * specular * L.color;
		//color = vec4(1,0,0,1);
	}
	else
	{
		color += coefs.y*computeDiffuse(lDir,normal) * diffuse * L.color;
		color += coefs.z*computeSpecular(lDir,cPos.xyz,normal,coefs.w) * specular * L.color;
	}

	return color;
}

vec4 addPhong(in vec3 position, in vec3 normal,in vec4 ambiant, in vec4 diffuse,in vec4 specular,in vec4 coefs)
{	

	vec4 color;
	vec2 tcoord = gl_FragCoord.xy / wSize;

	color.xyz = ambiant.xyz*coefs.x;//*texture(ssaoMap,tcoord).x;

	for (int i = 0;i < nbLights;i++)
		{
			vec4 col = addLight(Lights[i],position,	normal,ambiant,diffuse,specular,coefs);
			color.xyz += Lights[i].color.w*col.xyz;			
		}


		return color;

		
}
vec4 addTriLight(in vec3 position, in vec3 normal,in vec4 color0, in vec4 color1,in vec4 color2)
{	
	vec4 color;
	vec4 cPos;
	cPos.xyz = normalize(camPos.xyz-position);
	for (int i = 0;i < nbLights;i++)
		{
			vec3 L = normalize(Lights[i].pos.xyz-position);
			float NL = dot(normal,L);
			color = color0 * clamp(NL,0.0,1.0) + color1 * (1.0-abs(NL)) + color2 * clamp(-NL,0.0,1.0);	
			color = color + vec4(1.0)*computeSpecular(vec4(L,0.0),cPos.xyz,normal,256);
			
			
		}
		
		return color;
}

/**
*	Modèle d'illumination de la these de K. BOULANGER pour un champ d'herbe
*	@param position			position de calcul de la lumiere
*	@param positionColor	couleur original du point d'evaluation
*	@param normal			normal du point d'evaluation
*	@param occlusionFactor	facteur d'occlusion ambiente du champ
*	@param reflectance		facteur de reflection de la lumiere par brin
*	@param attenuation		facteur d'absorption de la lumiere par le brin
*	@param emittance		facteur d'emission de la lumiere par le brin
*	@return			Color of the point
*/
vec4 addBoulanger	(	vec3 position, 
						vec4 positionColor,
						vec3 normal,	
						float occlusionFactor, 
						float reflectance, 
						float attenuation, 
						float emittance  
					) 
{
	vec4 color_added = positionColor;
	color_added.xyz *= reflectance * occlusionFactor;
	
	for (int i = 0;i < nbLights;i++){
	
		vec3 L = Lights[i].pos.xyz - position;
		float d = length(L);
		L = normalize(L);
		vec3 N = normalize(normal);
		vec3 Id = Lights[i].color.xyz;
	
		vec3 diffuse_in = reflectance * 
							max(dot(N,L),0) * 
							(Id / (1.0 + attenuation * d * d));
		vec3 diffuse_out = reflectance * emittance * 
							max(dot(-N,L),0) * 
							(Id / (1.0 + attenuation * d * d));

		color_added.xyz += diffuse_in + diffuse_out;

	}
	
	
	return color_added;

}