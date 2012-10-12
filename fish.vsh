#version 120

attribute vec3 position;

varying vec4 out_color;

uniform vec3 offset; 	//try changing/animating the uniform below in your code.

void main() {

	float PIE = 3.14159265;
	float wavelength = 1.0;		//must be > width to look like fish. < width looks like jellyfish.
	float depth_displacement = 0.2;
	float depth_distort = 0.4;
	

	float theta = 2 * PIE * ( ( mod( (2 + position.x + offset.x), wavelength) - (wavelength/2) ) / wavelength );

	float z = offset.z + depth_displacement * sin (theta);
	
	float distortion = (1 - depth_distort) + (z*depth_distort);
	
	float x = offset.x + (position.x) * distortion;
	float y = offset.y + (position.y) * distortion;
	
	
	out_color = vec4( z*5 , (1-z)*5, z*0.4, 1.0);
	
	
	gl_Position = vec4(x, y, z, 1.0 );
	
}