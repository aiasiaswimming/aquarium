#version 120

uniform vec3 offset;

varying vec4 out_color;

void main() {
	float px = out_color.x*50;
	float py = out_color.y*50;
	float q = (sin(px + py + offset.z*10) + 1)/2;
	//gl_FragColor = vec4(sqrt(q),(cos(px) + cos(py) + 2)/4, q, 1.0);
	gl_FragColor = vec4(vec3(cos(offset.x),cos(offset.y),offset.z), 1.0);
}
