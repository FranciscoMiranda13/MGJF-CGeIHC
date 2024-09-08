/*Pr�ctica 4: Modelado Jer�rquico.
Se implementa el uso de matrices adicionales para almacenar informaci�n de transformaciones geom�tricas que se quiere 
heredar entre diversas instancias para que est�n unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pir�mide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//funci�n para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los v�rtices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//funci�n para crear pir�mide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}

int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindro(20, 1.0f);//�ndice 2 en MeshList
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
	CreateShaders();

	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	// Para la grua
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glm::mat4 modelauxprismarec(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a // para el prisma 
	glm::mat4 modelauxpiramiderec(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a // para la piramide
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Para el perro
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glm::mat4 modelaux_Cuerpo_Perro(1.0);
	glm::mat4 modelaux_Cabeza_Perro(1.0);
	glm::mat4 modelaux_Cola_Perro(1.0);
	glm::mat4 modelaux_Pata_Perro_1(1.0);
	glm::mat4 modelaux_Pata_Perro_2(1.0);
	glm::mat4 modelaux_Pata_Perro_3(1.0);
	glm::mat4 modelaux_Pata_Perro_4(1.0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
				
		//para reiniciar la matriz de modelo con valor de la matriz identidad
		model = glm::mat4(1.0);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODIGO PARA LA GRUA. INICIO

		////AQU� SE DIBUJA LA CABINA, LA BASE, LAS 4 LLANTAS

		////prisma rectangular
		//model = glm::translate(model, glm::vec3(3.0f, 5.0f, -4.0f));//centro prisma rectangular

		//modelauxprismarec = model;//guarda las transformaciones anteriores

		//model = glm::scale(model, glm::vec3(6.0f, 2.0f, 2.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())); 
		//color = glm::vec3(1.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); 

		//model = modelauxprismarec;// a partir del centro del prisma rectangular (ESTE ES MI INICIO) <----------------------------------

		////piramide cuadrangular
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));//centro de la piramide cuadrangular

		//modelauxpiramiderec = model; //guarda las transformaciones anteriores

		//model = glm::scale(model, glm::vec3(6.0f, 2.0f, 4.0f));// escala piramide
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//color = glm::vec3(0.5f, 0.5f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[4]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular
		////meshList[2]->RenderMeshGeometry(); //dibuja las figuras geom�tricas cilindro y cono

		//model = modelauxpiramiderec;//a partir del centro de la piramide (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		////llanta 1. C
		//model = glm::translate(model, glm::vec3(-3.0f, -2.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacionllanta1()), glm::vec3(0.0f, 1.0f, 0.0f)); 

		//model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.18f, 0.2f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxpiramiderec;//a partir del centro de la piramide (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		////llanta 2. V
		//model = glm::translate(model, glm::vec3(3.0f, -2.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacionllanta2()), glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.18f, 0.2f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxpiramiderec;//a partir del centro de la piramide (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		////llanta 3. B
		//model = glm::translate(model, glm::vec3(-3.0f, -2.0f, -2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacionllanta3()), glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.18f, 0.2f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxpiramiderec;//a partir del centro de la piramide (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		////llanta 4. N
		//model = glm::translate(model, glm::vec3(3.0f, -2.0f, -2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacionllanta4()), glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.18f, 0.2f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry();

		//model = modelauxprismarec;// a partir del centro del prisma rectangular (ESTE ES MI INICIO) <----------------------------------

		//// SE EMPIEZA EL DIBUJO DEL BRAZO

		//// F G H J
		////articulaci�n 1
		//model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 0.0f)); //aqui empieza la articulaci�n
		////rotaci�n alrededor de la articulaci�n que une con la cabina (prisma rectangular)
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));

		//modelauxprismarec = model;//guarda las transformaciones anteriores

		////dibujar una peque�a esfera
		////no queremos la informacion de la esfera
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		//model = modelauxprismarec; //a partir de la articulacion 1

		////primer brazo que conecta con la cabina (prisma rectangular)
		//
		//model = glm::translate(model, glm::vec3(-2.0f, 2.0f, 0.0f));// al centro del brazo 1
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //a partir de su centro lo rotamos en z

		//modelauxprismarec = model; //guarda las transformaciones anteriores

		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));// escala del brazo 1
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//color = glm::vec3(1.0f, 0.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular
		////meshList[2]->RenderMeshGeometry(); //dibuja las figuras geom�tricas cilindro y cono

		////para descartar la escala que no quiero heredar se carga la informaci�n de la matrix auxiliar
		//model = modelauxprismarec;//a partir del centro del brazo 1

		//// los ejes de rotaron al rededor de z
		//// por eso cambian
		////articulaci�n 2
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));//en ese punto rotara la segunda articulacion

		//modelauxprismarec = model;//guarda las transformaciones anteriores

		////dibujar una peque�a esfera
		////no queremos la informacion de la esfera
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		//model = modelauxprismarec; //a partir de la articulacion 2

		////segundo brazo
		//model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));//centro del brazo 2

		//modelauxprismarec = model;//guarda las transformaciones anteriores

		//model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

		//model = modelauxprismarec;// a partir del centro del brazo 2

		////articulaci�n 3 extremo derecho del segundo brazo
		//model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));

		//modelauxprismarec = model; //guarda las transformaciones anteriores

		////dibujar una peque�a esfera
		////no queremos la informacion de la esfera
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		//model = modelauxprismarec; //a partir de la articulacion 3

		////tercer brazo
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));//centro del brazo 3

		//modelauxprismarec = model;//guarda las transformaciones anteriores

		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

		//model = modelauxprismarec;// a partir del centro del brazo 3

		////articulaci�n 4 
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //a partir de su centro lo rotamos en z
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));

		//modelauxprismarec = model; //guarda las transformaciones anteriores

		////dibujar una peque�a esfera
		////no queremos la informacion de la esfera
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		//model = modelauxprismarec; //a partir de la articulacion 4

		////canasta
		//model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));//centro de la canasta

		//modelauxprismarec = model;//guarda las transformaciones anteriores 

		//model = glm::scale(model, glm::vec3(1.5f, 3.0f, 1.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos 
		//meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular 

		//model = modelauxprismarec;// a partir del centro de la canasta 

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODIGO PARA LA GRUA. FIN

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODIGO PARA EL PERRO. INICIO

		//prisma rectangular (cuerpo del perro)
		model = glm::translate(model, glm::vec3(0.0f, 7.0f, -6.0f));//centro prisma rectangular (cuerpo del perro)

		modelaux_Cuerpo_Perro = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(12.0f, 6.0f, 6.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.65f, 0.32f, 0.16f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_Cuerpo_Perro;// a partir del centro del prisma rectangular (cuerpo del perro) (ESTE ES MI INICIO) <----------------------------------

		//prisma rectangular (cabeza)
		model = glm::translate(model, glm::vec3(-3.0f, 6.0f, 0.0f));//centro del prisma rectangular (cebeza)

		modelaux_Cabeza_Perro = model; //guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));// escala piramide
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		color = glm::vec3(0.65f, 0.32f, 0.16f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux_Cabeza_Perro;//a partir del centro del prisma rectangular (cabeza) (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		//prisma rectangular (hocico)
		model = glm::translate(model, glm::vec3(-5.0f, -1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, glm::vec3(1.0f, 3.99f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Cabeza_Perro;//a partir del centro del prisma rectangular (cabeza) (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		//prisma rectangular (ojo 1)
		model = glm::translate(model, glm::vec3(-3.5f, 1.0f, 1.5f));

		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_Cabeza_Perro;//a partir del centro del prisma rectangular (cabeza) (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		//prisma rectangular (ojo 2)
		model = glm::translate(model, glm::vec3(-3.5f, 1.0f, -1.5f));

		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_Cabeza_Perro;//a partir del centro del prisma rectangular (cabeza) (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		//piramide rectangular (oreja 1)
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, 1.5f));

		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[4]->RenderMesh();

		model = modelaux_Cabeza_Perro;//a partir del centro del prisma rectangular (cabeza) (ESTE ES MI SEGUNDO INICO) <-----------------------------------

		//piramide rectangular (oreja 2)
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -1.5f));

		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[4]->RenderMesh();

		model = modelaux_Cuerpo_Perro;// a partir del centro del prisma rectangular (cuerpo del perro) (ESTE ES MI INICIO) <----------------------------------

		//cola esfera
		model = glm::translate(model, glm::vec3(7.0f, 2.0f, 0.0f)); //aqui empieza la articulaci�n de la cola
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacioncola()), glm::vec3(1.0f, 0.0f, 0.0f)); // Mover con: P

		modelaux_Cola_Perro = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Cola_Perro; // a partir de la articulacion de la cola (esfera) (ESTE ES MI TERCER INICIO. 1) <----------------------------------

		//cola cilindro
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));

		modelaux_Cola_Perro = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Cola_Perro; // a partir de el centro de la cola (cilindro) (ESTE ES MI TERCER INICIO. 2) <----------------------------------

		//cola cono
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));

		modelaux_Cola_Perro = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(0.5f, 2.0f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[3]->RenderMeshGeometry();

		model = modelaux_Cuerpo_Perro;// a partir del centro del prisma rectangular (cuerpo del perro) (ESTE ES MI INICIO) <----------------------------------

		//pata 1 esfera 1
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 4.0f)); 
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionpata1_1()), glm::vec3(0.0f, 0.0f, 1.0f)); // Mover con: U

		modelaux_Pata_Perro_1 = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Pata_Perro_1;// a partir de la articulacion 1 de la pata 1 (esfera) (ESTE ES MI CUARTO INICIO. 1) <----------------------------------

		//pata 1 cilindro 1
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux_Pata_Perro_1 = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Pata_Perro_1;// a partir del centro de la pata 1 (cilindro) (ESTE ES MI CUARTO INICIO. 2) <----------------------------------

		//pata 1 esfera 2
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionpata1_2()), glm::vec3(0.0f, 0.0f, 1.0f)); // Mover con: I

		modelaux_Pata_Perro_1 = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Pata_Perro_1;// a partir de la articulacion 2 de la pata 1 (esfera) (ESTE ES MI CUARTO INICIO. 3) <----------------------------------

		//pata 1 cilindro 2
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));

		modelaux_Pata_Perro_1 = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Cuerpo_Perro;// a partir del centro del prisma rectangular (cuerpo del perro) (ESTE ES MI INICIO) <----------------------------------

		//pata 2 esfera 1
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 4.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionpata2_1()), glm::vec3(0.0f, 0.0f, 1.0f)); // Mover con: O

		modelaux_Pata_Perro_2 = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Pata_Perro_2;// a partir de la articulacion 1 de la pata 2 (esfera) (ESTE ES MI QUINTO INICIO. 1) <----------------------------------

		//pata 2 cilindro 1
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux_Pata_Perro_2 = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Pata_Perro_2;// a partir del centro de la pata 2 (cilindro) (ESTE ES MI QUINTO INICIO. 2) <----------------------------------

		//pata 2 esfera 2
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionpata2_2()), glm::vec3(0.0f, 0.0f, 1.0f)); // Mover con: P

		modelaux_Pata_Perro_2 = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Pata_Perro_2;// a partir de la articulacion 2 de la pata 2 (esfera) (ESTE ES MI QUINTO INICIO. 3) <----------------------------------

		//pata 2 cilindro 2
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));

		modelaux_Pata_Perro_2 = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Cuerpo_Perro;// a partir del centro del prisma rectangular (cuerpo del perro) (ESTE ES MI INICIO) <----------------------------------

		//pata 3 esfera 1
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionpata3_1()), glm::vec3(0.0f, 0.0f, 1.0f)); // Mover con: Z

		modelaux_Pata_Perro_3 = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Pata_Perro_3;// a partir de la articulacion 1 de la pata 3 (esfera) (ESTE ES MI SEXTO INICIO. 1) <----------------------------------

		//pata 3 cilindro 1
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux_Pata_Perro_3 = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Pata_Perro_3;// a partir del centro de la pata 3 (cilindro) (ESTE ES MI SEXTO INICIO. 2) <----------------------------------

		//pata 3 esfera 2
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionpata3_2()), glm::vec3(0.0f, 0.0f, 1.0f)); // Mover con: X

		modelaux_Pata_Perro_3 = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Pata_Perro_3;// a partir de la articulacion 2 de la pata 3 (esfera) (ESTE ES MI SEXTO INICIO. 3) <----------------------------------

		//pata 3 cilindro 2
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));

		modelaux_Pata_Perro_3 = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Cuerpo_Perro;// a partir del centro del prisma rectangular (cuerpo del perro) (ESTE ES MI INICIO) <----------------------------------

		//pata 4 esfera 1
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionpata4_1()), glm::vec3(0.0f, 0.0f, 1.0f)); // Mover con: Q

		modelaux_Pata_Perro_4 = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Pata_Perro_4;// a partir de la articulacion 1 de la pata 4 (esfera) (ESTE ES MI SEPTIMO INICIO. 1) <----------------------------------

		//pata 4 cilindro 1
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux_Pata_Perro_4 = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();

		model = modelaux_Pata_Perro_4;// a partir del centro de la pata 4 (cilindro) (ESTE ES MI SEPTIMO INICIO. 2) <----------------------------------

		//pata 4 esfera 2
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionpata4_2()), glm::vec3(0.0f, 0.0f, 1.0f)); // Mover con: L

		modelaux_Pata_Perro_4 = model;//guarda las transformaciones anteriores

		//dibujar una peque�a esfera
		//no queremos la informacion de la esfera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_Pata_Perro_4;// a partir de la articulacion 2 de la pata 4 (esfera) (ESTE ES MI SEPTIMO INICIO. 3) <----------------------------------

		//pata 4 cilindro 2
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));

		modelaux_Pata_Perro_4 = model;//guarda las transformaciones anteriores

		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODIGO PARA EL PERRO. FIN


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
