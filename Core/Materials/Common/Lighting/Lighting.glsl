
#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL
#include "/Materials/Common/Common"
struct Light
{
	vec4 pos;
	vec4 color;
	vec4 direction;
	vec4 info;
};

layout (std430,binding=LIGHTING_SSBO_BINDING) readonly buffer LightingBuffer
{
	vec4 camPos;
	ivec4 nbLights;	
	Light Lights[];
};

struct node {
	vec4 plane;
	uint link[4];
//	uvec4 distance;
};


layout (std430, binding=13) restrict buffer ShadowBuffer
{
	node nodes[];
};


layout (std430, binding=29)  buffer RootBuffer	{
	uint root[];
};

layout (std430,binding=30) buffer util { uint nbr_nodes_added[]; } nb;

layout (std430, binding=31) buffer computeLightBuffer { uint light; } computeLight;

const uint nodesOffset = nb.nbr_nodes_added[0] + nb.nbr_nodes_added[1] + nb.nbr_nodes_added[2]+12;

//float 	PLANEPOINT(in vec4 plane,in vec3 point) 	{ return dot(plane.xyz,point)+plane.w; }


//float planPointPosition(in vec4 plane,in vec3 p) // in 1,out -1,cut 0
//{
	//const float value = PLANEPOINT(plane,p);
	//if(abs(value)<POLYGON_ZERO) return 0;
	//return int(sign(value));
//}



// Phong Highlight
vec3 PhongHighlight(in float dotRV, in vec3 S, in vec4 K)
{
	return (S * K.z * pow(dotRV, K.w));
}

// Blinn-Phong Highlight
vec3 BlinnPhongHighlight(in float dotNH, in vec3 S, in vec4 K)
{
	return (S * K.z * pow(dotNH, K.w));
}

// Ward's Anisotropic Highlight
vec3 WardHighlight(in float dotNL, in vec3 V, in vec3 H, in vec3 T, in vec3 B, in vec3 N, in vec3 S, in vec4 K)
{
	vec2 roughness = vec2(0.5, 0.5);
	float PI = 3.1415927410125732421875;
	
	float dotNV = max(dot(N, V), 0.0);

	if(dotNL < 0.0 || dotNV < 0.0)
		return vec3(0.0);
	else
	{
		float dotNH = max(dot(N, H), 0.0);
		float dotHT = max(dot(H, T), 0.0);
		float dotHB = max(dot(H, B), 0.0);
		/*(1.0 / sqrt(dotNL * dotNV)) * (dotNL / (4.0 * PI * roughness.x * roughness.y)) */
		float Ward =  exp(-2.0 * ((pow((dotHT / roughness.x), 2.0) + pow((dotHB / roughness.y), 2.0)) / (1.0 + dotNH)));
		
		return (S * K.z * sqrt(max(0.0, dotNL / dotNV)) * Ward);
	}
}

vec3 Shading(in vec3 T, in vec3 B, in vec3 N, in vec3 V, in vec3 P, in float AO, in vec3 D, in vec3 S, in vec4 K, in int SpecularHighlight)
{
	float Falloff = 1.0, dotNL;
	vec3 L, Ambient = vec3(0.0), Diffuse = vec3(0.0), Specular = vec3(0.0);
	
	Ambient = D * K.x;
	for (int i = 0; i < nbLights.x; i++)
	{
		L = normalize(Lights[i].pos.xyz - P);

		if(Falloff > 0.0)
		{
			dotNL = max(dot(L, N), 0.0);

			Diffuse += D * K.y * dotNL * Lights[i].color.rgb;
			
			if(dotNL > 0.0)
			{
				switch(SpecularHighlight)
				{
					case 0: // Phong Highlight
							Specular += PhongHighlight(max(dot(reflect(-L, N), V), 0.0), S, K);
							break;
					case 1: // Blinn-Phong Highlight
							Specular += BlinnPhongHighlight(max(dot(N, normalize(V + L)), 0.0), S, K);
							break;
					case 2: // Ward's Anisotropic Highlight
							Specular += WardHighlight(dotNL, V, normalize(V + L), T, B, N, S, K);
							break;
				}
				Specular *= Lights[i].color.rgb;
			}
		}
	}
	Diffuse = Diffuse / nbLights.x;
	Specular = Specular / nbLights.x;
	return (Ambient + Diffuse + Specular) * AO;
}


float computeSpecular(in vec4 lightDir,	in vec3 eyeDir ,in vec3 normal,float power)
{
	vec3 R = reflect(-lightDir.xyz,normal);
	return pow(max(0.0,dot(R,eyeDir.xyz)),power);
}
float computeDiffuse(in vec4 lightDir,	in vec3 normal)
{
	return max(0.0,dot(lightDir.xyz,normal));	
}

vec4 
addLight(in vec4 LightPos, in vec4 LightColor, in vec3 position, in vec3 normal, in vec4 ambiant, in vec4 diffuse, in vec4 specular, in vec4 coefs)
{
	vec4 _color;
	vec4 lDir;
	lDir.xyz = LightPos.xyz - position;
	lDir.w = length(lDir);
	lDir.xyz = normalize(lDir.xyz);

	vec4 cPos;
	cPos.xyz = normalize(camPos.xyz-position);
	
	_color += coefs.y * computeDiffuse(lDir,normal) * diffuse * LightColor; // * L.color;
	_color += coefs.z * computeSpecular(lDir,cPos.xyz,normal,coefs.w) * specular * LightColor;// * L.color;

	return _color;
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

	vec4 _color = vec4(0.0);

	_color.xyz = ambiant.xyz*coefs.x;

	for (int i = 0;i < nbLights.x ; i++)
	{
		vec4 col = addLight(Lights[i].pos, Lights[i].color, 
						position, normal,
						ambiant,diffuse,specular,coefs);
		_color.xyz += Lights[i].color.w*col.xyz;			
	}

	return _color;
		
}

float G1V ( float dotNV, float k ) {
	return 1.0 / (dotNV*(1.0 - k) + k);
}


vec3 computePBRLighting ( in Light light, in vec3 position, in vec3 N, in vec3 V, in vec3 albedo, in float roughness, in vec3 F0, in vec3 SSDO ) {

	float alpha = roughness*roughness;
	vec3 L = normalize(light.pos.xyz - position);
	//vec3 V = normalize (camPos.xyz-position);
	vec3 H = normalize (V + L);

	float dotNL = clamp (dot (N, L), 0.0, 1.0);
	float dotNV = clamp (dot (N, V), 0.0, 1.0);
	float dotNH = clamp (dot (N, H), 0.0, 1.0);
	float dotLH = clamp (dot (L, H), 0.0, 1.0);

	float D, vis;
	vec3 F;

	// NDF : GGX
	float alphaSqr = alpha*alpha;
	float pi = 3.1415926535;
	float denom = dotNH * dotNH *(alphaSqr - 1.0) + 1.0;
	D = alphaSqr / (pi * denom * denom);

	// Fresnel (Schlick)
	float dotLH5 = pow (1.0 - dotLH, 5);
	F = F0 + (1.0 - F0)*(dotLH5);

	// Visibility term (G) : Smith with Schlick's approximation
	float k = alpha / 2.0;
	vis = G1V (dotNL, k) * G1V (dotNV, k);

	vec3 specular = /*dotNL **/ D * F * vis;

	

	float invPi = 0.31830988618;
	vec3 diffuse = (albedo /** invPi*/);

	float lightOcclusion = 1; // Facteur SSDO
	//if (SSDO != vec3(0))
		lightOcclusion = 1 - clamp(dot(L, SSDO), 0.0f, 1.0f);

	return  (diffuse + specular) * light.color.xyz * dotNL * lightOcclusion ;
}

vec3 addPBRLighting ( in vec3 position, in vec3 N, in vec3 V, in vec3 albedo, in float roughness, in vec3 F0, in vec3 SSDO ) 
{

vec3 _color = vec3(0.0);
vec3 ambient = vec3(.03);
	_color.xyz = ambient.xyz;
	for (int i = 0;i < nbLights.x ; i++)
	{
		vec3 col = computePBRLighting(Lights[i], 
						position, N,
						V,albedo,roughness,F0,SSDO);
		_color.xyz += Lights[i].color.w*col.xyz;			
	}
	return _color;
}









struct triangle {
	vec4 a;
	vec4 b;
	vec4 c;
};


layout (std430,binding=28)  readonly	 buffer toto	{ 
	mat4 MVP2;
	ivec4 info;
	triangle myTriangles[];
};

//167 486



//Calcul d'ombre
vec4 PSV_EGcode_dist( in vec3 p, in vec3 normal){ //avec distance uint
	vec4 res = vec4(0);
	uint stacksize = 0;
    uint stack[32];
	const float dist = length(p);

	// si on tourne le dos à la lumière ce n'est pas la peine de faire une requête
	if ( dot( normal, -p) < 0.0 ) return res;
 	
	uint current = 4;

	while(current>1){
		// pop
		const node n = nodes[ current ];
		
		// res.y +=1.0;

		if ( current%4==0 && dist < uintBitsToFloat(n.link[2]) ){  
			current = stacksize > 0 ? stack[ --stacksize ] : 1;
			continue;
		}

		const float offset = dot(n.plane.xyz, p) + n.plane.w;

		// si il y a un arbre both, on empile le noeud si on est dans l'interval du sous arbre	
		 if ( n.link[1]>0  &&   offset*offset / (dist*dist) < uintBitsToFloat(n.link[3] )  ) 
		   stack[ stacksize++ ] = n.link[1];

		//if ( stacksize > 32 ) res.y = 666.0;
				
		// positive case
		if ( offset>0.0 ){	
			if (n.link[0]>0)
				current = n.link[0] ;
			else
				current = stacksize>0 ? stack[ --stacksize ] : 1;										
		}
		else// negative case
			current = current%4==3 ? 0 : current+1;	
			
	}
	res.x = float(current);
	return res;
}


vec4 PSV_StackLess( in vec3 p, in vec3 normal, in int nLight ){ //avec distance uint
	if(nb.nbr_nodes_added[computeLight.light]<=4) return vec4(1);
	bool bothdone = false;
	vec4 res = vec4(0);
	const float dist =  length(p);

	// si on tourne le dos à la lumière ce n'est pas la peine de faire une requête
	if ( dot( normal, -p) < .0f ) return res;
 
 	//uint current = root[nLight];
	uint current = root[computeLight.light];

	while(current>1){
		
		const node n = nodes[ current ];
		// res.y+=1.0;
		if ( current%4==0 && dist< uintBitsToFloat(n.link[2]) ) {//} n.plane.w  ) {
			current = nodes[current+2].link[2];
			bothdone = true;
		}
		else{
			const float offset = dot(n.plane.xyz, p) + n.plane.w;
	
	 		if ( bothdone==false && n.link[1]>0u && offset*offset / (dist*dist)< uintBitsToFloat(n.link[3])  )
	 			current = n.link[1];
	 		else
	 		{
				if ( offset>0.0f ){ // go left
					if (n.link[0]==0){
						bothdone=true;
						current = nodes[current - current%4 + 2].link[2];
						//if (current==0) current =1;
					}
					else{
						bothdone=false;
						current = n.link[0];
					}
				
				}
				else { // go right (negative case)
					bothdone=false;
					current = current%4==3 ? 0 : current+1;
		 		}
		 	}
		 	
		}
	}
	
	res.x = float(current);
	return res;
}

vec4 addPBR( in vec3 position, in vec3 N, in vec3 albedo, in float roughness, in vec3 F0, in vec3 SSDO) 
{
	vec4 _color;
	vec3 eyeDir = normalize(camPos.xyz - position);
	for ( int i = 0; i < nbLights.x; i++ ) {
			vec3 col = computePBRLighting ( Lights[i], position, N, eyeDir, albedo, roughness, F0, SSDO);
			_color.xyz += Lights[i].color.w*col;
			vec4 query = PSV_StackLess(position-Lights[i].pos.xyz, N, i);
			_color *= query.x;
	}

	return _color;
}



vec4 addBoulanger3 ( vec3 position,
					vec4 positionColor,
					vec3 normal,
					vec3 groundNormal,
					float height,
					float reflectance,
					float attenuation,
					float emittance,
					float occlusionDensity
					)
{
	vec4 color_added  = vec4(0.0);
	// test
	color_added.rgb = emittance * positionColor.rgb;
	

	bool barbare = false;
	vec3 N = normalize(normal);

	for (int i = 0;i < nbLights.x;i++)
	{

		vec3 L = Lights[i].pos.xyz - position;
		//L = vec3(0.0,0.0,1.0);
		float d = length(L);
		L = normalize(L);
		
		vec3 Id = Lights[i].color.xyz * Lights[i].color.w;

		vec3 LL = L * L;

		float volOcclusion = clamp( max(exp(-occlusionDensity * (-height * dot(groundNormal,L) + sqrt(1.0-pow(height,2.0)*(1.0 - dot(groundNormal,LL))))),0.0),0.0,1.0);
		volOcclusion = pow(volOcclusion,2.0);
		//volOcclusion *= clamp(abs(dot(N,groundNormal)),0.1, 1.0);
		//volOcclusion *= clamp( abs(  max(dot(N,groundNormal),0.0) + max(dot(-N,groundNormal),0.0) ) ,0.1, 1.0);

		if (barbare)
		{
			color_added.rgb += volOcclusion * positionColor.rgb;
			barbare = true;
		}
		
		//color_added.xyz *= 0.3*reflectance * volOcclusion;

		vec3 diffuse_in = 	reflectance 
							* max(dot(N,L),0) 
							* Id * positionColor.xyz ;
							//* (Id / (1.0 + attenuation * d * d))
							//* (Id / (1.0 + attenuation))
							;
		vec3 diffuse_out = 	reflectance 
							* max(dot(-N,L),0) 
							* Id * positionColor.xyz ;
							//* (Id / (1.0 + attenuation * d * d))
							//* (Id / (1.0 + attenuation))
							;

		color_added.xyz += volOcclusion * attenuation * (diffuse_in + diffuse_out);


		//color_added.xyz = vec3(volOcclusion,0.0,0.0);
	}

	color_added.w = positionColor.w;
	
	return color_added;
}

vec4 addBoulangerPBR( 
					vec3 position,
					vec4 positionColor,
					vec3 normal,
					vec3 groundNormal,
					float height,
					float reflectance,
					float attenuation,
					float emittance,
					float occlusionDensity
					)
{
	vec4 color_added  = vec4(0.0);
	// test
	color_added.rgb = emittance * positionColor.rgb;
	

	bool barbare = false;
	vec3 N = normalize(normal);

	for (int i = 0;i < nbLights.x;i++)
	{

		vec3 L = Lights[i].pos.xyz - position;
		//L = vec3(0.0,0.0,1.0);
		float d = length(L);
		L = normalize(L);
		
		vec3 Id = Lights[i].color.xyz * Lights[i].color.w;

		vec3 LL = L * L;

		float volOcclusion = clamp( max(exp(-occlusionDensity * (-height * dot(groundNormal,L) + sqrt(1.0-pow(height,2.0)*(1.0 - dot(groundNormal,LL))))),0.0),0.0,1.0);
		volOcclusion = pow(volOcclusion,2.0);
		//volOcclusion *= clamp(abs(dot(N,groundNormal)),0.1, 1.0);
		//volOcclusion *= clamp( abs(  max(dot(N,groundNormal),0.0) + max(dot(-N,groundNormal),0.0) ) ,0.1, 1.0);

		if (barbare)
		{
			color_added.rgb += volOcclusion * positionColor.rgb;
			barbare = true;
		}
		
		//color_added.xyz *= 0.3*reflectance * volOcclusion;

		vec3 diffuse_in = 	reflectance 
							* max(dot(N,L),0) 
							* Id * positionColor.xyz ;
							//* (Id / (1.0 + attenuation * d * d))
							//* (Id / (1.0 + attenuation))
							;
		vec3 diffuse_out = 	reflectance 
							* max(dot(-N,L),0) 
							* Id * positionColor.xyz ;
							//* (Id / (1.0 + attenuation * d * d))
							//* (Id / (1.0 + attenuation))
							;

		color_added.xyz += volOcclusion * attenuation * (diffuse_in + diffuse_out);


		//color_added.xyz = vec3(volOcclusion,0.0,0.0);
	}

	color_added.w = positionColor.w;
	
	return color_added;
}

vec4 addBoulangerFixe ( vec3 position,
					vec4 positionColor,
					vec3 normal,
					vec3 groundNormal,
					float height,
					float reflectance,
					float attenuation,
					float emittance,
					float occlusionDensity
					)
{
	vec4 color_added = positionColor;


	
		vec3 L = vec3(150,150,150) - position;
		//L = vec3(0.0,0.0,1.0);
		float d = length(L);
		L = normalize(L);
		vec3 N = normalize(normal);
		vec3 Id = vec3(1.0);

		vec3 LL = L*L;

		float volOcclusion = clamp( max(exp(-occlusionDensity * (-height * dot(groundNormal,L) + sqrt(1.0-pow(height,2.0)*(1.0 - dot(groundNormal,LL))))),0.0),0.0,1.0);
		volOcclusion = pow(volOcclusion,2.0);
		color_added.xyz *= 0.3 * reflectance * volOcclusion;

		vec3 diffuse_in = reflectance 
							* max(dot(N,L),0) 
							* Id * color_added.xyz ;
							//* (Id / (1.0 + attenuation * d * d))
							//* (Id / (1.0 + attenuation))
							;
		vec3 diffuse_out = reflectance
							* emittance 
							* max(dot(-N,L),0) 
							* Id * color_added.xyz ;
							//* (Id / (1.0 + attenuation * d * d))
							//* (Id / (1.0 + attenuation))
							;

		color_added.xyz += volOcclusion * (diffuse_in + diffuse_out);


		L = vec3(-150,-150,150) - position;
		//L = vec3(0.0,0.0,1.0);
		 d = length(L);
		L = normalize(L);
		 N = normalize(normal);
		 Id = vec3(1.0);

		 LL = L*L;

		 volOcclusion = clamp( max(exp(-occlusionDensity*(-height*dot(groundNormal,L) + sqrt(1.0-pow(height,2.0)*(1.0 - dot(groundNormal,LL))))),0.0),0.0,1.0);
		volOcclusion = pow(volOcclusion,2.0);
		color_added.xyz *= 0.3*reflectance * volOcclusion;

		diffuse_in = reflectance 
							* max(dot(N,L),0) 
							* Id * color_added.xyz ;
							//* (Id / (1.0 + attenuation * d * d))
							//* (Id / (1.0 + attenuation))
							;
		diffuse_out = reflectance
							* emittance 
							* max(dot(-N,L),0) 
							* Id *color_added.xyz ;
							//* (Id / (1.0 + attenuation * d * d))
							//* (Id / (1.0 + attenuation))
							;

		color_added.xyz += volOcclusion*(diffuse_in + diffuse_out);


		//color_added.xyz = vec3(volOcclusion,0.0,0.0);
	


	return color_added;
}

#endif
