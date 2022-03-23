// [type] [name] [min val] [max val] [default val]
//	float n; // 1  1000  100
//	float k; // 0.1  10  1
float BRDFNishino( vec3 L, vec3 V, vec3 N, float n, float k)
{   
    vec3 H = normalize(L+V);
    float NdotH = max(0,dot(N,H));
    float epd = 1-exp(-k * pow(NdotH,n));

    return epd;
}