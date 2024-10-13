#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	// PARA EJERCICIOS Y PRACTICAS PASADAS
	//////////////////////////////////
	muevex = 2.0f;
	apagarPrenderLampara = false;
	mueveHelicoptero = 0.0f;
	rotaeny = 0.0f;
	rotaenz = 0.0f;
	articulacionCofre = 0.0f;
	//////////////////////////////////

	// ACTIVIDADES PRACTICA 8
	//////////////////////////////////////
	haciaAdelanteAtras = false;
	inicio = false;
	apagaPrende = false;
	inicio2 = false;
	//////////////////////////////////////

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 8: Iluminacion 2", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// PARA EJERCICIOS Y PRACTICAS PASADAS
	///////////////////////////////////////////////
	// PARA LAS SPOTLIGHT
	// ADELANTE
	if (key == GLFW_KEY_Y)
	{
		theWindow->haciaAdelanteAtras = false;
		theWindow->inicio = true;
		theWindow-> muevex += 1.0;
	}
	// ATRAS
	if (key == GLFW_KEY_U)
	{
		theWindow->haciaAdelanteAtras = true;
		theWindow->inicio = true;
		theWindow-> muevex -= 1.0;
	}
	// REINICIO
	if (key == GLFW_KEY_L) 
	{
		theWindow->inicio = false;
	}
	// PARA LAS POINTLIGHT
	// LAMPARA
	if (key == GLFW_KEY_B)
	{
		theWindow->apagarPrenderLampara = true;
		theWindow->inicio2 = true;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->apagarPrenderLampara = false;
	}
	// DIAMANTE
	if (key == GLFW_KEY_F) 
	{
		theWindow->apagaPrende = true;
		theWindow->inicio2 = true;
	}
	if (key == GLFW_KEY_G)
	{
		theWindow->apagaPrende = false;
	}
	// REINICIO
	if (key == GLFW_KEY_Q)
	{
		theWindow->inicio2 = false;
		theWindow->apagarPrenderLampara = false;
		theWindow->apagaPrende = false;
	}
	// HELICOPTERO
	if (key == GLFW_KEY_I)
	{
		theWindow->mueveHelicoptero += 1.0;
	}
	if (key == GLFW_KEY_O)
	{
		theWindow->mueveHelicoptero -= 1.0;
	}
	// ROTACIONES
	if (key == GLFW_KEY_P)
	{
		theWindow->rotaeny -= 1.0;
	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->rotaenz -= 1.0;
	}
	// COFRE
	if (key == GLFW_KEY_C) 
	{
		if (theWindow->articulacionCofre >= 0.0)
		{
		}
		else
		{
			theWindow->articulacionCofre += 10.0;
		}
	}
	if (key == GLFW_KEY_M)
	{
		if (theWindow->articulacionCofre <= -60.0)
		{
		}
		else
		{
			theWindow->articulacionCofre -= 10.0;
		}
	}
	///////////////////////////////////////////////

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
