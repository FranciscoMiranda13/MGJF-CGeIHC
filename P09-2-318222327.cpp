
/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
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

const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0f;
float angulovaria = 0.0f;

// EJERCICIO CLASE
float tiempoInicial = 0.0f;
float tiempoInicial2 = 0.0f;
float toffsetnumerov2 = 0.0f;
int conta = 0;


// ANIMACIÓN DEL LETRERO
float toffsetnumerouLetrero = 1.0f;
float toffsetnumerovLetrero = 0.2f;

// ANIMACIÓN DE LAS ALAS
float rotaAla01 = 0.0f;
float rotaAla01Offset = 0.8f;
float rotaAla02 = 0.0f;
float rotaAla02Offset = 0.8f;
bool bajasube01 = true;

// ANIMACION DEL DRAGON (TRASLACIONES)
float mueveDragon = 0.0f;
float mueveDragonOffset = 0.1f;
float mueveDragon2 = 0.0f;
float mueveDragonOffset2 = 0.1f;
float mueveDragon3 = 0.0f;
float mueveDragonOffset3 = 0.1f;
float mueveDragon4 = 0.0f;
float mueveDragonOffset4 = 0.1f;

// ANIMACIÓN DEL DRAGON (CURVAS)
float rotaDragon2 = 0.0f;
float rotaDragonOffset2 = 0.45f; // ESTE VALOR ME ASEGURA QUE AL TERMINAR DE MOVERSE POR EL EJE Z LA ROTACION SEA DE 180°
float rotaDragon3 = 0.0f;
float rotaDragonOffset3 = 0.45f; // ESTE VALOR ME ASEGURA QUE AL TERMINAR DE MOVERSE POR EL EJE Z LA ROTACION SEA DE 180°

// FUNCION SENO PARA LA CURVA DEL DRAGON
float angulovaria2 = 0.0f;
float angulovaria3 = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

// PRACTICA 09-2
// MENSAJE: PROYECTO CGEIHC MONOPOLY
Texture tipografia;

Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;

// EJERCICIO CLASE
// EJERCICIO 03
Model DragonCuerpo;
Model DragonAla01;
Model DragonAla02;

// PRACTICA 09-2
// ARCO, LETRERO Y DRAGON
Model arco;
Model arcoLetrero;
Model DragonCabeza01;
Model DragonCabeza02;
Model DragonCabeza03;
Model DragonCabeza04;
Model DragonCabeza05;

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



//cálculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	// PRACTICA 09-2
	// MENSAJE : PROYECTO CGEIHC MONOPOLY
	unsigned int mensajeLetreroIndices[] = {
	   0, 1, 2,
	   1, 2, 3,
	};

	GLfloat mensajeLetreroVertices[] = {
		5.0f, 5.0f, 0.0f,		0.97f, 1.0f,	0.0f, 0.0f, -1.0f,
		5.0f, 4.0f, 0.0f,		0.97f, 0.8f,	0.0f, 0.0f, -1.0f,
		6.0f, 5.0f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		6.0f, 4.0f, 0.0f,		1.0f, 0.8f,		0.0f, 0.0f, -1.0f,
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

	// PRACTICA 09-2
	// MENSAJE : PROYECTO CGEIHC MONOPOLY
	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(mensajeLetreroVertices, mensajeLetreroIndices, 32, 6);
	meshList.push_back(obj8);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	// PRACTICA 09-2
	// TEXTURA DEL MENSAJE
	tipografia = Texture("Textures/tipografiaLetrero.png");
	tipografia.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/17174_Tiamat_new.obj");

	// EJERCICIO CLASE
	// EJERCICIO 03
	DragonCuerpo = Model();
	DragonCuerpo.LoadModel("Models/cuerpoDragon.obj");
	DragonAla01 = Model();
	DragonAla01.LoadModel("Models/ala01Dragon.obj");
	DragonAla02 = Model();
	DragonAla02.LoadModel("Models/ala02Dragon.obj");

	// PRACTICA 09-2
	// ARCO, LETRERO Y DRAGON
	arco = Model();
	arco.LoadModel("Models/arco.obj");
	arcoLetrero = Model();
	arcoLetrero.LoadModel("Models/letreroArco.obj");
	DragonCabeza01 = Model();
	DragonCabeza01.LoadModel("Models/cabeza01Dragon.obj");
	DragonCabeza02 = Model();
	DragonCabeza02.LoadModel("Models/cabeza02Dragon.obj");
	DragonCabeza03 = Model();
	DragonCabeza03.LoadModel("Models/cabeza03Dragon.obj");
	DragonCabeza04 = Model();
	DragonCabeza04.LoadModel("Models/cabeza04Dragon.obj");
	DragonCabeza05 = Model();
	DragonCabeza05.LoadModel("Models/cabeza05Dragon.obj");


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
		0.5f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
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
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glfwSetTime(0);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;

		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
			rotllanta += rotllantaOffset * deltaTime;
		}

		// PRACTICA 09-2
		// ALETEO ALA 01 y ALA 02
		// LLEGAN A 45° Y -45° Y REPITEN
		if (bajasube01)
		{
			if (rotaAla01 < 45.0f && rotaAla02 > -45.0f)
			{
				rotaAla01 += rotaAla01Offset * deltaTime;
				rotaAla02 -= rotaAla02Offset * deltaTime;
			}
			else
			{
				bajasube01 = !bajasube01;
			}
		}
		else
		{
			if (rotaAla01 > -45.0f && rotaAla02 < 45.0f)
			{
				rotaAla01 -= rotaAla01Offset * deltaTime;
				rotaAla02 += rotaAla02Offset * deltaTime;
			}
			else
			{
				bajasube01 = !bajasube01;
			}
		}

		// PRACTICA 09-2
		// IDA Y REGRESO DEL DRAGON
		if (mueveDragon > -100.0)
		{
			mueveDragon -= mueveDragonOffset * deltaTime;
		}
		else
		{
			if (mueveDragon2 < 40.0)
			{
				mueveDragon2 += mueveDragonOffset * deltaTime;
				// VALORES POSITIVOS PARA EL ANGULO
				angulovaria2 += 0.5f * deltaTime;
				rotaDragon2 += rotaDragonOffset2 * deltaTime;
				//printf("GRADOS AL FINAL : %f", rotaDragon2); // SON 180 GRADOS AL TERMINAR DE DAR LA CURVA, ASI EL DRAGON YA PODRA IR DE REGRESO
			}
			else
			{
				if (mueveDragon3 > -100.0)
				{
					mueveDragon3 -= mueveDragonOffset3 * deltaTime;
				}
				else
				{
					if (mueveDragon4 < 40.0)
					{
						mueveDragon4 += mueveDragonOffset4 * deltaTime;
						// VALORES POSITIVOS PARA EL ANGULO
						angulovaria3 += 0.5f * deltaTime;
						rotaDragon3 += rotaDragonOffset3 * deltaTime;
						//printf("GRADOS AL FINAL : %f", rotaDragon3); // SON 180 GRADOS AL TERMINAR DE DAR LA CURVA, ASI EL DRAGON YA PODRA IR DE REGRESO
					}
					else
					{
						// REINICAR LOS VALORES PARA VOLVER A DAR LA VUELTA
						mueveDragon = 0.0f;
						mueveDragon2 = 0.0f;
						mueveDragon3 = 0.0f;
						mueveDragon4 = 0.0f;

						angulovaria2 = 0.0f;
						angulovaria3 = 0.0f;

						rotaDragon2 = 0.0f;
						rotaDragon3 = 0.0f;
					}
				}
			}
		}
		
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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);

		// MATRIZ AUXILIAR PARA EL DRAGON
		glm::mat4 modelauxDragon(1.0);

		// MATRIZ AUXILIAR PARA EL ARCO
		glm::mat4 modelauxArco(1.0);

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// ARCO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 6.8f, -20.0));

		modelauxArco = model; // GUARDAR

		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arco.RenderModel();

		model = modelauxArco; // A PARTIR DEL ARCO

		// LETRERO
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.4));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arcoLetrero.RenderModel();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
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

		// DRAGON
		// EJERCICIO PRACTICA 9-2
		model = glm::mat4(1.0);

		// model = glm::translate(model, glm::vec3(0.0f, 5.0f+sin(glm::radians(angulovaria)), 6.0));
		// model = glm::translate(model, glm::vec3(0.0f - (angulovaria / 6), 10.0f + (5 * sin(glm::radians(angulovaria * 1))), 6.0));
		// model = glm::translate(model, glm::vec3(0.0f, 10.0f , 6.0));

		// EL PRIMER TRANSLATE (HASTA LLEGAR A UNA PARTE)
		model = glm::translate(model, glm::vec3(mueveDragon, 10.0f + (5 * sin(glm::radians(angulovaria * 1))), 6.0));

		// SEGUNDO TRASNLATE Y ROTATE, AL COMPLETAR EL PRIMERO (PARA HACER UNA CURVA)
		// EL VALOR NEGATIVO ES PORQUE LA FUNCION SENOIDAL COMIENZA HACIA LOS VALORES POSITIVOS (PARA X), PERO AL QUERER QUE EMPIECE HACIA VALORES NEGATIVOS (PARA -X) SE PONE
		// EL SIGNO NEGATIVO
		model = glm::translate(model, glm::vec3((-20 * sin(glm::radians(angulovaria2 * 0.83))), 0.0f, mueveDragon2));
		model = glm::rotate(model, rotaDragon2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// TERCER TRASNLATE, AL COMPLETAR EL SEGUNDO
		model = glm::translate(model, glm::vec3(mueveDragon3, 0.0f, 0.0f));

		// CUARTO TRASNLATE Y ROTATE, AL COMPLETAR EL TERCERO (PARA HACER UNA CURVA)
		// EL VALOR NEGATIVO ES PORQUE LA FUNCION SENOIDAL COMIENZA HACIA LOS VALORES POSITIVOS (PARA X), PERO AL QUERER QUE EMPIECE HACIA VALORES NEGATIVOS (PARA -X) SE PONE
		// EL SIGNO NEGATIVO
		model = glm::translate(model, glm::vec3((-20 * sin(glm::radians(angulovaria3 * 0.83))), 0.0f, mueveDragon4));
		model = glm::rotate(model, rotaDragon3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		modelauxDragon = model;

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		// model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DragonCuerpo.RenderModel();

		model = modelauxDragon; // A PARTIR DEL DRAGON

		model = glm::translate(model, glm::vec3(0.3f, 0.21f, -0.26));
		model = glm::rotate(model, rotaAla01 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DragonAla01.RenderModel();

		model = modelauxDragon; // A PARTIR DEL DRAGON

		model = glm::translate(model, glm::vec3(0.18f, 0.52f, 0.4));
		model = glm::rotate(model, rotaAla02 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DragonAla02.RenderModel();

		model = modelauxDragon; // A PARTIR DEL DRAGON

		// TRANSLATE ORIGINAL
		// model = glm::translate(model, glm::vec3(-1.0f, -0.5f, -0.7));
		// TRANSLATE DE LA LEMNISCATA, MANTINIEDNO LOS VALORES ORIGINALES DEL TRANSLATE Y AGREGANDO LAS FUNCIONES PARA LA LEMNISCATA
		// EL 0.5 ES EL TAMAÑO DE LA LEMNISCATA, EN ESTE CASO ES PEQUEÑA
		// Y AL MULTIPLICAR POR 0.1 ES PARA QUE HAGA LA ANIMACION LO MAS CERCA POSIBLE DE SU PUNTO ORIGINAL
		// AL DIVIDIR ENTRE 7 HAGO QUE SEA MAS LENTA LA ANIMACION
		model = glm::translate(model, glm::vec3(-1.0f, 
												-0.5f + (0.5 * sin(angulovaria/7) * cos(angulovaria/7)) / (0.5 + sin(angulovaria/7) * sin(angulovaria/7)) * 0.1, 
												-0.7f + (0.5 * cos(angulovaria / 7)) / (0.5 + sin(angulovaria / 7) * sin(angulovaria / 7)) * 0.1));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DragonCabeza01.RenderModel();

		model = modelauxDragon; // A PARTIR DEL DRAGON

		// TRANSLATE ORIGINAL
		// model = glm::translate(model, glm::vec3(-0.83f, 0.6f, -0.5));
		// TRANSLATE DE LA FUNCION SENOIDAL (LA MISMA QUE TIENE EL DRAGON), MANTINIEDNO LOS VALORES ORIGINALES DEL TRANSLATE Y AGREGANDO LAS FUNCION SENOIDAL
		// 0.1 * PARA LA AMPLITUD
		// * 5 PARA LONGITUD DE ONDA
		model = glm::translate(model, glm::vec3(-0.83f, 
												0.6f + (0.1 * sin(glm::radians(angulovaria * 5))), 
												-0.5));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DragonCabeza02.RenderModel();

		model = modelauxDragon; // A PARTIR DEL DRAGON

		// TRANSLATE ORIGINAL
		// model = glm::translate(model, glm::vec3(-1.5f, -0.14f, 0.0));
		// TRANSLATE DEL ESPIRAL DE ARQUIMEDES, MANTINIEDNO LOS VALORES ORIGINALES DEL TRANSLATE Y AGREGANDO EL ESPIRAL DE ARQUIMEDES
		// 0.001f y 0.01f: ESTOS VALORES CONTROLAN LA FORMA DEL ESPIRAL. 0.001 PARA DEFINIR EL RADIO INICIAL y 0.01 PARA CONTROLAR CUANTO CRECE EL RADIO CADA VUELTA.
		model = glm::translate(model, glm::vec3(-1.5f,
												-0.14f + +(0.001f + 0.01f * glm::radians(angulovaria)) * sin(glm::radians(angulovaria)),
												0.0f + (0.001f + 0.01f * glm::radians(angulovaria)) * cos(glm::radians(angulovaria))));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DragonCabeza03.RenderModel();

		model = modelauxDragon; // A PARTIR DEL DRAGON

		// TRANSLATE ORIGINAL
		// model = glm::translate(model, glm::vec3(-0.9f, 0.57f, 0.52));
		// TRANSLATE DEL MOVIMIENTO CIRCULAR, MANTINIEDNO LOS VALORES ORIGINALES DEL TRANSLATE Y AGREGANDO EL MOVIMIENTO CIRCULAR
		// 0.1 PARA EL RADIO Y * 10 PARA HACER MAS RAPIDO EL GIRO
		model = glm::translate(model, glm::vec3(-0.9f, 0.57f + (0.1 * sin(glm::radians(angulovaria * 10))), 0.52f + (0.1 * cos(glm::radians(angulovaria * 10)))));

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DragonCabeza04.RenderModel();

		model = modelauxDragon; // A PARTIR DEL DRAGON

		// TRANSALATE ORIGINAL
		// model = glm::translate(model, glm::vec3(-1.25f, -0.55f, 0.7));
		// TRANSLATE DEL MOVIMIENTO ROBOTE, MANTINIEDNO LOS VALORES ORIGINALES DEL TRANSLATE Y AGREGANDO EL MOVIMIENTO REBOTE
		// 0.1 PARA UN REBOTE BAJO
		model = glm::translate(model, glm::vec3(-1.25f, 
												-0.55f + (abs(0.1f * sin(glm::radians(angulovaria)))), 
												0.7));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(0.478f, 0.255f, 0.067f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DragonCabeza05.RenderModel();

		
		/*color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		
		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.0001; // son las coordenadas UV y funciona igual el offset (aumentar la velocidad)
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		// FLECHA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		// PARA RECORRER LA TEXTURA
		for (int i = 1; i<4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		 }

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}
 

		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		
		// IR RECORRIENDO LA TEXTURA
		/*toffsetnumerocambiau += 0.0001;
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;*/

		// ACTIVIDAD 02
		// EJERCICIO DE CLASE PRACTICA 09-2
		/////////////////////////////////////////////////

		if ((glfwGetTime() - tiempoInicial2) < 2)
		{
			toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov2); 
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}
		else
		{
			toffsetnumerocambiau += 0.25f; 

			if (toffsetnumerocambiau >= 1.0f) {
				toffsetnumerocambiau = 0.0f; 
				toffsetnumerov2 -= 0.33f;
				conta += 1;
				// printf("NUMEROOOOOOOOOOOOO : %i: ", conta);
			}
			
			else if (conta >= 2 && toffsetnumerocambiau >= 0.5f)
			{
				conta = 0;
				toffsetnumerov2 = 0.0f;
				toffsetnumerocambiau = 0.0f;
			}
			
			tiempoInicial2 = glfwGetTime();
		}

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		
		// ACTIVIDAD 01
		// EJERCICIO DE CLASE PRACTICA 09-2
		/////////////////////////////////////////////////
		if ((glfwGetTime() - tiempoInicial) < 2.0f)
		{
			Numero1Texture.UseTexture();
		}
		else
		{
			if ((glfwGetTime() - tiempoInicial) < 4.0f)
			{
				Numero2Texture.UseTexture();
			}
			else
			{
				tiempoInicial = glfwGetTime();
			}
		}
		/////////////////////////////////////////////////
		
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		// PRACTICA 09-2
		// IR RECORRIENDO LA TEXTURA DEL MENSAJE
		toffsetnumerouLetrero -= 0.0001;
		if (toffsetnumerouLetrero <= 0.0 && toffsetnumerovLetrero >= 1.0)
		{
			toffsetnumerouLetrero = 1.0;
			toffsetnumerovLetrero = 0.2;
		}
		else if (toffsetnumerouLetrero <= 0.0)
		{
			toffsetnumerouLetrero = 1.0;
			toffsetnumerovLetrero += 0.2;
		}

		toffset = glm::vec2(toffsetnumerouLetrero, toffsetnumerovLetrero);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.5f, 6.3f, -19.0));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		tipografia.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		glDisable(GL_BLEND);
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
