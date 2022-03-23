// [type] [name] [min val] [max val] [default val]
//	float alpha_x; // 0  1.0  0.15
//	float alpha_y; // 0  1.0  0.15
//	vec4 Cs; // 0,0,0  1,1,1  1,1,1
//	vec4 Cd; // 0,0,0  1,1,1  1,1,1
//	bool isotropic; // false  true  false
//	X et Y tengente et bitengente a la surface
vec3 BRDFWard( vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y, float alpha_x, float alpha_y, vec4 Cs, vec4 Cd, bool isotropic)
{   
	vec3 H = normalize(L + V);

    // specular
    float ax = alpha_x;
    float ay = isotropic ? alpha_x : alpha_y;
	
	//float dotHX = dot(H,X)/alpha_x;
	float dotHX = dot(H,X)/ax;
	//float dotHY = dot(H,Y)/alpha_y;
	float dotHY = dot(H,Y)/ay;
	float dotHN = dot(H,N);
	
	float exponent = -2.0 * (dotHX*dotHX + dotHY*dotHY) / (1.0 + dotHN);
	vec4 spec = Cs * sqrt(max(0.0, dot(L,N))) * exp(exponent); 
	
	vec4 diffus = Cd * max(0.0, dot(L,N));
	
	return spec.xyz + diffus.xyz;
}