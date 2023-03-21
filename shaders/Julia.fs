#version 330 core
//#extension GL_ARB_gpu_shader_fp64 : enable

#define MAX_ITER 10000

uniform vec2 pos;
uniform float zoom;

uniform vec2 iResolution;
uniform float iTime;

out vec4 FragColor;

float MandelBrot(vec2 z, vec2 c) {
	float Iter = 0.0;

	while(length(z) < 4 && Iter < MAX_ITER) {

		z = vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y) + c;
		Iter += 1;
	}

	return Iter;
}

void main() {
	vec2 uv = (gl_FragCoord.xy - iResolution.xy/2.0) / iResolution.y;

	vec2 c = vec2(-0.75, 0.11);
	vec2 z = vec2(pos.x, pos.y) + uv * zoom;

	float Iter = MandelBrot(z, c);

	FragColor = sin(vec4(.3, .45, .65, 1) * sqrt(Iter/MAX_ITER) * 40)*.5+.5;
}