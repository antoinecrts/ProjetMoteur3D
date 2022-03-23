// [type] [name] [min val] [max val] [default val]
//	float n; // 1  1000  100
//	bool divide_by_NdotL; // false true true
float BRDFBlinnPhong( vec3 L, vec3 V, vec3 N, float n, bool divide_by_NdotL)
{   
    vec3 H = normalize(L+V);

    float val = pow(max(0,dot(N,H)),n);
    if (divide_by_NdotL)
        val = val / dot(N,L);
    return val;
}