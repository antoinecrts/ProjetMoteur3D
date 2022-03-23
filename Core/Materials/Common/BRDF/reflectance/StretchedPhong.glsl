// [type] [name] [min val] [max val] [default val]
//	float Ks; // 0  1  1
//	float n; // 1  1000  1
//	float p; // 0  1  1
float BRDFStretchedPhong( vec3 L, vec3 V, vec3 N, float Ks, float n, float p)
{   
    float norm = (n+2)/(2);
    float NdotL = dot(N,L);
    float NdotV = dot(N,V);
    float LdotV = dot(L,V);

    float val = norm * pow(max(0, 2 * NdotL * NdotV - LdotV), n) / pow(max(NdotL, NdotV), p);
    return Ks * val;
}