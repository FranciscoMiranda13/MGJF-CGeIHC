
/*
PRIMERO SOBRE LOS VALORES CON,EXP y LIN
 COMO USARLOS

 PODEMOS PONER COMO MAXIMO 6 POINTLIGHT (1 ES PARA SPOT Y OTRA PARA EL SOL LA CUAL SIEMPRE DEBE ESTAR)
 PODEMOS JUGAR CON ESOS VALORES (SPOT Y POINT)
 PERO SOLO CON 8


 PARA LOS DOS ARREGLOS:

 POINTLIGHT POINTLIGHTS [MAX_POINT_LIGHT]
 POINTLIGHT SEGUNDOPOINT [MAX_POINT_LIGHT]
 ...
 EN LA FOTO:
 YA NO ES NECESARIO AUMENTAR EL CONTADOR CON EL SEGUNDO ARREGLO, PUES YA ESTA ESTABLECIDO
 IGUAL PODEMOS COPIAR Y PEGAR LA LUZ COMPLETA, PERO LO DE LA FOTO ES MAS OPTIMO
 */

/*
Práctica 8: Iluminación 2
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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

// TEXTURAS DE EJERCICIOS Y PRACTICAS PASADAS	
/////////////////////
Texture dado10Caras;
Texture rojo;
Texture negro;
Texture cars;
Texture superior;
Texture inferior;
/////////////////////

// PARA PRACTICA 8
//////////////////
Texture Diamante;
//////////////////
	
Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

// MODELOS DE EJERCICIOS Y PRACTICAS PASADAS
////////////////////////////////
Model CocheCars;
Model Cofre;
Model LlantaTraseraIzquierda;
Model LlantaTraseraDerecha;
Model LlantaDelanteraIzquierda;
Model LlantaDelanteraDerecha;
Model LamparaSimpSons;
////////////////////////////////

// PARA PRACTICA 8
//////////////////
Model DiamanteM; 
//////////////////

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

// PARA PRACTICA 8
///////////////////////////////////////////////////
SpotLight spotlightFaro[MAX_SPOT_LIGHTS];
///////////////////////////////////////////////////

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

	// Plano X Z
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,// A partir de que direccion va a llegar la luz (Por eso apunta hacia -Y)
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
		// Para iluminar el agave poner en -z para que lo vea iluminado igual podemos poner en +x en el mismo 
		// igual podemos poner -y
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, 
		// igual es el agave, pero el otro plano (la otra parte?)
		// aqui hacia -x, para que tambien se ilumine del otro lado
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
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

	// CARGAR TEXTURAS DE EJERCICIOS Y PRACTICAS PASADAS
	//////////////////////////////////////////////////////
	dado10Caras = Texture("Textures/dado10CarasGIMP.png");
	dado10Caras.LoadTextureA();
	cars = Texture("Textures/Imagen-318222327GIMP.png");
	cars.LoadTextureA();
	negro = Texture("Textures/ColorNegroGIMP.png");
	negro.LoadTextureA();
	rojo = Texture("Textures/ColorRojoGIMP.png");
	rojo.LoadTextureA();
	superior = Texture("Textures/lamp_shade.bmp.png");
	inferior = Texture("Textures/char_swatches.bmp.png");
	//////////////////////////////////////////////////////

	// PARA PRACTICA 8
	///////////////////////////////////////////////////
	Diamante = Texture("Textures/Tex_0001_0.png");
	Diamante.LoadTextureA();
	///////////////////////////////////////////////////

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	// CARGAR MODELOS DE EJERCICIOS Y PRACTICAS PASADAS
	//////////////////////////////////////////////////////////////////////////////////////
	CocheCars = Model();
	CocheCars.LoadModel("Models/cochePractica06.obj");
	Cofre = Model();
	Cofre.LoadModel("Models/cofrePractica06.obj");
	LlantaTraseraIzquierda = Model();
	LlantaTraseraIzquierda.LoadModel("Models/llantaTraseraIzquierdaPractica06.obj");
	LlantaTraseraDerecha = Model();
	LlantaTraseraDerecha.LoadModel("Models/llantaTraseraDerechaPractica06.obj");
	LlantaDelanteraIzquierda = Model();
	LlantaDelanteraIzquierda.LoadModel("Models/llantaDelanteraIzquierdaPractica06.obj");
	LlantaDelanteraDerecha = Model();
	LlantaDelanteraDerecha.LoadModel("Models/llantaDelanteraDerechaPractica06.obj");
	LamparaSimpSons = Model();
	LamparaSimpSons.LoadModel("Models/LamparaSimpsons.obj");
	//////////////////////////////////////////////////////////////////////////////////////

	// PARA PRACTICA 8
	///////////////////////////////////////////////////
	DiamanteM = Model(); 
	DiamanteM.LoadModel("Models/Diamante.obj");
	///////////////////////////////////////////////////

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
	mainLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,
		0.2f, 0.3f,
		0.0f, 0.0f, -1.0f);

	// ACTIVIDADES PRACTICA 8
	/////////////////////////////////////////////////////////////////////
		
	// POINTLIGHTS

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	// POINTLIGHT DIAMANTE <--------------------------------------------
	pointLights[0] = PointLight(
		0.0f, 0.0f, 1.0f,
		0.9f, 0.5f,
		-60.0f, 3.5f, -30.0f,
		0.0f, 0.1f, 0.06f);

	pointLightCount++;

	// POINTLIGHT LAMPARA <---------------------------------------------
	pointLights[1] = PointLight(
		1.0f, 1.0f, 1.0f,
		0.8f, 1.0f,
		-20.0f, 6.0f, -30.0f,
		0.0f, 0.05f, 0.09f);

	pointLightCount++;

	// SPOTLIGHTS

	// Contador de luces SpotLight
	unsigned int spotLightCount = 0;

	//linterna
	spotLights[0] = SpotLight(
		1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);

	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(
		0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		-14.0f, 10.0f, -15.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);

	spotLightCount++;

	// COFRE <-------------------------------------------------------
	spotLights[2] = SpotLight(
		1.0f, 0.0f, 1.0f,
		3.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.01f, 0.001f,
		15.0f);

	spotLightCount++;

	// HELICOPTERO
	spotLights[3] = SpotLight(
		1.0f, 1.0f, 0.0f,
		3.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);

	spotLightCount++;

	// PARA LOS FAROS DELNATEROS Y TRASEROS

	unsigned int spotLightCountFaros = 0;

	// ADELANTE <---------------------------------------------------------
	spotlightFaro[0] = SpotLight(
		1.0f, 0.5f, 0.0f,
		3.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.01f, 0.001f,
		15.0f);

	spotLightCountFaros++;

	// ATRAS <------------------------------------------------------------
	spotlightFaro[1] = SpotLight(
		0.5f, 1.0f, 0.5f,
		3.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.01f, 0.001f,
		15.0f);

	spotLightCountFaros++;
	
	/////////////////////////////////////////////////////////////////////

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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

		/*
			shaderList[0].SetPointLights(pointLights, pointLightCount);// AQUI SE LE PASAN LAS LUCES
		
			 LAS CONTROLAMOS AQUI

			 PARA INTERCAMBIAR ENTRE ARREGLOS
		
			WHILE ()
				IF VAR = ALGUNA TECLA
					ARREGLO 1?
				ELSE
					ARREGLO 2?
		

			 PARA APAGAR
		
			shaderList[0].SetPointLights(pointLights, pointLightCount -1); ESE -1 APAGA LAS ULTIMAS????
			DEPENDE DE QUE TANTO LE RESTES
			PERO APAGA DOS AL MISMO TIEMPO?
		*/

		// F (PRENDE DIAMANTE) . G (APAGA DIAMANTE)
		// B (PRENDE LAMPARA) . N (APAGA LAMPARA)
		// Q (ORIGINAL)

		if (mainWindow.getinicio2() == false) // TODAS LAS LUCES APAGADAS 
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 2);
		}
		else if (mainWindow.getapagaPrende() == true && 
				mainWindow.getapagarPrenderLampara() == false && 
				mainWindow.getinicio2() != false) // SI QUIERO LA LUZ DEL DIAMANTE
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}
		else if (mainWindow.getapagaPrende() == false && 
				mainWindow.getapagarPrenderLampara() == true && 
				mainWindow.getinicio2() != false) // SI QUIERO LA LUZ DE LA LAMPARA
		{
			shaderList[0].SetPointLights(pointLights + 1, pointLightCount - 1);
		}
		else if (mainWindow.getapagaPrende() == true && 
				mainWindow.getapagarPrenderLampara() == true && 
				mainWindow.getinicio2() != false) // SI QUIERO AMBAS
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (mainWindow.getapagaPrende() == false && 
				mainWindow.getapagarPrenderLampara() == false 
				&& mainWindow.getinicio2() != false) // SI QUIERO APAGAGAR AMBAS
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 2);
		}
		else if (mainWindow.getapagaPrende() == false && 
				mainWindow.getapagarPrenderLampara() == true &&		
				mainWindow.getinicio2() != false) // SI QUIERO APAGAR LA LUZ DEL DIAMANTE
		{
			shaderList[0].SetPointLights(pointLights + 1, pointLightCount - 1);
		}
		else if (mainWindow.getapagaPrende() == true && 
				mainWindow.getapagarPrenderLampara() == false && 
				mainWindow.getinicio2() != false) // SI QUIERO APAGAR LA LUZ DE LA LAMPARA
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}

		// U (ADELANTE) . Y (ATRAS)
		// L (ORIGINAL)

		if (mainWindow.gethaciaAdelanteAtras() == true && mainWindow.getinicio() != false)
		{
			shaderList[0].SetSpotLights(spotlightFaro, spotLightCountFaros - 1); // ADELANTE
		}
		if (mainWindow.gethaciaAdelanteAtras() == false && mainWindow.getinicio() != false)
		{
			shaderList[0].SetSpotLights(spotlightFaro + 1, spotLightCountFaros - 1); // ATRAS
		}
		if (mainWindow.getinicio() == false) 
		{
			shaderList[0].SetSpotLights(spotLights, spotLightCount); // ORIGINAL
		}

		glm::mat4 model(1.0);

		// PARA LA JERARQUIA
		/////////////////////////////////////////////
		glm::mat4 modelaux(1.0); // COCHE
		glm::mat4 modelauxH(1.0); // HELICOPTERO
		glm::mat4 modelauxC(1.0); // COFRE
		/////////////////////////////////////////////

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// DADO 10 CARAS
		/////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 20.0f, 15.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10Caras.UseTexture(); // CAMBIAMOS POR NUESTRA TEXTURA
		meshList[4]->RenderMesh();
		/////////////////////////////////////////////////////////////////////////////////////

		// COCHE
		///////////////////////////////////////////////////////////////////////////////////////
		//Instancia del coche 
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 4.2f, -3.0f));
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.0f,0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(mainWindow.getrotaeny()), glm::vec3(0.0f, 1.0f, 0.0f)); // SOLO COMO EJEMPLO (ROTAR CON P)
		//EN VEZ DE PONERLE ROTACION AL COCHE Y CADA LLANTA POR SEPARADO, SE HEREDA DESDE EL COCHE 

		modelaux = model; // GUARDAR

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		CocheCars.RenderModel();

		model = modelaux; // A PARTIR DEL COCHE

		// PARA LA LUZ AZUL CON JERARQUIA

		if (mainWindow.gethaciaAdelanteAtras() == true)  
		{
			glm::vec3 posicion = glm::vec3(model[3]) + glm::vec3(-12.0f, -1.0f, 0.0f); // AJUSTAR LA POSICION A PARTIR DEL COCHE 
			// ESTO HACE QUE LA LUZ SE ACOMODE ENFRENTE DEL COCHE, PERO SI INTENTAMOS ROTAR EL COCHE LA LUZ ESTARA SEPAEADA DEL PUNTO DE ORIGEN, POR LO CUAL SE VERA DESFASADA
			// COMO LA ROTACION DEL COCHE NO SE SOLICITA EN LA PRACTICA REALEMNTE NO IMPORTA LO ANTERIOR
			glm::vec3 direccion = glm::vec3(model[2]);

			spotlightFaro[0].SetFlash(posicion, direccion);
		}

		if (mainWindow.gethaciaAdelanteAtras() == false)
		{
			glm::vec3 posicion = glm::vec3(model[3]) + glm::vec3(10.0f, -1.0f, 0.0f); 
			glm::vec3 direccion = glm::vec3(model[2]);

			direccion = - direccion;

			spotlightFaro[1].SetFlash(posicion, direccion);
		}

		model = modelaux; // A PARTIR DEL COCHE

		// COFRE

		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 12.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionCofre()), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotar con: M y C

		modelauxC = model; // GUARDAR

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		Cofre.RenderModel(); 

		model = modelauxC; // A PARTIR DEL COFRE

		// PARA LA LUZ AZUL CON JERARQUIA

		glm::vec3 posicionC = glm::vec3(model[3]);
		glm::vec3 direccionC = glm::vec3(model[2]);

		spotLights[2].SetFlash(posicionC, direccionC);

		model = modelaux; // A PARTIR DEL COCHE

		// LLANTA TRASERA IZQUIERDA

		model = glm::translate(model, glm::vec3(6.6f, -2.25f, -9.08f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		LlantaTraseraIzquierda.RenderModel();

		model = modelaux; // A PARTIR DEL COCHE

		// LLANTA TRASERA DERECHA

		model = glm::translate(model, glm::vec3(-6.9f, -2.4f, -9.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		LlantaTraseraDerecha.RenderModel();

		model = modelaux; // A PARTIR DEL COCHE

		// LLANTA DELANTERA IZQUIERDA

		model = glm::translate(model, glm::vec3(6.6f, -2.35f, 14.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		LlantaDelanteraIzquierda.RenderModel();

		model = modelaux; // A PARTIR DEL COCHE

		// LLANTA DELANTERA DERECHA

		model = glm::translate(model, glm::vec3(-6.9f, -2.35f, 14.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(0.42f, 0.0f, 0.18f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		LlantaDelanteraDerecha.RenderModel();
		///////////////////////////////////////////////////////////////////////////////////////

		// HELICOPTERO
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-20.0f, 20.0f, 20.0));
		model = glm::translate(model, glm::vec3(mainWindow.getmueveHelicoptero(), 0.0f, 0.0f)); // MOVER EL HELICOPTERO EN EL EJE X (CON I y O)
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaenz()), glm::vec3(1.0f, 0.0f, 0.0f)); // SOLO COMO EJEMPLO (ROTAR CON Z)

		modelauxH = model;

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		model = modelauxH; // A PARTIR DEL HELICOPTERO

		// PARA LA LUZ AZUL CON JERARQUIA

		glm::vec3 posicionH = glm::vec3(model[3]);  
		glm::vec3 direccionH = glm::vec3(model[2]);

		direccionH = -direccionH; // PORQUE APUNTA A Z INICIALMENTE EN EL MODELO ORIGINAL Y QUIERO QUE APUNTE A - Z EN EL MODELO ORIGINAL
		// AL HACER LAS ROTACIONES ES COMO SI APUNTARA HACIA -Y

		spotLights[3].SetFlash(posicionH, direccionH);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// LAMPARA
		//////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-20.0f, 2.55f, -30.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LamparaSimpSons.RenderModel();
		//////////////////////////////////////////////////////////////////////////////

		// PARA PRACTICA 8
		///////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-60.0f, 3.5f, -30.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DiamanteM.RenderModel();
		///////////////////////////////////////////////////////////////////////////

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 3.0f, -20.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//ESTAS DOS PARA LA TRANSLUCIDES
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);// PARA DESACTIVAR LA TRANSLUCIDES
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
