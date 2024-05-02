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

Texture pisoTexture, brickTexture;

// Edificios
Model KameHouse, Capsule, CasaBob, CasaCalamardo, Flores, Piedra, CasaSaitama;

// Personajes
Model Roshi, Bob, Calamardo, Gary, Karin, Overgrown;

//Vehículos
Model Cangremovil, Bicicleta, Nube;

// Puertas
Model LionGate;

Skybox skybox, skyboxNigth;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight, mainLightNigth;
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

//meshList[4]
void CreateCubeMesh()
{

	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	//TODO: La textura de ladrillo aparece rotada en el subo
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,			0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		3.0f, 0.0f,			0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.0f, 2.0f,			0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	3.0f, 2.0f,			0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f, 0.0f,			-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	3.0f, 0.0f,			-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.0f, 2.0f,			-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    3.0f, 2.0f,			-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		3.0f, 0.0f,			0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.0f, 2.0f,			0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	3.0f, 2.0f,			0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f, 0.0f,			1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	3.0f, 0.0f,			1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 2.0f,			1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	3.0f, 2.0f,			1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,				0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	3.0f, 0.0f,			0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.0f, 2.0f,			0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	3.0f, 2.0f,			0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f, 0.0f,				0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	3.0f, 0.0f,			0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.0f, 2.0f,			0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	3.0f, 2.0f,			0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void LoadModels() {
	
	// Edificios
	/*
	KameHouse = Model();
	KameHouse.LoadModel("Models/DragonBall/KameHouse/kameHouse.obj");
	Capsule = Model();
	Capsule.LoadModel("Models/DragonBall/CapsuleCorp/CapsuleCorp.obj");*/
	CasaBob = Model();
	CasaBob.LoadModel("Models/BobEsponja/CasaBob/CasaBob.obj");
	//Flores = Model();
	//Flores.LoadModel("Models/BobEsponja/CasaBob/flores.obj");
	//Piedra = Model();
	//Piedra.LoadModel("Models/BobEsponja/CasaBob/piedras.obj");
	CasaCalamardo = Model();
	CasaCalamardo.LoadModel("Models/BobEsponja/CasaCalamardo/CasaCalamardo.obj");
	CasaSaitama = Model();
	CasaSaitama.LoadModel("Models/OnePunchMan/CasaSaitama/CasaSaitama.obj");
	
	// Personajes
	Roshi = Model();
	Roshi.LoadModel("Models/DragonBall/MaestroRoshi/MaestroRoshi.obj");
	Karin = Model();
	Karin.LoadModel("Models/DragonBall/Karin/Karin.obj");
	Bob = Model();
	Bob.LoadModel("Models/BobEsponja/Bob/BobEsponja.obj");
	Calamardo = Model();
	Calamardo.LoadModel("Models/BobEsponja/Calamardo/Calamardo.obj");
	Gary = Model();
	Gary.LoadModel("Models/BobEsponja/Gary/Gary.obj");
	Overgrown = Model();
	Overgrown.LoadModel("Models/OnePunchMan/Overgrown/Overgrown.obj");

	// Vehículos
	Cangremovil = Model();
	Cangremovil.LoadModel("Models/BobEsponja/Cangremovil/Cangremovil.obj");
	Bicicleta = Model();
	Bicicleta.LoadModel("Models/OnePunchMan/Bicicleta/Bicicleta.obj");
	Nube = Model();
	Nube.LoadModel("Models/DragonBall/Nube/Nube.obj");

	// Puertas
	LionGate = Model();
	LionGate.LoadModel("Models/gate.obj");
}

void RenderEdificios(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel) {

	glm::mat4 casa(1.0);
	// *********************************************************************
			// Capsule Corp
	// *********************************************************************
	/*
	model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -150.0f));
	// Por ahora se usa esto, lo mejor seria escalar el modelo directo en 3DSMax
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Capsule.RenderModel();
	*/
	// *********************************************************************
			// Capsule Bob Esponja
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//casa = model; no sé que es esta linea
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	CasaBob.RenderModel();
	/*
	model = casa;
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Flores.RenderModel();

	model = casa;
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra.RenderModel();*/

	// *********************************************************************
			// Casa Calamardo
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-250.0f, 0.0f, -100.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	CasaCalamardo.RenderModel();

	// *********************************************************************
			// Kame House
	// *********************************************************************
	/*
	model = modelaux;
	model = glm::translate(model, glm::vec3(300.0f, 0.0f, -300.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KameHouse.RenderModel();
	*/
	// *********************************************************************
			// Departamento Saitama
	// *********************************************************************
	
	model = modelaux;
	model = glm::translate(model, glm::vec3(40.0f, 0.0f, 230.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	CasaSaitama.RenderModel();
}

void RenderPersonajes(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel) {

	// *********************************************************************
			// Maestro Roshi 
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-190.0f, 0.0f, -180.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Roshi.RenderModel();

	// *********************************************************************
			// Bob Esponja
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-230.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bob.RenderModel();

	// *********************************************************************
			// Calamardo
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -90.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Calamardo.RenderModel();

	// *********************************************************************
			// Gary
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-50.0f, 1.0f, -190.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Gary.RenderModel();

	// *********************************************************************
			// Karin
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-60.0f, -0.5f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Karin.RenderModel();

	// *********************************************************************
			// Overgrown
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-50.0f, 0.3f, -170.0f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Overgrown.RenderModel();
}

void RenderVehiculos(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel) {

	// *********************************************************************
		// Bicicleta 
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(170.0f, 0.3f, -200.0f));
	model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bicicleta.RenderModel();

	// *********************************************************************
		// Cangremovil
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(180.0f, 1.3f, -170.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Cangremovil.RenderModel();

	// *********************************************************************
		// Nube
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(170.0f, 20.0f, -140.0f));
	model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Nube.RenderModel();
}
/*
	Este metodo va a crear varias instancias del cubo para formar las paredes. Esto
	evitara tener que mover la textura si queremos hacer paredes de diferentes tamanos
*/
void buildWall(int size, glm::vec3 dir, glm::mat4* model, GLuint uniformModel, GLuint uniformColor, Texture* texture,
	GLuint uniformSpecularIntensity, GLuint uniformShininess, glm::vec3 color) {
	for (int i = 0; i < size; i++) {
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(*model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		(*texture).UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		*model = glm::translate(*model, dir);
	}
}

void RenderOutsideWalls(glm::mat4 model, GLuint uniformModel, GLuint uniformColor, Texture* brickTexture,
	GLuint uniformSpecularIntensity, GLuint uniformShininess) {

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 wallModelAux(1.0);
	model = glm::translate(model, glm::vec3(-10.0f, 3.5f, 275.0f));
	wallModelAux = model;

	model = glm::scale(model, glm::vec3(20.0f, 7.0f, 2.0f));
	model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
	buildWall(13, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);
	
	model = wallModelAux;
	model = glm::scale(model, glm::vec3(20.0f, 7.0f, 2.0f));
	model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	buildWall(14, glm::vec3(-1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	model = wallModelAux;
	model = glm::scale(model, glm::vec3(2.0f, 7.0f, 20.0f));
	model = glm::translate(model, glm::vec3(140.0f, 0.0f, -0.5f));
	buildWall(28, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	model = wallModelAux;
	model = glm::scale(model, glm::vec3(2.0f, 7.0f, 20.0f));
	model = glm::translate(model, glm::vec3(-140.0f, 0.0f, -0.5f));
	buildWall(28, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	model = wallModelAux;
	model = glm::scale(model, glm::vec3(20.0f, 7.0f, 2.0f));
	model = glm::translate(model, glm::vec3(-13.5f, 0.0f, -280.0f));
	buildWall(28, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	// Puerta Principal
	model = wallModelAux;
	model = glm::scale(model, glm::vec3(0.35f, 0.4f, 0.5f));
	model = glm::translate(model, glm::vec3(30.0f, -9.0f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LionGate.RenderModel();
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CreateCubeMesh();

	camera = Camera(glm::vec3(0.0f, 0.0f, 290.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.0f, 0.5f);
	
	pisoTexture = Texture("Textures/Skybox/sqpantsdn.tga");
	pisoTexture.LoadTextureA();


	brickTexture = Texture("Textures/brick2.png");
	brickTexture.LoadTextureA();

	LoadModels();

	std::vector<std::string> skyboxFaces;

	skyboxFaces.push_back("Textures/Skybox/sqpantsrt.tga");
	skyboxFaces.push_back("Textures/Skybox/sqpantslf.tga");
	skyboxFaces.push_back("Textures/Skybox/sqpantsdn.tga");
	skyboxFaces.push_back("Textures/Skybox/sqpantsup.tga");
	skyboxFaces.push_back("Textures/Skybox/sqpantsbk.tga");
	skyboxFaces.push_back("Textures/Skybox/sqpantsft.tga");

	skybox = Skybox(skyboxFaces);

	// CARGA EL SKYBOX DE NOCHE

	std::vector<std::string> skyboxFacesN;

	skyboxFacesN.push_back("Textures/Skybox/sqpantsrt_N.tga");
	skyboxFacesN.push_back("Textures/Skybox/sqpantslf_N.tga");
	skyboxFacesN.push_back("Textures/Skybox/sqpantsdn_N.tga");
	skyboxFacesN.push_back("Textures/Skybox/sqpantsup_N.tga");
	skyboxFacesN.push_back("Textures/Skybox/sqpantsbk_N.tga");
	skyboxFacesN.push_back("Textures/Skybox/sqpantsft_N.tga");

	skyboxNigth = Skybox(skyboxFacesN);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.6f, 0.6f,
		0.0f, 0.0f, -1.0f);

	mainLightNigth = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.2f, 0.1f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(0.0f, 0.0f, 0.0f,
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
	////Loop mientras no se cierra la ventana

	int n = 1; // Para controlar el tiempo de día y noche

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

		if(now >= 24 * n && now < 24 * (n + 1)) { // DIA
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else if (now >= 24 * (n + 1) && now < 24 * (n + 2)) { // NOCHE
			skyboxNigth.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else {
			n = n + 2;
		}

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
		if (now >= 24 * n && now < 24 * (n + 1)) {
			shaderList[0].SetDirectionalLight(&mainLight);
		}
		else if (now >= 24 * (n + 1) && now < 24 * (n + 2)) {
			shaderList[0].SetDirectionalLight(&mainLightNigth);
		}
		else {
			n = n + 2;
		}
		
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = modelaux;

		//*****************************************************************
				// CARGA LOS EDIFICIOS
		//*****************************************************************

		RenderEdificios(model, modelaux, uniformModel);

		//*****************************************************************
				// CARGA LOS PERSONAJES
		//*****************************************************************

		RenderPersonajes(model, modelaux, uniformModel);

		//*****************************************************************
				// CARGA LOS VEHÍCULOS
		//*****************************************************************

		RenderVehiculos(model, modelaux, uniformModel);

		//*****************************************************************
				// CARGA PAREDES EXTERNAS DE ESCENA
		//*****************************************************************

		RenderOutsideWalls(model, uniformModel, uniformColor, &brickTexture, uniformSpecularIntensity, uniformShininess);
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

//blending: transparencia o traslucidez
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//AgaveTexture.UseTexture();
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		//glDisable(GL_BLEND);