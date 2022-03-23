// [type] [name] [min val] [max val] [default val]
//	float k; // 0.5  1  1
float BRDFMinnaert( vec3 L, vec3 V, vec3 N, float k)
{
    return pow(cos(dot(N,L)),k)*pow(cos(dot(N,V)), k-1);
}