#include <GL/glew.h>
#include <GL/glew.c>
#include <GL/glfw.h>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <vector>
#include <cstdlib>


//STILL GOTTA MAKE THE SHADERS, MAN. VERTEX AND FRAGMENT.


using namespace std;

enum { ATTRIB_POS };

struct Fish {
	GLfloat startTime;		//appears on screen at what time?
	GLfloat endTime;		//stays how long on screen?
	GLfloat x, y, z;		//position of fish
	GLfloat addx, addy, addz; //increments to fish
};

const float PIE = 3.14159265f;
const GLsizei MAX_VERTICES = 128*4+16;
const GLsizei MAX_FISH = 100;

GLfloat vertices[MAX_VERTICES], colors[MAX_VERTICES];
Fish school[MAX_FISH];
GLuint mainProgram;

//SHADER INITIALIZATION

bool loadShaderSource(GLuint shader, const char *path) {	//helper method
	FILE *f = fopen(path, "r");
	if ( !f ) {
		std::cerr << "ERROR: shader source not found: " << path << '\n';
		return false;
	}
	fseek(f, 0, SEEK_END);
	vector<char> sauce(ftell(f) + 1);
	fseek(f, 0, SEEK_SET);
	fread(&sauce[0], 1, sauce.size(), f);
	fclose(f);
	const GLchar *ptr = &sauce[0];
	glShaderSource(shader, 1, &ptr, 0);
	if ( glGetError() != GL_NO_ERROR ) {
		std::cerr << "ERROR: Unable to load shader\n";
		return false;
	}
	return true;
}

void checkShaderStatus(GLuint shader) { //helper method
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	/*if (logLength > 0) {
		GLchar *log = (GLchar *)malloc(logLength);
		glGetShaderInfoLog(shader, logLength, &logLength, log);
		std::cout << "Shader compile log:\n" << log << endl;
		free(log);
	}*/
}

bool initShader() { 	//change filenames of shaders!!
	GLuint vtx = glCreateShader(GL_VERTEX_SHADER),
	       frag = glCreateShader(GL_FRAGMENT_SHADER);
	
	if ( !loadShaderSource(vtx, "fish.vsh") ) {
		std::cerr << "Error in compiling the vertex shader\n";
		glDeleteShader(vtx);
		glDeleteShader(frag);
		return false;
	}
	if ( !loadShaderSource(frag, "fish.fsh") ) {
		std::cerr << "Error in compiling the fragment shader\n";
		glDeleteShader(vtx);
		glDeleteShader(frag);
		return false;
	}
	
	glCompileShader(vtx);
	checkShaderStatus(vtx);

	glCompileShader(frag);
	checkShaderStatus(frag);

	mainProgram = glCreateProgram();
	glAttachShader(mainProgram, vtx);
	glAttachShader(mainProgram, frag);

	glBindAttribLocation(mainProgram, ATTRIB_POS, "position");

	glLinkProgram(mainProgram);

	GLint logLength;
	glGetProgramiv(mainProgram, GL_INFO_LOG_LENGTH, &logLength);
	/*if (logLength > 0) {
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(mainProgram, logLength, &logLength, log);
		std::cout << "Program link log:\n" << log << endl;
		free(log);
	}*/

	glDeleteShader(vtx);
	glDeleteShader(frag);

	return true;
}

//HELPER METHODS

float fRand() {
	return (float)rand()/RAND_MAX;
}

double conv(double mint){
	return (mint/180)*M_PI;
}

//FISH CREATION

void generateFishModel(){
	int m = 0.1, b = 128*2-2;
	for(int i = 0; i < b; i+=2){
		vertices[i] = 0.36*cos(m);
		vertices[i+1] = 0.2*sin(m);
		//std::cout << vertices[i] << ":::" << vertices[i+1] << "\n";
		m++;
	}

	vertices[b] = cos(conv(35))*0.5;
	vertices[b+1] = sin(conv(35))*0.5;
	vertices[b+2] =  vertices[b-2];
	vertices[b+3] =  vertices[b-1];
	
	vertices[b+4] = vertices[b-2];
	vertices[b+5] = vertices[b-1];
	vertices[b+2] = cos(conv(-35))*0.5;
	vertices[b+3] = sin(conv(-35))*0.4;

	for(int i = 0; i < MAX_VERTICES; i++)
		colors[i] = 1;

	glEnableVertexAttribArray(ATTRIB_POS);	
	glVertexAttribPointer(ATTRIB_POS, 2, GL_FLOAT, GL_TRUE, 0, &(vertices[0]));
}

void generateSchool() {
	generateFishModel();
	for(int i=0; i < MAX_FISH; i++) {
		school[i].startTime = fRand() * 8000.0;
		school[i].x = 1.2;					//x offscreen right
		if(fRand() > 0.5)
			school[i].x *= -1.0;			//50% chance of changing to x offscreen left
		
		school[i].y = fRand() - 0.5;	//random start y between +0.5 and -0.5
		school[i].z = fRand() - 0.5;	//random start z between +0.5 and -0.5
		
		//increments are in range [+0.25, -0.25]
		school[i].addx = 0.05 * fRand() - 0.025;
		school[i].addy = 0.05 * fRand() - 0.025;
		school[i].addz = 0.05 * fRand() - 0.025;
	}
}

void drawAll(int th, int ta){
	//define which indices are which	
	GLuint topHalf[th], tail[ta], tail1[ta];

	for(int i = 0; i < th+1; i++)
		topHalf[i] = i;
	for(int i = 0; i < ta; i++){
		tail[i] = i;
		tail1[i] = i;
	}

	glDrawElements(GL_TRIANGLE_FAN,th, GL_UNSIGNED_INT, topHalf);
	glDrawElements(GL_TRIANGLE_FAN, ta, GL_UNSIGNED_INT, tail1);
	glDrawElements(GL_TRIANGLE_FAN, ta, GL_UNSIGNED_INT, tail);
}

//MAIN

int main() {
	if ( !glfwInit() ) {
		std::cerr << "Unable to initialize OpenGL!\n";
		return -1;
	}
	
	if ( !glfwOpenWindow(640,640,  8,8,8,8,  0,0,  GLFW_WINDOW)) {
		std::cerr << "Unable to create OpenGL window.\n";
		glfwTerminate();
		return -1;
	}

	if ( glewInit() != GLEW_OK ) {
		std::cerr << "Unable to hook OpenGL extensions!\n";
		return -1;
	}

	glfwSetWindowTitle("GLFW Simple Example");
	glfwEnable( GLFW_STICKY_KEYS ); // Ensure we can capture the escape key being pressed below
	glfwSwapInterval( 1 ); // Enable vertical sync (on cards that support it)
	glClearColor(0,0,0,0);

	if ( !initShader() ) {
		return -1;
	}
	generateSchool();

	glUseProgram(mainProgram);	
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	
	GLuint OFFSET_UNIFORM = glGetUniformLocation(mainProgram, "offset");
		
	// READY!
	double t = 0.0;
	
	do {		
		// Get window size (may be different than the requested size)
		//we do this every frame to accommodate window resizing.
		int width, height;
		glfwGetWindowSize( &width, &height );
		glViewport( 0, 0, width, height );
		glClear(GL_COLOR_BUFFER_BIT);
				
		t += 0.02;
		if(t > 8000.0) {	//"refreshes" every minute
			glClear(GL_COLOR_BUFFER_BIT);			
			generateSchool();
			t = 0;
		}
		
		for(int i = 0; i < MAX_FISH; ++i) {
			if(school[i].startTime > t && school[i].x < 1.2) {
				school[i].x += school[i].addx;
				school[i].y += school[i].addy;
				school[i].z += school[i].addz;
				glUniform3f(OFFSET_UNIFORM, school[i].x, school[i].y, school[i].z);
				drawAll(128*2,6);
			}
		}		
		//VERY IMPORTANT: displays the buffer to the screen
		glfwSwapBuffers();	
	} 
	
	while ( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
			glfwGetWindowParam(GLFW_OPENED) );

	glDeleteProgram(mainProgram);
	glfwTerminate();
	return 0;
}
