#version 330 core

#extension GL_ARB_gpu_shader_fp64 : enable

#define MAX_ITER 5000

uniform vec2 pos;
uniform float zoom;

uniform vec2 iResolution;
uniform float iTime;

out vec4 FragColor;

float MandelBrot(dvec2 z, dvec2 c) {
	float Iter = 0;

	double zrsqr = z.x * z.x;
	double zisqr = z.y * z.y;

	while(zrsqr + zisqr <= 4. && Iter < MAX_ITER) {
		z.y = (z.x + z.y)*(z.x + z.y) - zrsqr - zisqr;
		z.y += c.y;
		z.x = zrsqr-zisqr + c.x;
		zrsqr = z.x * z.x;
		zisqr = z.y * z.y;
		
		Iter += 1;
	}

	return Iter;
}

void main() {
	dvec2 uv = (gl_FragCoord.xy - iResolution.xy/2.0) / iResolution.y;

	dvec2 z = dvec2(0.0);
	dvec2 c = pos + uv * zoom;

	float Iter = MandelBrot(z, c);

	FragColor = sin(vec4(.3, .45, .85, 1) * sqrt(Iter/MAX_ITER) * 40)*.5+.5;
	//FragColor =vec4(vec3(Iter/MAX_ITER), 1.0);
}