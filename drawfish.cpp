#include <GL/glfw.h>
#include <iostream>
#include <cmath>

const GLsizei MAX_VERTICES = 128*4+16;
GLfloat vertices[MAX_VERTICES], colors[MAX_VERTICES];
//GLuint topHalf[th], bottomHalf[bh], tail[ta];

double conv(double mint){
	return (mint/180)*M_PI;
}

void draw(){
	int m = 0.1, b = 128*2-2;;
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

int main(){
	if( !glfwInit() ) {
		std::cerr << "Can't initialize OpenGL!\n";
		return -1;
	}
	
	if( !glfwOpenWindow(480, 480, 8, 8, 8, 8, 0, 0, GLFW_WINDOW) ) {
		std::cerr << "Can't open an OpenGL window!\n";
		glfwTerminate();
		return -1;
	}
	glfwSetWindowTitle("");
	glfwEnable(GLFW_STICKY_KEYS);
	glfwSwapInterval(1);
	glClearColor(0,0,0,0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//call draw function
	//call color function	
	draw();

	glColorPointer(4, GL_FLOAT, 0, colors);
	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	do {		
		int width, height;

		glfwGetWindowSize(&width, &height);
		glViewport(0,0,width,height);
		glClear(GL_COLOR_BUFFER_BIT);

		drawAll(128*2,6);
		glfwSwapBuffers();
		
	} while( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED) );
	
	glfwTerminate();
	return 0;
}