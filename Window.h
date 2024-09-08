#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	//Para las llantas
	/////////////////////////////////////////////
	GLfloat getarticulacionllanta1() { return articulacionllanta1; }
	GLfloat getarticulacionllanta2() { return articulacionllanta2; }
	GLfloat getarticulacionllanta3() { return articulacionllanta3; }
	GLfloat getarticulacionllanta4() { return articulacionllanta4; }
	/////////////////////////////////////////////
	//Para el perro
	/////////////////////////////////////////////
	GLfloat getarticulacioncola() { return articulacioncola; }
	GLfloat getarticulacionpata1_1() { return articulacionpata1_1; }
	GLfloat getarticulacionpata1_2() { return articulacionpata1_2; }
	GLfloat getarticulacionpata2_1() { return articulacionpata2_1; }
	GLfloat getarticulacionpata2_2() { return articulacionpata2_2; }
	GLfloat getarticulacionpata3_1() { return articulacionpata3_1; }
	GLfloat getarticulacionpata3_2() { return articulacionpata3_2; }
	GLfloat getarticulacionpata4_1() { return articulacionpata4_1; }
	GLfloat getarticulacionpata4_2() { return articulacionpata4_2; }
	/////////////////////////////////////////////

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	//Grua y Perro
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacionllanta1, articulacionllanta2, articulacionllanta3, articulacionllanta4, articulacioncola, articulacionpata1_1, articulacionpata1_2, articulacionpata2_1, articulacionpata2_2, articulacionpata3_1, articulacionpata3_2, articulacionpata4_1, articulacionpata4_2;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

