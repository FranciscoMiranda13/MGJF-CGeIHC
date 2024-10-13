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
	GLfloat getXChange();
	GLfloat getYChange();

	// PARA EJERCICIOS Y PRACTICAS PASADAS
	/////////////////////////////////////////////////////////////////////
	GLfloat getmuevex() { return muevex; }
	GLfloat getapagarPrenderLampara() { return apagarPrenderLampara; }
	GLfloat getmueveHelicoptero() { return mueveHelicoptero; }
	GLfloat getrotaeny() { return rotaeny; }
	GLfloat getrotaenz() { return rotaenz; }
	GLfloat getarticulacionCofre() { return articulacionCofre; }
	/////////////////////////////////////////////////////////////////////

	// ACTIVIDADES PRACTICA 8
	/////////////////////////////////////////////////////////////////////
	GLfloat gethaciaAdelanteAtras() { return haciaAdelanteAtras; }
	GLfloat getinicio() { return inicio; }
	GLfloat getapagaPrende() { return apagaPrende; }
	GLfloat getinicio2() { return inicio2; }
	/////////////////////////////////////////////////////////////////////

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	/// PARA EJERCICIOS Y PRACTICAS PASADAS
	////////////////////////////////
	GLfloat muevex;
	GLfloat apagarPrenderLampara;
	GLfloat mueveHelicoptero;
	GLfloat rotaeny;
	GLfloat rotaenz;
	GLfloat articulacionCofre;
	////////////////////////////////

	// ACTIVIDADES PRACTICA 8
	/////////////////////////////////////////////
	GLfloat haciaAdelanteAtras;
	GLfloat inicio;
	GLfloat apagaPrende;
	GLfloat inicio2;
	/////////////////////////////////////////////

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

