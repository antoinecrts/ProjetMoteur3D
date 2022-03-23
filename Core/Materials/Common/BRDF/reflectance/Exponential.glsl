// [type] [name] [min val] [max val] [default val]
//	  float c; // 0.001  1  0.1
//    bool normalized; // false  true  true
float BRDFExponential( vec3 L, vec3 V, vec3 N, float c, bool normalized)
{   
	
	vec3 H = normalize(L+V);
    float NdotH = dot(N, H);

    float D = exp((-acos(NdotH))/c);
    if (normalized)
        D *= (1 + 4*c*c)/(2*c*c*(1 + exp(-(3.14/(2*c))))*3.14);
    return D;
}