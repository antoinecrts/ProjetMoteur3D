// [type] [name] [min val] [max val] [default val]
//	float alpha; // 0.001  1  0.1
float BRDFGGX( vec3 L, vec3 V, vec3 N, float alpha)
{
    vec3 H = normalize( L + V );
    float CosSquared = dot(N,H)*dot(N,H);
    float TanSquared = (1-CosSquared)/CosSquared;
    return (1.0/3.14) * sqrt(alpha/(CosSquared * (alpha*alpha + TanSquared)));
}