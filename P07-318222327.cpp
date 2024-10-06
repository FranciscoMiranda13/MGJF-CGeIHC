/*
Práctica 7: Iluminación 1 
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

// TEXTURAS DE LA PRACTICA 06
////////////////////////////////
Texture rojo;
Texture negro;
Texture cars;
////////////////////////////////

// EJERCICIO 03
// TEXTURAS DE LA PRACTICA 07
// LAMPARA
////////////////////////////////
Texture superior;
Texture inferior;
////////////////////////////////

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

// MODELOS DE LA PRACTICA 06
////////////////////////////////
Model CocheCars;
Model LlantaTraseraIzquierda;
Model LlantaTraseraDerecha;
Model LlantaDelanteraIzquierda;
Model LlantaDelanteraDerecha;
////////////////////////////////

// EJERCICIO 03
// MODELOS DE LA PRACTICA 07
// LAMPARA
////////////////////////////////
Model LamparaSimpSons;
////////////////////////////////


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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,// Para iluminar el agave
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
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


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
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

	// TEXTURAS DE LA PRACTICA 06
	////////////////////////////////////////////////////////////////////////
	cars = Texture("Textures/Imagen-318222327GIMP.png");
	cars.LoadTextureA();
	negro = Texture("Textures/ColorNegroGIMP.png");
	negro.LoadTextureA();
	rojo = Texture("Textures/ColorRojoGIMP.png");
	rojo.LoadTextureA();
	////////////////////////////////////////////////////////////////////////

	// EJERCICIO 03
	// TEXTURAS DE LA PRACTICA 07
	// LAMPARA
	//////////////////////////////////////////////////////
	superior = Texture("Textures/lamp_shade.bmp.png");
	inferior = Texture("Textures/char_swatches.bmp.png");
	//////////////////////////////////////////////////////

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	// MODELOS DE LA PRACTICA 06
	////////////////////////////////////////////////////////////////////
	CocheCars = Model();
	CocheCars.LoadModel("Models/cochePractica06.obj");
	LlantaTraseraIzquierda = Model();
	LlantaTraseraIzquierda.LoadModel("Models/llantaTraseraIzquierdaPractica06.obj");
	LlantaTraseraDerecha = Model();
	LlantaTraseraDerecha.LoadModel("Models/llantaTraseraDerechaPractica06.obj");
	LlantaDelanteraIzquierda = Model();
	LlantaDelanteraIzquierda.LoadModel("Models/llantaDelanteraIzquierdaPractica06.obj");
	LlantaDelanteraDerecha = Model();
	LlantaDelanteraDerecha.LoadModel("Models/llantaDelanteraDerechaPractica06.obj");
	////////////////////////////////////////////////////////////////////

	// EJERCICIO 03
	// MODELOS DE LA PRACTICA 07
	// LAMPARA
	/////////////////////////////////////////////////////////
	LamparaSimpSons = Model();
	LamparaSimpSons.LoadModel("Models/LamparaSimpsons.obj");
	/////////////////////////////////////////////////////////

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
		1.0f, 1.0f, 1.0f,// COLOR DE LA LUZ DEL SOL
		0.2f, 0.3f,// EL PRIMERO ES PARA LA INTENSIDAD DEL SOL, MAS ALTO ILUMINA MAS (MAXIMO 1)
		// EL SEGUNDO, ENTRE MAS ALTO EL COLOR ES MAS FUERTE? (EL COLOR DEL SOL?)
		// TIENE MÁS PESO EL PRIMERO
		// EL PRIMERO ES QUE TANTO ILUMINA (EL ESCENARIO?) Y EL SEGUNDO QUE TAN FUERTE ES EL COLOR (DE LO QUE ILUMINA?)
		0.0f, 0.0f, -1.0f);
		// ILUMINADO HACIA Z NEGATIVO
		
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(
		1.0f, 0.0f, 0.0f,//LA LUZ DE COLOR ROJO
		0.0f, 1.0f,// NO ABARCA TODO, PUES ESTA EN 0
		//Y ESTAMOS VIENDO EL COLOR ROJO MAS FUERTE POSIBLE (1.0)
		//NO PODEMOS USAR EL VALOR DE 1.0 EN LA PRIMERA PORQUE SE DA PREFERENCIA A ESTA Y NO A LA SEGUNDA?
		-25.0f, 1.5f, -2.5f, // CAMBIAMOS SU UBICACION
		0.3f, 0.2f, 0.1f);

	pointLightCount++;

	// EJERCICIO 03
	// PARA LA PRACTICA 07
	// LUZ
	////////////////////////////////
	pointLights[1] = PointLight(
		1.0f, 1.0f, 1.0f,
		0.8f, 1.0f,
		-30.0f, 6.0f, -20.0f,
		0.3f, 0.2f, 0.1f);

	pointLightCount++;
	////////////////////////////////

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

	spotLightCount++;// SE AUMENTA EL CONTADOR PARA NO SOBREESCRIBIR LA LUZ

	//luz fija
	spotLights[1] = SpotLight(
		0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,//AMBIENTAL Y DIFUSO
		-14.0f, 10.0f, -15.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);//SI PONGO 15 EN LUGAR DE 1 SE CIERRA AL CONO (O SEA ES MAS PEQUEÑA LA LUZ)
	// NO QUEREMOS QUE ILUMINE HASTA EL INFINITO?

	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	// PARA EL EJERCICIO DE CLASE 07
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//Faro frontal de color azul
	spotLights[2] = SpotLight(
		0.0f, 0.0f, 1.0f,
		4.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.015f, 0.0015f,
		10.0f);

	spotLightCount++;// SE AUMENTA EL CONTADOR PARA NO SOBREESCRIBIR LA LUZ
	///////////////////////////////////////////////////////////////////////////////////////////////////

	// EJERCICIO 02
	// PARA LA PRACTICA 07
	// SPOTLIGHT AMARILLA
	///////////////////////////////////////////////////////////////////////////////////////////////////
	spotLights[3] = SpotLight(
		1.0f, 1.0f, 0.0f,
		3.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);

	spotLightCount++;// SE AUMENTA EL CONTADOR PARA NO SOBREESCRIBIR LA LUZ
	///////////////////////////////////////////////////////////////////////////////////////////////////


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
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		// PARA LA JERARQUIA
		////////////////////////
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxH(1.0);
		////////////////////////

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// MODIFICADO A LO QUE SE REALIZO EN LA PRACTICA 06
		///////////////////////////////////////////////////////////////////////////////////////
		//Instancia del coche 
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 5.4f, -3.0f));
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
		
		glm::vec3 posicion = glm::vec3(model[3]); //+ glm::vec3(-12.0f, -1.0f, 0.0f); // AJUSTAR LA POSICION A PARTIR DEL COCHE
		glm::vec3 direccion = glm::vec3(model[2]);

		spotLights[2].SetFlash(posicion, direccion);

		model = modelaux; // A PARTIR DEL COCHE

		// LLANTA TRASERA IZQUIERDA

		model = glm::translate(model, glm::vec3(6.28f, -3.5f, -13.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		LlantaTraseraIzquierda.RenderModel();

		model = modelaux; // A PARTIR DEL COCHE

		// LLANTA TRASERA DERECHA

		model = glm::translate(model, glm::vec3(-6.5f, -3.5f, -13.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		LlantaTraseraDerecha.RenderModel();

		model = modelaux; // A PARTIR DEL COCHE

		// LLANTA DELANTERA IZQUIERDA

		model = glm::translate(model, glm::vec3(6.2f, -3.5f, 10.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		LlantaDelanteraIzquierda.RenderModel();

		model = modelaux; // A PARTIR DEL COCHE

		// LLANTA DELANTERA DERECHA

		model = glm::translate(model, glm::vec3(-6.5f, -3.5f, 10.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*color = glm::vec3(0.42f, 0.0f, 0.18f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		LlantaDelanteraDerecha.RenderModel();
		///////////////////////////////////////////////////////////////////////////////////////

		// EJERCICIO 01
		// PARA LA PRACTICA 07
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

		// EJERCICIO 02
		// PARA LA PRACTICA 07
		// SPOTLIGHT AMARILLA

		// PARA LA LUZ AZUL CON JERARQUIA

		glm::vec3 posicionH = glm::vec3(model[3]);  
		glm::vec3 direccionH = glm::vec3(model[2]);

		direccionH = -direccionH; // PORQUE APUNTA A Z INICIALMENTE EN EL MODELO ORIGINAL Y QUIERO QUE APUNTE A - Z EN EL MODELO ORIGINAL
		// AL HACER LAS ROTACIONES ES COMO SI APUNTARA HACIA -Y

		spotLights[3].SetFlash(posicionH, direccionH);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// EJERCICIO 03
		// PARA LA PRACTICA 07
		// LAMPARA
		//////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-30.0f, 2.55f, -20.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LamparaSimpSons.RenderModel();
		//////////////////////////////////////////////////////////////////////////////

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
		//IMPORTANTE EL ORDEN QUE SE DIBUJAN PARA QUE LA TRANSPARENCIA NO MUERA

		//OCUPAR LA LUZ DE LA LINTERANA PARA EL CARRO?

		//SI MUEVES EL HELICOPTERO SE MUEVE LA LUZ DE ABAJO

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
