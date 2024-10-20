
/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/

// NO SE CONSIDERA ANIMACION UNA SOLA TRASNFORMACION

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"


// PARA ELEGIR LOS GRADOS DE MANERA RANDOM
//////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
//////////////////////////////////////////////////////

const float toRadians = 3.14159265f / 180.0f;

// variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;

// EJERCICIO 09
// VARIABLES PARA ANIMACION
/////////////////////////////////////////////////////////
float subeBaja;
float subeBajaOffset;
float rotaLetrero;
float rotaLetreroOffset;
bool inicia;
float tiempoInicio;
/////////////////////////////////////////////////////////

// PRACTICA 09
// VARIABLES PARA ANIMACION
// DADO
/////////////////////////////////////////////////////////
float subeBajaDado;
float subeBajaDadoOffset;

float rotaDadox;
float rotaDadoxOffset;
float rotaDadoy;
float rotaDadoyOffset;
float rotaDadoz;
float rotaDadozOffset;
/////////////////////////////////////////////////////////

// PRACTICA 09
// VARIABLES PARA ANIMACION
// VEHICULO
/////////////////////////////////////////////////////////
float mueveVehiculo;
float mueveVehiculoOffset;

float mueveVehiculo2;
float mueveVehiculoOffset2;

float mueveVehiculo3;
float mueveVehiculoOffset3;

float mueveVehiculo4;
float mueveVehiculoOffset4;

float rotaHelice;
float rotaHeliceOffset;

float rotaVehiculo;
float rotaVehiculoOffset;

float rotaVehiculo2;
float rotaVehiculoOffset2;

float rotaVehiculo3;
float rotaVehiculoOffset3;

float rotaVehiculo4;
float rotaVehiculoOffset4;

bool comenzar;
/////////////////////////////////////////////////////////

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

// EJERCICIO 09
// TEXTURAS
////////////////////////// 
Texture ladrillos;
Texture negro;
Texture rojo;
Texture blanco;
/////////////////////////

// PRACTICA 09
// TEXTURAS
////////////////////////////
Texture dado10Caras;
Texture H1;
Texture	H2;
Texture	H3;
Texture	H4;
Texture	H5;
Texture colorBlancoCasilla;
////////////////////////////

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

// EJERCICIO 09
// MODELOS
/////////////////////////// 
Model arco;
Model arcoLetrero;
///////////////////////////

// PRACTICA 09
// MODELOS
////////////////////////////
Model casilla;
Model helicoptero;
Model helices;
////////////////////////////

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {

		// 1
		0, 1, 2,
		// 2
		3, 4, 5,
		// 3
		6, 7, 8,
		// 4
		9, 10, 11,
		// 5
		12, 13, 14,
		// 6
		15, 16, 17,
		// 7
		18, 19, 20,
		// 8
		21, 22, 23,
		// 9
		24, 25, 26,
		// 10
		27, 28, 29,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
	// average normals
	GLfloat cubo_vertices[] = {

		// 1
		//x		y		z		S		T			NX		NY		NZ
		0.49f, 0.17f,  0.0f,	0.40f,  0.85f,		-1.0f,	-1.0f,	-1.0f,
		0.0f,  0.52f,  0.0f,	0.55f,	0.85f,		-1.0f,	-1.0f,	-1.0f,
		0.0f,  0.0f,   1.0f,	0.50f,	0.99f,		-1.0f,	-1.0f,	-1.0f,

		// 2
		//x		y		z		S		T			NX		NY		NZ
		-0.49f, 0.17f,  0.0f,	0.60f,  0.02f,		1.0f,	-1.0f,	-1.0f,
		0.0f,   0.52f,  0.0f,	0.44f,	0.02f,		1.0f,	-1.0f,	-1.0f,
		0.f,    0.f,  1.0f,		0.5f,	0.20f,		1.0f,	-1.0f,	-1.0f,

		// 3
		//x		y		z		S		T			NX		NY		NZ
		-0.49f, 0.17f,  0.0f,	0.4f,  0.52f,		1.0f,	1.0f,	-1.0f,
		-0.3f,  -0.4f,  0.0f,	0.60f,	0.52f,		1.0f,	1.0f,	-1.0f,
		0.f,    0.f,   1.0f,	0.5f,	0.70f,		1.0f,	1.0f,	-1.0f,

		// 4
		//x		y		z		S		T			NX		NY		NZ
		0.3f, -0.4f,  0.0f,	   0.60f,  0.38f,		0.0f,	1.0f,	-1.0f,
		-0.3f, -0.4f, 0.0f,    0.40f,  0.38f,		0.0f,	1.0f,	-1.0f,
		0.f,   0.f,   1.0f,	   0.5f,   0.49f,		0.0f,	1.0f,	-1.0f,

		// 5
		//x		y		z		S		T			NX		NY		NZ
		0.3f,  -0.4f,  0.0f,	0.65f,  0.63f,		-1.0f,	1.0f,	-1.0f,
		0.49f,  0.17f, 0.0f,	0.85f,	0.63f,		-1.0f,	1.0f,	-1.0f,
		0.f,    0.f,   1.0f,	0.75f,	0.745f,		-1.0f,	1.0f,	-1.0f,

		// 6
		//x		y		z		S		T			NX		NY		NZ
		0.49f, 0.17f,  0.0f,	0.85f,  0.27f,		-1.0f,	-1.0f,	1.0f,
		0.0f,  0.52f,  0.0f,	0.65f,  0.27f,		-1.0f,	-1.0f,	1.0f,
		0.f,   0.f,    -1.0f,	0.725f,	0.43f,		-1.0f,	-1.0f,	1.0f,

		// 7
		//x		y		z		S		T			NX		NY		NZ
		-0.49f, 0.17f,  0.0f,	0.64f,  0.75f,		1.0f,	-1.0f,	1.0f,
		0.0f,   0.52f,  0.0f,	0.85f,  0.75f,		1.0f,	-1.0f,	1.0f,
		0.f,    0.f,    -1.0f,	0.75f,  0.95f,		1.0f,	-1.0f,	1.0f,

		// 8
		//x		y		z		S		T			NX		NY		NZ
		-0.49f, 0.17f,  0.0f,	0.37f,  0.12f,		1.0f,	1.0f,	1.0f,
		-0.3f, -0.4f,   0.0f,	0.14f,	0.12f,		1.0f,	1.0f,	1.0f,
		0.f,    0.f,   -1.0f,	0.25f,	0.245f,		1.0f,	1.0f,	1.0f,

		// 9
		//x		y		z		S		T			NX		NY		NZ
		0.3f,  -0.4f,  0.0f,   0.1f,   0.75f,		0.0f,	1.0f,	1.0f,
		-0.3f, -0.4f,  0.0f,   0.35f,  0.75f,		0.0f,	1.0f,	1.0f,
		0.f,    0.f,  -1.0f,   0.24f,  0.90f,		0.0f,	1.0f,	1.0f,

		// 5
		//x		y		z		S		T			NX		NY		NZ
		0.3f, -0.4f,  0.0f,	   0.89f,  0.11f,		-1.0f,	1.0f,	1.0f,
		0.49f, 0.17f, 0.0f,	   0.61f,  0.11f,		-1.0f,	1.0f,	1.0f,
		0.f,   0.f,  -1.0f,	   0.75f,  0.245f,		-1.0f,	1.0f,	1.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 240, 36);
	meshList.push_back(dado);

}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	// EJERCICIO 09
	// CARGAR TEXTURAS
	/////////////////////////////////////////////////////////
	ladrillos = Texture("Textures/textura-01.png");
	ladrillos.LoadTextureA();
	negro = Texture("Textures/textura-02.png");
	negro.LoadTextureA();
	rojo = Texture("Textures/textura-03.png");
	rojo.LoadTextureA();
	blanco = Texture("Textures/textura-04.png");
	blanco.LoadTextureA();
	/////////////////////////////////////////////////////////

	// PRACTICA 09
	// CAGAR TEXTURAS
	//////////////////////////////////////////////////////
	dado10Caras = Texture("Textures/dado10CarasGIMP.png");
	dado10Caras.LoadTextureA();
	H1 = Texture("Textures/body.png");
	H1.LoadTextureA();
	H2 = Texture("Textures/P_body.png");
	H2.LoadTextureA();
	H3 = Texture("Textures/P_tail.png");
	H3.LoadTextureA();
	H4 = Texture("Textures/pely_a.png");
	H4.LoadTextureA();
	H5 = Texture("Textures/prope.png");
	H5.LoadTextureA();
	colorBlancoCasilla = Texture("Textures/colorBlanco.png");
	colorBlancoCasilla.LoadTextureA();
	//////////////////////////////////////////////////////

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	// EJERCICIO 09
	// CARGAR MODELOS
	///////////////////////////////////////////////////
	arco = Model();
	arco.LoadModel("Models/arco.obj");
	arcoLetrero = Model();
	arcoLetrero.LoadModel("Models/letreroArco.obj");
	///////////////////////////////////////////////////

	// PRACTICA 09
	// CAGAR MODELOS
	//////////////////////////////////////////////////////
	casilla = Model();
	casilla.LoadModel("Models/casilla.obj");
	helicoptero = Model();
	helicoptero.LoadModel("Models/helicoptero.obj");
	helices = Model();
	helices.LoadModel("Models/helices.obj");
	//////////////////////////////////////////////////////

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.5f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glfwSetTime(0);

	movCoche = 0.0f;
	/*movOffset = 0.03f; */ //SUBIR LA VELOCIDAD (CON ESO ES MAS RAPIDO EL MOVIMIENTO)
	movOffset = 0.3f; 
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;
	avanza = true; 

	// EJERCICIO 09
	///////////////////////////
	subeBaja = 4.0f;
	subeBajaOffset = 0.0966f;
	rotaLetrero = 0.0f;
	rotaLetreroOffset = 2.952;
	inicia = true;
	tiempoInicio = 0.0f;
	///////////////////////////

	// PRACTICA 09
	// DADO
	///////////////////////////
	subeBajaDado = 20.0f;
	subeBajaDadoOffset = 0.03;

	rotaDadox = 0.0f;
	rotaDadoxOffset = 1.0;

	rotaDadoy = 0.0f;
	rotaDadoyOffset = 1.0;

	rotaDadoz = 0.0f;
	rotaDadozOffset = 1.0;
	///////////////////////////

	// PRACTICA 09
	// GRADOS ALEATORIOS 
	/////////////////////////////////////////////////////////////////
	
	// Inicializar la semilla solo una vez
	std::srand(static_cast<unsigned>(std::time(0)));

	// PARA GRADOS DE Y
	std::vector<int> listaY = { 180, -180 };
	int gradosDeY = listaY[std::rand() % listaY.size()];

	// PARA GRADOS DE Z
	std::vector<int> listaZ = { 36, 108, 180, 252, 324, -36, -108, -180, -252, -324 };
	int gradosDeZ = listaZ[std::rand() % listaZ.size()];

	// PARA GRADOS DE X
	std::vector<int> listaX = { 25,-25 };
	int gradosDeX = listaX[std::rand() % listaX.size()];

	/////////////////////////////////////////////////////////////////

	// PRACTICA 09
	// VEHICULO
	///////////////////////////
	mueveVehiculo = 0.0f;
	mueveVehiculoOffset = 0.5f;

	mueveVehiculo2 = 0.0f;
	mueveVehiculoOffset2 = 0.5f;

	mueveVehiculo3 = 0.0f;
	mueveVehiculoOffset3 = 0.5f;

	mueveVehiculo4 = 0.0f;
	mueveVehiculoOffset4 = 0.5f;

	rotaHelice = 0.0f;
	rotaHeliceOffset = 2.0f;

	rotaVehiculo = 0.0f;
	rotaVehiculoOffset = 1.0f;

	rotaVehiculo2 = 0.0f;
	rotaVehiculoOffset2 = 1.0f;

	rotaVehiculo3 = 0.0f;
	rotaVehiculoOffset3 = 1.0f;

	rotaVehiculo4 = 0.0f;
	rotaVehiculoOffset4 = 1.0f;

	comenzar = true;;
	///////////////////////////

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//if (glfwGetTime() > 5)
		//{
		//	if (avanza)
		//	{
		//		if (movCoche > -305.0f)
		//		{
		//			movCoche -= movOffset * deltaTime;
		//			rotllanta += rotllantaOffset * deltaTime;
		//			//printf("avanza%f \n ",movCoche);
		//		}
		//		else
		//		{
		//			avanza = !avanza;
		//		}
		//	}
		//	else
		//	{
		//		if (movCoche < 285.0f)
		//		{
		//			movCoche += movOffset * deltaTime;
		//			rotllanta -= rotllantaOffset * deltaTime;
		//			//printf("avanza%f \n ",movCoche);
		//		}
		//		else
		//		{
		//			avanza = !avanza;
		//		}
		//	}
		//}

		//if ((glfwGetTime() - tiempoInicio) < 2.0f)
		//{
		//	rotaLetrero += rotaLetreroOffset * deltaTime;
		//}
		//else
		//{
		//	/*printf("angulo = %f \n ", rotaLetrero);*/
		//}

		//if ((glfwGetTime() - tiempoInicio) > 2.0f)
		//{
		//	/*printf("Tiempo 1 : %f", glfwGetTime());*/
		//	if (inicia)
		//	{
		//		if (subeBaja > -7.8f) 
		//		{
		//			subeBaja -= subeBajaOffset * deltaTime; 
		//		}
		//		else
		//		{
		//			/*printf("Tiempo 2 : ------------> %f", glfwGetTime());*/
		//			/*glfwSetTime(0); */
		//			tiempoInicio = glfwGetTime();
		//			inicia = !inicia; 
		//		}
		//	}
		//	else
		//	{
		//		if (subeBaja < 4.0f) 
		//		{
		//			subeBaja += subeBajaOffset * deltaTime; 
		//		}
		//		else
		//		{
		//			/*printf("Tiempo 3 : ------------> %f", glfwGetTime());*/
		//			/*glfwSetTime(0);*/
		//			tiempoInicio = glfwGetTime();
		//			inicia = !inicia;
		//		}
		//	}
		//}

		
		// PARA BAJAR
		// Y
		if (mainWindow.getsubeBajaDado() == true)
		{
			// HACER QUE EL DADO BAJE
			if (subeBajaDado > 1)
			{
				subeBajaDado -= subeBajaOffset * deltaTime;
			}
			// CUANDO LLEGUE AL FINAL
			else
			{
				// ROTAR EN Y
				switch (gradosDeY)
				{
				case 180:
					if (rotaDadoy < 180)
					{
						rotaDadoy += rotaDadoyOffset * deltaTime;
					}
					else
					{
						// ROTAR EN Z
						switch (gradosDeZ)
						{
						case 36:
							if (rotaDadoz < 36)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								// ROTAR EN X
								// ELEGIR EL NUMERO DEL LADO
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -36:
							if (rotaDadoz > -36)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case 108:
							if (rotaDadoz < 108)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -108:
							if (rotaDadoz > -108)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case 180:
							if (rotaDadoz < 180)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -180:
							if (rotaDadoz > -180)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case 252:
							if (rotaDadoz < 252)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -252:
							if (rotaDadoz > -252)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case 324:
							if (rotaDadoz < 324)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -324:
							if (rotaDadoz > -324)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						}
					}
					break;
				case -180:
					if (rotaDadoy > -180)
					{
						rotaDadoy -= rotaDadoyOffset * deltaTime;
					}
					else
					{
						switch (gradosDeZ)
						{
						case 36:
							if (rotaDadoz < 36)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -36:
							if (rotaDadoz > -36)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case 108:
							if (rotaDadoz < 108)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -108:
							if (rotaDadoz > -108)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case 180:
							if (rotaDadoz < 180)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -180:
							if (rotaDadoz > -180)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case 252:
							if (rotaDadoz < 252)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -252:
							if (rotaDadoz > -252)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case 324:
							if (rotaDadoz < 324)
							{
								rotaDadoz += rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						case -324:
							if (rotaDadoz > -324)
							{
								rotaDadoz -= rotaDadozOffset * deltaTime;
							}
							else
							{
								switch (gradosDeX)
								{
								case 25:
									if (rotaDadox < 25)
									{
										rotaDadox += rotaDadoxOffset * deltaTime;
									}
									break;
								case -25:
									if (rotaDadox > -25)
									{
										rotaDadox -= rotaDadoxOffset * deltaTime;
									}
									break;
								}
							}
							break;
						}
					}
					break;
				}
			}
		}

		// PARA SUBIR
		// U
		if (mainWindow.getsubeBajaDado() == false)
		{
			if (subeBajaDado < 20)
			{
				subeBajaDado += subeBajaOffset * deltaTime;
			}
			else
			{
				// REGRESAR A SU ESTADO ORIGINAL Y REASIGNAR LOS VALORES DE LOS GRADOS
				gradosDeY = listaY[std::rand() % listaY.size()];
				rotaDadoy = 0.0f;
				gradosDeZ = listaZ[std::rand() % listaZ.size()];
				rotaDadoz = 0.0f;
				gradosDeX = listaX[std::rand() % listaX.size()];
				rotaDadox = 0.0f;
			}
		}

		// VEHICULO
		if (glfwGetTime() > 5)
		{
			if (comenzar)
			{
				if (mueveVehiculo < 60)
				{
					mueveVehiculo += mueveVehiculoOffset * deltaTime;
					rotaHelice += rotaHeliceOffset * deltaTime;
				}
				else
				{
					if (rotaVehiculo > -90)
					{
						rotaVehiculo -= rotaVehiculoOffset * deltaTime;
						rotaHelice += rotaHeliceOffset * deltaTime;
					}
					else
					{
						if (mueveVehiculo2 < 76)
						{
							mueveVehiculo2 += mueveVehiculoOffset2 * deltaTime;
							rotaHelice += rotaHeliceOffset * deltaTime;
						}
						else
						{
							if (rotaVehiculo2 > -90)
							{
								rotaVehiculo2 -= rotaVehiculoOffset2 * deltaTime;
								rotaHelice += rotaHeliceOffset * deltaTime;
							}
							else
							{
								if (mueveVehiculo3 < 60)
								{
									mueveVehiculo3 += mueveVehiculoOffset3 * deltaTime;
									rotaHelice += rotaHeliceOffset * deltaTime;
								}
								else
								{
									if (rotaVehiculo3 > -90)
									{
										rotaVehiculo3 -= rotaVehiculoOffset3 * deltaTime;
										rotaHelice += rotaHeliceOffset * deltaTime;
									}
									else
									{
										if (mueveVehiculo4 < 76)
										{
											mueveVehiculo4 += mueveVehiculoOffset4 * deltaTime;
											rotaHelice += rotaHeliceOffset * deltaTime;
										}
										else
										{
											if (rotaVehiculo4 > -90)
											{
												rotaVehiculo4 -= rotaVehiculoOffset4 * deltaTime;
												rotaHelice += rotaHeliceOffset * deltaTime;
											}
											else
											{
												comenzar = !comenzar;
											}
										}
									}
								}
							}
						}
					}
				}

			}
			else
			{
				// REINICAR VALORES
				mueveVehiculo = 0.0f;
				mueveVehiculo2 = 0.0f;
				mueveVehiculo3 = 0.0f;
				mueveVehiculo4 = 0.0f;

				rotaVehiculo = 0.0f;
				rotaVehiculo2 = 0.0f;
				rotaVehiculo3 = 0.0f;
				rotaVehiculo4 = 0.0f;
				
				comenzar = !comenzar;
			}
		}		

		// QUE GIRE SOBRE X

		// QUE AL CAER EL DADO GIRE EN EL SUELO O QUE VAYA GIRANDO AL CAER

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);

		// EJERCICIO 09
		// MATRIZ AUXILIAR PARA EL ARCO
		//////////////////////////////////
		glm::mat4 modelauxArco(1.0);
		//////////////////////////////////

		// PRACTICA 09
		// MATRIZ AUXILIAR PARA CASILLAS
		//////////////////////////////////
		glm::mat4 modelauxCasilla(1.0);
		//////////////////////////////////

		// PRACTICA 09
		// MATRIZ AUXILIAR PARA EL HELICOPTERO
		//////////////////////////////////
		glm::mat4 modelauxHelicoptero(1.0);
		//////////////////////////////////

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		// PISO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f)); 
		// VA DE -300 A 300 (600), PORQUE SE ESCALO
		// AL INICIO SOLO VALE 20 (DE -10 A 10)
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// PRACTICA 09
		// DADO 10 CARAS
		/////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, subeBajaDado, 15.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::rotate(model, rotaDadox * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::rotate(model, rotaDadoy * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotaDadoz * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10Caras.UseTexture(); // CAMBIAMOS POR NUESTRA TEXTURA
		meshList[4]->RenderMesh();
		/////////////////////////////////////////////////////////////////////////////////////

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		// helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// EJERCICIO 09
		// ARCO
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ARCO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 7.8f, -50.0));

		modelauxArco = model; // GUARDAR

		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arco.RenderModel();

		model = modelauxArco; // A PARTIR DEL ARCO

		// LETRERO
		// 7.8f + 4.0f = 11.8f -> distancia 
		model = glm::translate(model, glm::vec3(0.0f, subeBaja, 0.4));
		model = glm::rotate(model, rotaLetrero * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arcoLetrero.RenderModel();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// PRACTICA 09
		// CASILLAS
		/////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, 20.0f));

		modelauxCasilla = model;

		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casilla.RenderModel();

		model = modelauxCasilla;

		model = glm::translate(model, glm::vec3(-28.0f, 0.0f, -20.0f));

		modelauxCasilla = model;

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 9.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casilla.RenderModel();

		model = modelauxCasilla;

		model = glm::translate(model, glm::vec3(28.0f, 0.0f, -20.0f));

		modelauxCasilla = model;

		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casilla.RenderModel();

		model = modelauxCasilla;

		model = glm::translate(model, glm::vec3(28.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 9.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casilla.RenderModel();
		/////////////////////////////////////////////////////////////////////////////////////////////////

		// PRACTICA 09
		// VEHICULO
		/////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-38.0f, 10.0f, 30.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// ESTAN UBICADOS DESPUES DE LAS TRANSFORMACIONES ANTERIORES
		// POR LO TANTO A PARTIR DE ESAS COMENZAMOS

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mueveVehiculo));
		model = glm::rotate(model, rotaVehiculo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// ESTAN UBICADOS DESPUES DE LAS TRANSFORMACIONES ANTERIORES
		// POR LO TANTO A PARTIR DE ESAS COMENZAMOS

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mueveVehiculo2));
		model = glm::rotate(model, rotaVehiculo2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// ESTAN UBICADOS DESPUES DE LAS TRANSFORMACIONES ANTERIORES
		// POR LO TANTO A PARTIR DE ESAS COMENZAMOS

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mueveVehiculo3));
		model = glm::rotate(model, rotaVehiculo3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// ESTAN UBICADOS DESPUES DE LAS TRANSFORMACIONES ANTERIORES
		// POR LO TANTO A PARTIR DE ESAS COMENZAMOS
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mueveVehiculo4));
		model = glm::rotate(model, rotaVehiculo4 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		modelauxHelicoptero = model;

		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helicoptero.RenderModel();

		model = modelauxHelicoptero;

		model = glm::translate(model, glm::vec3(0.0f, 5.8f, -2.0f));
		model = glm::rotate(model, rotaHelice * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helices.RenderModel();
		/////////////////////////////////////////////////////////////////////////////////////////////////

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
