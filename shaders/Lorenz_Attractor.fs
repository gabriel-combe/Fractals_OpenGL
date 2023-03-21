#version 330 core
//#extension GL_ARB_gpu_shader_fp64 : enable

#define MAX_ITER 64
#define THRESHOLD .01

uniform vec2 iResolution;
uniform float iTime;

out vec4 FragColor;

void main() {
	vec2 uv = (gl_FragCoord.xy - iResolution.xy/2.0) / iResolution.y;
    uv = vec2(0.0, 25.) + uv * 40.;

	float x = 0.1, y = 0.1, z = 0.1, x1, y1, z1;
    float dt = 0.001;
    float sigma = 10.0, rho = 8.0/3.0, r = iTime * 2.; //iTime * 2.;
    float a = -1.7, b = 1.3, c = -0.1, d = -1.2;
    
    float rez = 1000.;
    float dst = 0.;

    for (int i = 0; i < 25000; i++){

        x1 = x + sigma*(-x+y)*dt;
        y1 = y + (r*x-y-z*x)*dt;
        z1 = z + (-rho*z+x*y)*dt;
        
        //x1 = sin(a*y)+c*cos(a*x);
        //y1 = sin(b*x)+d*cos(b*y);

        x = x1;    y = y1;    z = z1;
        
        rez = min (rez, length(uv - vec2(x, z)));
    }

    float c1 = pow( clamp( rez / 2.,    0.0, 1.0 ), 0.5 );
    float c2 = pow( clamp( r / 20., 0.0, 1.0 ), 2.0 );
    vec3 col1 = 0.5 + 0.5*sin( 3.0 + 4.0*c2 + vec3(0.0,0.5,1.0) );
    vec3 col = 2.0*sqrt(c1*col1);

	FragColor = vec4(col, 1.0);
}