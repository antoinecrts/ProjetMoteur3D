// [type] [name] [min val] [max val] [default val]
//    float rho; // 0  1  0.9
//    float sigma; // 0  90  30
float BRDFOrenNayar( vec3 L, vec3 V, vec3 N, float rho, float sigma)
{
    float PI = 3.14159265358979323846;

    float VdotN = dot(V,N);
    float LdotN = dot(L,N);
    float theta_r = acos (VdotN);
    float sigma2 = pow(sigma*PI/180,2);

    float cos_phi_diff = dot( normalize(V-N*(VdotN)), normalize(L - N*(LdotN)) );
    float theta_i = acos (LdotN);
    float alpha = max (theta_i, theta_r);
    float beta = min (theta_i, theta_r);
    if (alpha > PI/2) return 0;

    float C1 = 1 - 0.5 * sigma2 / (sigma2 + 0.33);
    float C2 = 0.45 * sigma2 / (sigma2 + 0.09);
    if (cos_phi_diff >= 0) C2 *= sin(alpha);
    else C2 *= (sin(alpha) - pow(2*beta/PI,3));
    float C3 = 0.125 * sigma2 / (sigma2+0.09) * pow ((4*alpha*beta)/(PI*PI),2);
    float L1 = rho/PI * (C1 + cos_phi_diff * C2 * tan(beta) + (1 - abs(cos_phi_diff)) * C3 * tan((alpha+beta)/2));
    float L2 = 0.17 * rho*rho / PI * sigma2/(sigma2+0.13) * (1 - cos_phi_diff*(4*beta*beta)/(PI*PI));
    return (L1 + L2)*PI;
}