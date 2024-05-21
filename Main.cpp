//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <iostream> // Para números aleatorios
#include <cstdlib>

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

// Para el audio 
#include <irrklang.h>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")

// Audio
ISoundEngine* Pos1;
ISoundEngine* Pos2;

ISound* Goku;
ISound* Bang;


const float toRadians = 3.14159265f / 180.0f;

int n = 1; // Para controlar el tiempo de día y noche
int day = 30;
int burgir = 3;
float angle = 0.0f; // Para generar el glich
float ang = 0.0f; // Para el cangremovil
float angArm = 0.0f; // Para el saludo de Calamardo
float angArmB = 0.0f; // Para que bob lance las cangreburgers 
float movburgir = 35.8f; // Para la animación de la cangreburguer
float angburgir = 0.0f;
float movOffsetburgir = 1.2f;
float angArmBob = 0.0f;
float angleHead = 0.0f; // Para la cabeza de Overgrownd
float movbiciz = -260.0f, movbicix = 255.0f; // Para mover la bici
float movbiciOffset = 0.7f, rotllantaB = 0.0f; 
float movcangre = -250.0f, rotllantaC = 0.0f; // Para mover al cangremovil
float movOffSetC = 0.5f, rotllantaOffSetC = 8.0f, movOffsetC2 = 0.8f;
float movM1 = 20.0f,movM2 = 10.f, scaleM = 1.0f;
float movOffSetM = 0.08f, scalOffSetC = 0.009f;
float movnubeX = 0.0f, movnubeY = 0.0f, movnubeZ = 0.0f; // Para mover a la nube voladora
float angleNube = 0.0f;
float movnubeoffset, movnubeYoffset; 
int nubeState = 0;
bool saludo = true, mover = true, girar = true, aire = false, moverbrazo = true;
bool avanza = true, dig = true, ret = true; // Para el recorrido de la bici
bool up1 = true, up2 = true, crece = true;

// Extremidades Saitama
float WALK_ANIM_SPEED = 5.0;
float leftrot = 0.0f;
float rightrot = 0.0f;
float leftinc = WALK_ANIM_SPEED;
float rightinc = -WALK_ANIM_SPEED;

//Animacion letrero
float toffsetbanneru = 0.0f;

bool cameraState = true;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

std::vector<Texture*> wallTextures;

//Saitama pelon;

Camera camera;
Camera camaraAvatar = Camera(glm::vec3(-120.0f, 30.0f, 330.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 2.5f, 0.5f);;
Camera camaraAerea = Camera(glm::vec3(0.0f, 150.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0, 2.5f, 0.5f);;

Model SaitamaBody, SaitamaLeftL, SaitamaRightL, SaitamaLeftA, SaitamaRightA;

Texture pisoTexture, brickTexture, pisoNigth, bushTexture, rockWallTexture, pathTexture;
Texture GateBannerTexture;

// Edificios
Model KameHouse, CapsuleCorp, CasaBob, CasaCalamardo, Flores, Piedra, CasaSaitama;

// Personajes
Model Roshi, Bob, Calamardo, Gary, Karin;
Model BrazoCalamardo, BodyOver, HeadOver, BrazoBob;

// Vehículos
Model Cangremovil, Bicicleta, Nube;
Model llantaC, llantaB;

// Decoración 
Model Piedra1, Piedra2, Piedra3, Piedra4, Piedra5, Patito, LamparaZoo, Kunai, Leon;
Model Pato, Shuriken, Bamboo, LamparaZoo_On, LampGar, Pasto1, Saitama, Tutsumaki;
Model Planta1, Planta2, Planta3, Planta4, Planta5, Planta6, Pasto, Medusa;
Model SpotlightModel, PlankStage;
Model mesa, cangre, Paso, PasoPeatonal, Mapa, Peligro;

// Puertas
Model LionGate;
Model Porton;
Model PuertaMetal;

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

//Declaración de luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];
PointLight pointLights3[MAX_POINT_LIGHTS];
PointLight pointLights4[MAX_POINT_LIGHTS];

//Declaración de luces de tipo spotlight
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
		10.0f, 0.0f, -10.0f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f
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

	unsigned int letreroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat letreroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(letreroVertices, letreroIndices, 32, 6);
	meshList.push_back(obj5);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

//meshList[5]
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
		0.5f,  0.5f,  0.5f,		3.0f, 2.0f,			0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.0f, 2.0f,			0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f, 0.0f,			-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	3.0f, 0.0f,			-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	3.0f, 2.0f,			-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f, 2.0f,			-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		3.0f, 0.0f,			0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		3.0f, 2.0f,			0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 2.0f,			0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f, 0.0f,			1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	3.0f, 0.0f,			1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	3.0f, 2.0f,			1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f, 2.0f,			1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,				0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	3.0f, 0.0f,			0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	3.0f, 2.0f,			0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f, 2.0f,			0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f, 0.0f,				0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	3.0f, 0.0f,			0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	3.0f, 2.0f,			0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f, 2.0f,			0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void LoadModels() {

	// Edificios
	KameHouse = Model();
	KameHouse.LoadModel("Models/DragonBall/KameHouse/kameHouse.obj");
	CapsuleCorp = Model();
	CapsuleCorp.LoadModel("Models/DragonBall/CapsuleCorp/CapsuleCorp.obj");
	CasaBob = Model();
	CasaBob.LoadModel("Models/BobEsponja/CasaBob/CasaBob.obj");
	Flores = Model();
	Flores.LoadModel("Models/BobEsponja/CasaBob/flores.obj");
	Piedra = Model();
	Piedra.LoadModel("Models/BobEsponja/CasaBob/piedras.obj");
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
	BrazoBob = Model();
	BrazoBob.LoadModel("Models/BobEsponja/Bob/BrazoBob.obj");
	Calamardo = Model();
	Calamardo.LoadModel("Models/BobEsponja/Calamardo/Calamardo.obj");
	BrazoCalamardo = Model();
	BrazoCalamardo.LoadModel("Models/BobEsponja/Calamardo/brazoCalamardo.obj");
	Gary = Model();
	Gary.LoadModel("Models/BobEsponja/Gary/Gary.obj");
	Saitama = Model();
	Saitama.LoadModel("Models/OnePunchMan/Saitama/Saitama.obj");
	Tutsumaki = Model();
	Tutsumaki.LoadModel("Models/OnePunchMan/Tutsumaki/Tutsumaki.obj");
	BodyOver = Model();
	BodyOver.LoadModel("Models/OnePunchMan/Overgrown/BodyOvergrown.obj");
	HeadOver = Model();
	HeadOver.LoadModel("Models/OnePunchMan/Overgrown/Cabeza.obj");

	//Saitama
	SaitamaBody = Model();
	SaitamaBody.LoadModel("Models/OnePunchMan/Saitama/saitamabody.obj");
	SaitamaLeftL = Model();
	SaitamaLeftL.LoadModel("Models/OnePunchMan/Saitama/saitamapiernaizq.obj");
	SaitamaRightL = Model();
	SaitamaRightL.LoadModel("Models/OnePunchMan/Saitama/saitamapiernader.obj");
	SaitamaLeftA = Model();
	SaitamaLeftA.LoadModel("Models/OnePunchMan/Saitama/saitamabrazoizq.obj");
	SaitamaRightA = Model();
	SaitamaRightA.LoadModel("Models/OnePunchMan/Saitama/saitamabrazoder.obj");

	// Vehículos
	Cangremovil = Model();
	Cangremovil.LoadModel("Models/BobEsponja/Cangremovil/Cangremovil.obj");
	Bicicleta = Model();
	Bicicleta.LoadModel("Models/OnePunchMan/Bicicleta/bici.obj");
	Nube = Model();
	Nube.LoadModel("Models/DragonBall/Nube/Nube.obj");
	llantaC = Model();
	llantaC.LoadModel("Models/BobEsponja/Cangremovil/llantaC.obj");
	llantaB = Model();
	llantaB.LoadModel("Models/OnePunchMan/Bicicleta/llantaB.obj");

	// Decoración
	Piedra1 = Model();
	Piedra1.LoadModel("Models/Decoracion/Rocas/Roquita1.obj");
	Piedra2 = Model();
	Piedra2.LoadModel("Models/Decoracion/Rocas/Roquita2.obj");
	Piedra3 = Model();
	Piedra3.LoadModel("Models/Decoracion/Rocas/Roquita3.obj");
	Piedra4 = Model();
	Piedra4.LoadModel("Models/Decoracion/Rocas/Roquita4.obj");
	Piedra5 = Model();
	Piedra5.LoadModel("Models/Decoracion/Rocas/Roquita5.obj");
	Patito = Model();
	Patito.LoadModel("Models/Decoracion/Decoracion/Patito.obj");
	LamparaZoo = Model();
	LamparaZoo.LoadModel("Models/Decoracion/Decoracion/LamparaZoo.obj");
	LamparaZoo_On = Model();
	LamparaZoo_On.LoadModel("Models/Decoracion/Decoracion/lamp_on.obj");
	Kunai = Model();
	Kunai.LoadModel("Models/Decoracion/Decoracion/Kunai.obj");
	Leon = Model();
	Leon.LoadModel("Models/Decoracion/Animales/Leon.obj");
	Pato = Model();
	Pato.LoadModel("Models/Decoracion/Animales/Pato.obj");
	Shuriken = Model();
	Shuriken.LoadModel("Models/Decoracion/Decoracion/Shuriken.obj");
	LampGar = Model();
	LampGar.LoadModel("Models/Decoracion/Decoracion/LamparaZoo1.obj");
	mesa = Model();
	mesa.LoadModel("Models/Decoracion/Decoracion/mesaKrusty.obj");
	cangre = Model();
	cangre.LoadModel("Models/Decoracion/Decoracion/burgir.obj");
	SpotlightModel = Model();
	SpotlightModel.LoadModel("Models/Decoracion/Decoracion/Spotlight.obj");
	PlankStage = Model();
	PlankStage.LoadModel("Models/Decoracion/Decoracion/PlankStage.obj");
	Medusa = Model();
	Medusa.LoadModel("Models/Decoracion/Animales/Medusa.obj");

	// Vegetación
	Bamboo = Model();
	Bamboo.LoadModel("Models/Decoracion/Vegetacion/Bamboo.obj");

	Pasto = Model(); 
	Pasto.LoadModel("Models/Decoracion/Vegetacion/Pasto.obj");
	Pasto1 = Model();
	Pasto1.LoadModel("Models/Decoracion/Vegetacion/Pasto1.obj");
	Planta1 = Model();
	Planta1.LoadModel("Models/Decoracion/Vegetacion/Planta1.obj");
	Planta2 = Model();
	Planta2.LoadModel("Models/Decoracion/Vegetacion/Planta2.obj");
	Planta3 = Model();
	Planta3.LoadModel("Models/Decoracion/Vegetacion/Planta3.obj");
	Planta4 = Model();
	Planta4.LoadModel("Models/Decoracion/Vegetacion/Planta4.obj");
	Planta5 = Model();
	Planta5.LoadModel("Models/Decoracion/Vegetacion/Planta5.obj");
	Planta6 = Model();
	Planta6.LoadModel("Models/Decoracion/Vegetacion/Planta6.obj");

	// Puertas
	LionGate = Model();
	LionGate.LoadModel("Models/Puertas/gate.obj");
	Porton = Model();
	Porton.LoadModel("Models/Puertas/porton.obj");
	PuertaMetal = Model();
	PuertaMetal.LoadModel("Models/Puertas/PuertaMetalica.obj");

	//Señalización 
	Paso = Model();
	Paso.LoadModel("Models/Decoracion/Decoracion/Paso.obj");
	PasoPeatonal = Model();
	PasoPeatonal.LoadModel("Models/Decoracion/Decoracion/PasoPeatonal.obj");
	Peligro = Model();
	Peligro.LoadModel("Models/Decoracion/Decoracion/Peligro.obj");
	Mapa = Model();
	Mapa.LoadModel("Models/Decoracion/Decoracion/MapaZoo.obj");

}

void RenderEdificios(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel) {

	glm::mat4 casa(1.0);
	// *********************************************************************
			// Capsule Corp
	// *********************************************************************
	
	model = glm::translate(model, glm::vec3(-230.0f, 1.0f, 200.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	CapsuleCorp.RenderModel();
	
	// *********************************************************************
			// Casa Bob Esponja
	// *********************************************************************G

	model = modelaux;
	model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	casa = model;
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	CasaBob.RenderModel();

	model = casa;
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Flores.RenderModel();

	model = casa;
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra.RenderModel();

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
	
	model = modelaux;
	model = glm::translate(model, glm::vec3(-250.0f, 0.0f, -250.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KameHouse.RenderModel();

	// *********************************************************************
			// Departamento Saitama
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(55.0f, 0.0f, 245.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	CasaSaitama.RenderModel();
}

void RenderPersonajes(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel, GLfloat now) {

	float x, y, z;

	// *********************************************************************
			// Tutsumaki
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3( 40.0f, 0.3f, 200.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Tutsumaki.RenderModel();
  
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

	glm::mat4 Sponge(1.0);

	model = modelaux;
	model = glm::translate(model, glm::vec3(-208.0f, 0.0f, 35.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	Sponge = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bob.RenderModel();

	//:=========== Brazo :===========

	model = Sponge;
	model = glm::translate(model, glm::vec3(4.0f, 6.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angArmB), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	BrazoBob.RenderModel();

	// *********************************************************************
			// Calamardo
	// *********************************************************************

	glm::mat4 calamardo(1.0);

	model = modelaux;
	model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -90.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	calamardo = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Calamardo.RenderModel();

	//:=========== Brazo :===========

	model = calamardo;
	model = glm::translate(model, glm::vec3(0.0f, 8.5f, 0.0f));
	model = glm::rotate(model, glm::radians(angArm), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	BrazoCalamardo.RenderModel();

	// *********************************************************************
			// Karin y León 
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-10.0f, 7.0f, 20.0f));
	if (now >= day * n && now < day * (n + 1)) {
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		if (now >= (day * (n + 1) - 5) && day < day * (n + 1)) { // Genera un glich 10 seg antes de que sea de noche 
			x = rand() % 2;
			y = rand() % 2;
			z = rand() % 2;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(x, y, z));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Karin.RenderModel();
	}
	else if (now >= day * (n + 1) && now < day * (n + 2)) { // Genera un glich 10 seg antes de que sea de día
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		if (now >= (day * (n + 2) - 5) && day < day * (n + 2)) {
			x = rand() % 2;
			y = rand() % 2;
			z = rand() % 2;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(x, y, z));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Leon.RenderModel();
	}
	
	// *********************************************************************
			// Overgrown y Gary
	// *********************************************************************

	glm::mat4 Over(1.0);
	model = modelaux;
	
	if (now >= day * n && now < day * (n + 1)) { // DIA (Gary)
		model = glm::translate(model, glm::vec3(-50.0f, 1.0f, -170.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		if (now >= (day * (n + 1) - 5) && day < day * (n + 1)) { // Genera un glich 10 seg antes de que sea de noche 
			x = rand() % 2;
			y = rand() % 2;
			z = rand() % 2;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(x, y, z));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gary.RenderModel();
	}
	else if (now >= day * (n + 1) && now <= day * (n + 2)) { // NOCHE (Overgrown) 
		model = glm::translate(model, glm::vec3(-50.0f, 10.0f, -170.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Over = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		if (now >= (day * (n + 2) - 5) && day < day * (n + 2)) {
			x = rand() % 2;
			y = rand() % 2;
			z = rand() % 2;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(x, y, z));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BodyOver.RenderModel();

		//:=========== Cabeza :===========

		model = Over;
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 7.0f));
		model = glm::rotate(model, glm::radians(angleHead), glm::vec3(0.0f, 1.0f, 0.0f));
		if (now >= (day * (n + 2) - 5) && day < day * (n + 2)) {
			x = rand() % 2;
			y = rand() % 2;
			z = rand() % 2;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(x, y, z));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HeadOver.RenderModel();
	}	

	
}

void RenderVehiculos(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel) {

	// *********************************************************************
		// Bicicleta 
	// *********************************************************************
	glm::mat4 bici(1.0);
	model = modelaux;
	model = glm::translate(model, glm::vec3(movbicix, 0.0f, movbiciz));
	if (avanza) {
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (dig) {
		model = glm::rotate(model, glm::radians(213.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (ret) {
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	bici = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bicicleta.RenderModel();

	//:============ LLANTA TRASERA :============

	model = bici;
	model = glm::translate(model, glm::vec3(0.5f, 3.6f, 5.6f));
	model = glm::rotate(model, glm::radians(rotllantaB), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	llantaB.RenderModel();

	//:============ LLANTA DELANTERA :============

	model = bici;
	model = glm::translate(model, glm::vec3(0.5f, 3.3f, -5.7f));
	model = glm::rotate(model, glm::radians(rotllantaB), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	llantaB.RenderModel();

	// *********************************************************************
		// Cangremovil
	// *********************************************************************
	glm::mat4 cangre(1.0);

	model = modelaux;
	if (movcangre < 94) { // EL COCHE AVANZA
		model = glm::translate(model, glm::vec3(180.0f, 1.3f, movcangre));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cangre = model;
	}
	else { // EL COCHE SE VOLCA
		
		model = glm::translate(model, glm::vec3(180.0f, -0.031*pow(movcangre,2) + 7.78 * movcangre - 449.617, movcangre));
		model = glm::rotate(model, glm::radians(ang), glm::vec3(1.0f, 1.0f, 1.0f));
		cangre = model;
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Cangremovil.RenderModel();

	//:============ LLANTAS DELANTERAS :============
	
	model = cangre;
	model = glm::translate(model, glm::vec3(10.0f, 1.0f, -8.0f)); // Derecha
	model = glm::rotate(model, glm::radians(-rotllantaC), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
	llantaC.RenderModel();

	model = cangre;
	model = glm::translate(model, glm::vec3(-8.0f, 1.0f, -8.0f)); // Izquierda
	model = glm::rotate(model, glm::radians(-rotllantaC),glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	llantaC.RenderModel();

	//:============ LLANTAS TRASERAS :============

	model = cangre;
	model = glm::translate(model, glm::vec3(12.0f, 1.0f, 4.0f)); // Derecha
	model = glm::rotate(model, glm::radians(-rotllantaC), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	llantaC.RenderModel();

	model = cangre;
	model = glm::translate(model, glm::vec3(-10.0f, 1.0f, 4.0f)); // Izquierda
	model = glm::rotate(model, glm::radians(-rotllantaC), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	llantaC.RenderModel();

	// *********************************************************************
		// Nube
	// *********************************************************************

	model = modelaux;

	//Trayecto nube

	if (nubeState == 0) {
		if (movnubeX < -50) {
			//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			nubeState = 1;
		}
		movnubeX -= movnubeoffset * deltaTime;
	}
	else if (nubeState == 1) {
		if (movnubeZ < -50) {
			nubeState = 2;
			//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		movnubeX += movnubeoffset * deltaTime;
		movnubeZ -= movnubeoffset * deltaTime;
	}
	else if (nubeState == 2) {
		if (movnubeZ > 50) {
			//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			nubeState = 2;
		}
		movnubeX += movnubeoffset * deltaTime;
		movnubeZ += movnubeoffset * deltaTime;
	}

	model = glm::translate(model, glm::vec3(movnubeX, 10.0 * sin(glm::radians(movnubeY) * 0.1f) + 70, movnubeZ));
	model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Nube.RenderModel();
}

void RenderDecoracion(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel) {

	// *********************************************************************
		// Roca 1
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-177.0f, 0.4f, 85.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra1.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-280.0f, 0.4f, -40.0f));
	model = glm::scale(model, glm::vec3(3.5f, 8.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra1.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-275.0f, 0.4f, -170.0f));
	model = glm::scale(model, glm::vec3(10.5f, 15.5f, 10.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra1.RenderModel();

	//
	model = modelaux;
	model = glm::translate(model, glm::vec3(110.0f, 0.4f, -100.0f));
	model = glm::scale(model, glm::vec3(10.5f, 15.5f, 10.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra1.RenderModel();

	// *********************************************************************
		// Roca 2
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-180.3f, 0.4f, 74.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra2.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-280.3f, 0.4f, -30.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 7.0f, 4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra2.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-267.3f, 0.4f, -150.0f));
	model = glm::scale(model, glm::vec3(7.0f, 5.0f, 7.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra2.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(127.3f, 0.4f, -110.0f));
	model = glm::scale(model, glm::vec3(7.0f, 5.0f, 7.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra2.RenderModel();

	// *********************************************************************
		// Roca 3
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-185.0f, 0.4f, 90.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-280.0f, 0.4f, -120.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-275.0f, 0.4f, -35.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(145.0f, 0.4f, -40.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(200.0f, 0.4f, 85.0f));
	model = glm::scale(model, glm::vec3(5.5f, 3.5f, 5.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra3.RenderModel();

	//Saitama
	model = modelaux;
	model = glm::translate(model, glm::vec3(145.0f, 0.4f, 230.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(145.0f, 0.4f, 250.0f));
	model = glm::scale(model, glm::vec3(5.0f, 3.5f, 5.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra3.RenderModel();

	// *********************************************************************
		// Roca 4
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-280.0f, 0.4f, -130.0f));
	model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra4.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-250.0f, 0.4f, -170.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra4.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(95.0f, 0.4f, -142.5f));
	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra4.RenderModel();

	//
	model = modelaux;
	model = glm::translate(model, glm::vec3 (30.0f, 0.4f, 80.5f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra4.RenderModel();

	//camino
	model = modelaux;
	model = glm::translate(model, glm::vec3(160.0f, 0.4f, 80.0f));
	model = glm::scale(model, glm::vec3(3.0f, 4.5f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra4.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(160.0f, 0.4f, 50.0f));
	model = glm::scale(model, glm::vec3(3.0f, 4.5f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra4.RenderModel();

	//Saitama
	model = modelaux;
	model = glm::translate(model, glm::vec3(120.0f, 0.4f, 250.0f));
	model = glm::scale(model, glm::vec3(3.0f, 4.5f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra4.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(100.0f, 0.4f, 230.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.5f, 4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra4.RenderModel();

	// *********************************************************************
		// Roca 5
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-180.0f, 0.4f, -30.0f));
	model = glm::scale(model, glm::vec3(10.0f, 3.0f, 10.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra5.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(157.0f, 0.4f, 52.0f));
	model = glm::scale(model, glm::vec3(5.0f, 3.0f, 5.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra5.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(65.0f, 0.4f, 80.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 8.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra5.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(85.0f, 0.4f, 70.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 8.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra5.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(155.0f, 0.4f, 230.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Piedra5.RenderModel();


	// *********************************************************************
		// Patito
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(40.0f, 19.5f, 200.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Patito.RenderModel();

	// *********************************************************************
		// Kunai
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-190.0f, 0.3f, 70.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Kunai.RenderModel();

	// *********************************************************************
		// Pato
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-190.0f, 0.0f, 80.0f));
	model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pato.RenderModel();

	// *********************************************************************
		// Shuriken
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(20.0f, 0.0f, 20.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Shuriken.RenderModel();

	// *********************************************************************
		// Bamboo
	// *********************************************************************

	//Derecha
	model = modelaux;
	model = glm::translate(model, glm::vec3(-70.0f, 0.0f, -75.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-53.0f, 0.0f, -75.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-36.0f, 0.0f, -75.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-17.0f, 0.0f, -75.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, -75.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	//Izquierda
	model = modelaux;
	model = glm::translate(model, glm::vec3(-70.0f, 0.0f, -280.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-53.0f, 0.0f, -280.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-36.0f, 0.0f, -280.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-17.0f, 0.0f, -280.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, -280.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	//Atrás
	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -274.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -258.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -242.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -226.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -210.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -194.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -178.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -162.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -146.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -130.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -114.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -98.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, -82.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bamboo.RenderModel();

	// *********************************************************************
		// Pasto
	// *********************************************************************

	//Casa Calamardo
	model = modelaux;
	model = glm::translate(model, glm::vec3(-190.0f, 0.0f, -30.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -40.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-275.0f, 0.0f, -130.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-275.0f, 0.0f, -45.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	//Casa Bob
	model = modelaux;
	model = glm::translate(model, glm::vec3(-180.0f, 0.0f, 65.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-245.0f, 0.0f, 60.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-240.0f, 0.0f, 73.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-280.0f, 0.0f, 60.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	// Vegetación 
	model = modelaux;
	model = glm::translate(model, glm::vec3(55.0f, 0.0f, 60.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(90.0f, 0.0f, 80.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(45.0f, 0.0f, -50.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(95.0f, 0.0f, -185.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(195.0f, 0.0f, 75.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(220.0f, 0.0f, 95.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(195.0f, 0.0f, 80.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(185.0f, 0.0f, 60.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(170.0f, 0.0f, 3.5f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(180.0f, 0.0f, 50.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(135.0f, 0.0f, -70.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(125.0f, 0.0f, 60.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(95.0f, 0.0f, 70.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(95.0f, 0.0f, 6.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(85.0f, 0.0f, -140.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(80.0f, 0.0f, -160.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(80.0f, 0.0f, 60.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(55.0f, 0.0f, -160.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(50.0f, 0.0f, -175.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto.RenderModel();

	// *********************************************************************
		// Pasto 1
	// *********************************************************************

	//Casa Calamardo
	model = modelaux;
	model = glm::translate(model, glm::vec3(-184.5f, 0.0f, -120.0f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 0.5f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto1.RenderModel();

	//Casa Bob
	model = modelaux;
	model = glm::translate(model, glm::vec3(-260.3f, 0.0f, 80.0f));
	model = glm::scale(model, glm::vec3(2.0f, 0.5f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto1.RenderModel();

	//Casa Roshi 
	model = modelaux;
	model = glm::translate(model, glm::vec3(-184.5f, 0.0f, -220.0f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 0.5f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Pasto1.RenderModel();

	// *********************************************************************
		// Planta 1
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(255.0f, 0.0f, 90.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta1.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-200.0f, 0.0f, 75.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta1.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(55.0f, 0.0f, -90.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta1.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(55.0f, 0.0f, -155.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta1.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(80.0f, 0.0f, 45.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta1.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(120.0f, 0.0f, -80.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta1.RenderModel();

	//Karin
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 85.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta1.RenderModel();

	// *********************************************************************
		// Planta 2
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(35.0f, 0.0f, -265.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta2.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(130.0f, 0.0f, 52.0f));
	model = glm::scale(model, glm::vec3(3.0f, 2.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta2.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(70.0f, 0.0f, -152.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta2.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(70.0f, 0.0f, -70.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta2.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(50.0f, 0.0f, 50.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta2.RenderModel();
	// *********************************************************************
		// Planta 3
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(80.0f, 0.0f, -185.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(100.0f, 0.0f, 25.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(205.0f, 0.0f, 40.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(50.0f, 0.0f, 70.5f));
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta3.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(130.0f, 0.0f, -45.5f));
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta3.RenderModel();

	// Saitama
	model = modelaux;
	model = glm::translate(model, glm::vec3(110.0f, 0.0f, 230.5f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta3.RenderModel();

	// *********************************************************************
		// Planta 4
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(120.0f, 0.0f, -80.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta4.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(95.0f, 0.0f, 52.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta4.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(157.0f, 0.0f, -30.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta4.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(45.0f, 0.0f, -195.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta4.RenderModel();

	//Karin
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -55.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta4.RenderModel();

	//Gary
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -90.0f));
	model = glm::scale(model, glm::vec3(3.0f, 2.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta4.RenderModel();

	//Bob
	model = modelaux;
	model = glm::translate(model, glm::vec3(-265.0f, 0.0f, 75.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta4.RenderModel();

	//Saitama
	model = modelaux;
	model = glm::translate(model, glm::vec3(130.0f, 0.0f, 250.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta4.RenderModel();

	// *********************************************************************
		// Planta 5
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(195.0f, 0.0f, 3.5f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta5.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(215.0f, 0.0f, 80.5f));
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta5.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(150.0f, 0.0f, -35.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta5.RenderModel();

	//Karin
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 95.0f));
	model = glm::scale(model, glm::vec3(4.0f, 3.5f, 4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta5.RenderModel();

	//Gary
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -100.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta5.RenderModel();

	// *********************************************************************
		// Planta 6
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(195.0f, 0.0f, 50.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta6.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(55.0f, 0.0f, 4.5f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta6.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(155.0f, 0.0f, -80.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta6.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(230.0f, 0.0f, 70.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta6.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(130.0f, 0.0f, -55.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta6.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(130.0f, 0.0f, -55.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta6.RenderModel();

	//Gary
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -270.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta6.RenderModel();

	//Saitama
	model = modelaux;
	model = glm::translate(model, glm::vec3(150.0f, 0.0f, 230.5f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Planta6.RenderModel();

	// *********************************************************************
		// Luces
	// *********************************************************************
	
	model = modelaux;
	model = glm::translate(model, glm::vec3(130.0f, 0.0f, -100.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LampGar.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-75.0f, 0.0f, -155.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LampGar.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(93.0f, 7.0f, 220.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LampGar.RenderModel();

	// *********************************************************************
		// Stage de Leon
	// *********************************************************************
	
	model = modelaux;
	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 20.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	PlankStage.RenderModel();

	// *********************************************************************
		// Mesa
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-215.0f, 0.0f, 35.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mesa.RenderModel();

	// *********************************************************************
		// Cangreburger
	// *********************************************************************
	
	
	for (int i = 0; i < burgir; i++) {
		model = modelaux;
		model = glm::translate(model, glm::vec3(-213.0f - 2*i, 4.0f, 35.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cangre.RenderModel();
	}

	if (aire){
		model = modelaux;
		model = glm::translate(model, glm::vec3(-210.5f, (-0.005 * pow(movburgir, 2)) - 0.362 * movburgir + 23.804, movburgir));
		model = glm::rotate(model, glm::radians(angburgir), glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cangre.RenderModel();
	}

	// *********************************************************************
		// Medusa
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-40.0f, movM1, -155.0f));
	model = glm::scale(model, glm::vec3(scaleM, scaleM, scaleM));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Medusa.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-50.0f, movM2, -175.0f));
	model = glm::scale(model, glm::vec3(scaleM, scaleM, scaleM));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Medusa.RenderModel();

	// *********************************************************************
		// Señalización
	// *********************************************************************

	// Mapa
	model = modelaux;
	model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 110.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Mapa.RenderModel();

	// Peligro
	model = modelaux;
	model = glm::translate(model, glm::vec3(-85.0f, 0.0f, -180.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Peligro.RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(-85.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Peligro.RenderModel();

	// Paso Peatonal
	model = modelaux;
	model = glm::translate(model, glm::vec3(210.0f, 0.0f, 200.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	PasoPeatonal.RenderModel();

	// Paso
	model = modelaux;
	model = glm::translate(model, glm::vec3(200.0f, 4.0f, 170.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Paso.RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(200.0f, 4.0f, 180.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Paso.RenderModel();
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
		meshList[5]->RenderMesh();
		*model = glm::translate(*model, dir);
	}
}

void RenderOutsideWalls(glm::mat4 model, GLuint uniformModel, GLuint uniformColor, Texture* brickTexture,
	GLuint uniformSpecularIntensity, GLuint uniformShininess, GLuint uniformTextureOffset, glm::vec2 toffset) {

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 wallModelAux(1.0);
	model = glm::translate(model, glm::vec3(-10.0f, 3.5f, 275.0f));
	wallModelAux = model;

	model = glm::scale(model, glm::vec3(20.0f, 10.0f, 2.0f));
	model = glm::translate(model, glm::vec3(-4.5f, 0.0f, 0.0f));
	buildWall(19, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	model = wallModelAux;
	model = glm::scale(model, glm::vec3(20.0f, 10.0f, 2.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 0.0f, 0.0f));
	buildWall(8, glm::vec3(-1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	model = wallModelAux;
	model = glm::scale(model, glm::vec3(2.0f, 10.0f, 20.0f));
	model = glm::translate(model, glm::vec3(140.0f, 0.0f, -0.5f));
	buildWall(3, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	buildWall(22, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	//Porton
	model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.2f));
	model = glm::translate(model, glm::vec3(0.0f, -1.7f, 120.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Porton.RenderModel();

	model = wallModelAux;
	model = glm::scale(model, glm::vec3(2.0f, 10.0f, 20.0f));
	model = glm::translate(model, glm::vec3(-140.0f, 0.0f, -0.5f));
	buildWall(28, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	model = wallModelAux;
	model = glm::scale(model, glm::vec3(20.0f, 10.0f, 2.0f));
	model = glm::translate(model, glm::vec3(-13.5f, 0.0f, -280.0f));
	buildWall(6, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
	buildWall(18, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, brickTexture, uniformSpecularIntensity, uniformShininess, color);

	//Puerta Metal
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 2.0f));
	model = glm::translate(model, glm::vec3(-102.5f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	PuertaMetal.RenderModel();

	// Puerta Principal
	model = wallModelAux;
	model = glm::scale(model, glm::vec3(0.35f, 0.4f, 0.5f));
	model = glm::translate(model, glm::vec3(-314.0f, -9.0f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LionGate.RenderModel();

	toffsetbanneru += 0.005;
	if (toffsetbanneru > 1.0)
		toffsetbanneru = 0.0;

	toffset = glm::vec2(toffsetbanneru, 0.0f);
	model = wallModelAux;
	model = glm::translate(model, glm::vec3(-110.0f, 15.5f, 1.5f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(15.0f, 7.0f,4.5f));
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	GateBannerTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[4]->RenderMesh();
}

void RenderInnerWalls(glm::mat4 model, GLuint uniformModel, GLuint uniformColor, std::vector<Texture*> wallTextures,
	GLuint uniformSpecularIntensity, GLuint uniformShininess) {
	
	float WALL_HEIGHT = 5.0f;
	float WALL_LENGTH = 17.0f;
	float WALL_WIDTH = 2.0f;

	float WALL_YPOS = -0.2f;

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 wallModelAux(1.0);
	model = glm::translate(model, glm::vec3(-10.0f, 3.5f, 275.0f));
	wallModelAux = model;

	//Paredes eje X
	model = glm::scale(model, glm::vec3(WALL_LENGTH, WALL_HEIGHT, WALL_WIDTH));
	model = glm::translate(model, glm::vec3(-16.0f, WALL_YPOS, -90.0f));
	buildWall(7, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, wallTextures[2], uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(-7.06f, 0.0f, -58.7f));
	buildWall(7, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, wallTextures[2], uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(-7.06f, 0.0f, -59.3f));
	buildWall(7, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, wallTextures[2], uniformSpecularIntensity, uniformShininess, color);

	model = glm::translate(model, glm::vec3(5.3f, 0.0f, 35.0f));
	buildWall(6, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, wallTextures[2], uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 88.0f));
	buildWall(6, glm::vec3(-1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, wallTextures[2], uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f));
	buildWall(15, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, wallTextures[2], uniformSpecularIntensity, uniformShininess, color);

	//Paredes eje Z
	model = wallModelAux;
	model = glm::scale(model, glm::vec3(WALL_WIDTH, WALL_HEIGHT, WALL_LENGTH));
	model = glm::translate(model, glm::vec3(-81.29f, WALL_YPOS, -11.05f));
	buildWall(3, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	buildWall(3, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);
	buildWall(3, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	buildWall(3, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);
	buildWall(1, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.4f));
	buildWall(6, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);

	model = glm::translate(model, glm::vec3(45.0f, 0.0f, 1.0f));
	buildWall(8, glm::vec3(0.0f, 0.0f, 1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	buildWall(6, glm::vec3(0.0f, 0.0f, 1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	buildWall(7, glm::vec3(0.0f, 0.0f, 1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);

	model = glm::translate(model, glm::vec3(50.0f, 0.0f, -1.0f));
	buildWall(23, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, wallTextures[1], uniformSpecularIntensity, uniformShininess, color);

	//Camino de piedra
	model = wallModelAux;
	model = glm::scale(model, glm::vec3(40.0f, 0.2f, 40.0f));
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -1.5f));
	buildWall(12, glm::vec3(0.0f, 0.0f, -1.0f), &model, uniformModel, uniformColor, wallTextures[3], uniformSpecularIntensity, uniformShininess, color);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 11.0f));
	buildWall(9, glm::vec3(1.0f, 0.0f, 0.0f), &model, uniformModel, uniformColor, wallTextures[3], uniformSpecularIntensity, uniformShininess, color);

}

void RenderLamps(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel, GLfloat now) {

	// *********************************************************************
		// Lámpara Zoológico
	// *********************************************************************

	model = modelaux;
	model = glm::translate(model, glm::vec3(-90.0f, 0.0f, -70.0f));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (now >= day * n && now < day * (n + 1)) {
		LamparaZoo.RenderModel();
	}
	else if (now >= day * (n + 1) && now <= day * (n + 2)) {
		LamparaZoo_On.RenderModel();
	}

	model = modelaux;
	model = glm::translate(model, glm::vec3(-167.0f, 0.0f, -140.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	if (now >= day * n && now < day * (n + 1)) {
		LamparaZoo.RenderModel();
	}
	else if (now >= day * (n + 1) && now <= day * (n + 2)) {
		LamparaZoo_On.RenderModel();
	}

	model = modelaux;
	model = glm::translate(model, glm::vec3(-167.0f, 0.0f, -25.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	if (now >= day * n && now < day * (n + 1)) {
		LamparaZoo.RenderModel();
	}
	else if (now >= day * (n + 1) && now <= day * (n + 2)) {
		LamparaZoo_On.RenderModel();
	}

	model = modelaux;
	model = glm::translate(model, glm::vec3(-10.0f, 7.0f, 50.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SpotlightModel.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-10.0f, 7.0f, -10.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SpotlightModel.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-120.0f, 21.0f, 273.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SpotlightModel.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-190.0f, 0.0f, -120.0f));
	model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SpotlightModel.RenderModel();
}

void RenderSaitama(glm::mat4 model, glm::mat4 modelaux, GLuint uniformModel, GLuint uniformTextureOffset, glm::vec2 toffset) {


	if (mainWindow.getWalkFlag() == 1) {
		if (leftrot > 45.0) {
			leftinc = -WALK_ANIM_SPEED;
		}
		else if(leftrot < -45){
			leftinc = WALK_ANIM_SPEED;
		}

		if (rightrot > 45.0) {
			rightinc = -WALK_ANIM_SPEED;
		}
		else if (rightrot < -45) {
			rightinc = WALK_ANIM_SPEED;
		}

		leftrot += leftinc;
		rightrot += rightinc;
	}
	else {
		leftrot = 0.0f;
		rightrot = 0.0f;
	}


	model = modelaux;
	if (cameraState) {
		model = glm::translate(model, glm::vec3(camera.getCameraPosition().x, camera.getCameraPosition().y - 20.0f, camera.getCameraPosition().z - 30.0f));
	}
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	modelaux = model;
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SaitamaBody.RenderModel();

	model = glm::translate(model, glm::vec3(1.0f, -1.5f, 0.0f));
	model = glm::rotate(model, glm::radians(leftrot), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SaitamaLeftL.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, -1.5f, 0.0f));
	model = glm::rotate(model, glm::radians(rightrot), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SaitamaRightL.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(1.5f, 7.3f, -1.0f));
	model = glm::rotate(model, glm::radians(rightrot), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SaitamaLeftA.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 7.3f, -1.0f));
	model = glm::rotate(model, glm::radians(leftrot), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SaitamaRightA.RenderModel();
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CreateCubeMesh();

	camera = camaraAvatar;

	pisoTexture = Texture("Textures/Skybox/floor.tga");
	pisoTexture.LoadTextureA();
	pisoNigth = Texture("Textures/floor_N2.tga");
	pisoNigth.LoadTextureA();

	brickTexture = Texture("Textures/brick2.png");
	brickTexture.LoadTextureA();
	wallTextures.push_back(&brickTexture);

	bushTexture = Texture("Textures/bushtexture.jpg");
	bushTexture.LoadTextureA();
	wallTextures.push_back(&bushTexture);

	rockWallTexture = Texture("Textures/rockwall.png");
	rockWallTexture.LoadTextureA();
	wallTextures.push_back(&rockWallTexture);

	pathTexture = Texture("Textures/stonepath.png");
	pathTexture.LoadTextureA();
	wallTextures.push_back(&pathTexture);


	GateBannerTexture = Texture("Textures/banner.png");
	GateBannerTexture.LoadTextureA();

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

	//-------Ponit Ligth---------
	
	unsigned int pointLightCount = 0;

	//Luz Jardin 
	pointLights[0] = PointLight(0.85f, 0.25f, 0.0f, //color
		0.5f, 1.0f,
		130.0f, 6.0f, -100.0f, //coordenadas
		0.01f, 0.01f, 0.001f);
		//0.001f, 0.02f, 0.001f);
	pointLightCount++;

	//Luz Gary 
	unsigned int pointLightCount2 = 0;
	pointLights2[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		-75.0f, 5.0f, -155.0,
		0.01f, 0.2f, 0.01f);
	pointLightCount2++;

	//Luz Depa
	unsigned int pointLightCount3 = 0;
	pointLights3[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		93.0f, 12.0f, 220.0,
		0.01f, 0.2f, 0.01f);
	pointLightCount3++;

	unsigned int pointLightCount4 = 3;
	pointLights4[0] = pointLights[0];
	pointLights4[1] = pointLights2[0];
	pointLights4[2] = pointLights3[0];

	//-------Spot Ligth---------

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		-190.f, 2.0f, -120.0f,
		-5.0f, 1.5f, 5.0f,
		1.5f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 0.8f,
		1.0f, 2.0f,
		-120.0f, 21.0f, 285.0f,
		0.0f, -10.0f, -35.0f,
		5.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//Spotlight leon
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.5f,
		1.0f, 2.0f,
		-10.0f, 9.0f, 60.0f,
		0.0f, 1.5f, -5.0f,
		1.5f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana

	movnubeoffset = 0.1f;
	movnubeYoffset = 12.0f;

	//------------------SONIDO-----------------------
	
	ISoundEngine* Ambiental = createIrrKlangDevice();
	Ambiental->play2D("Media/ambiental.mp3", true);
	Ambiental->setSoundVolume(0.35f);

	Pos1 = createIrrKlangDevice();
	Pos2 = createIrrKlangDevice();

	Pos1->play2D("Media/DragonBallRap.mp3", true);
	Pos2->play2D("media/OnePunchMan.mp3",true);

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		angle += 0.1f;

		//Recibir eventos del usuario
		glfwPollEvents();
		//setCamera(mainWindow.getTipoCamara());

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Camara

		if (mainWindow.getCameraSwitch()) {
			if(cameraState) {
				camaraAvatar = camera;
				camera = camaraAerea;
				cameraState = !cameraState;
			}
			else {
				camaraAerea = camera;
				camera = camaraAvatar;
				cameraState = !cameraState;
			}
			mainWindow.clearCameraSwitch();
		}
		
		// Activa el audio posicional (GOKU)
		printf("x: %f z: %f \n", camera.getCameraPosition().x, camera.getCameraPosition().z);
		if ((camera.getCameraPosition().x <= -210.0f && camera.getCameraPosition().x >= -255.0f) && (camera.getCameraPosition().z >= -280.0f && camera.getCameraPosition().z <= -210.0f)) {
			Ambiental->setSoundVolume(0.0f);
			Pos1->setSoundVolume(0.45f);
		}
		else if ((camera.getCameraPosition().x <= -180.0f && camera.getCameraPosition().x >= -265.0f) && (camera.getCameraPosition().z >= -280.0f && camera.getCameraPosition().z <= -180.0f)) {
			Ambiental->setSoundVolume(0.05f);
			Pos1->setSoundVolume(0.35f);
		}
		else if ((camera.getCameraPosition().x <= -150.0f && camera.getCameraPosition().x >= -275.0f) && (camera.getCameraPosition().z >= -280.0f && camera.getCameraPosition().z <= -150.0f)) {
			Ambiental->setSoundVolume(0.15f);
			Pos1->setSoundVolume(0.25f);
		}
		else if ((camera.getCameraPosition().x <= -120.0f && camera.getCameraPosition().x >= -270.0f) && (camera.getCameraPosition().z >= -280.0f && camera.getCameraPosition().z <= -120.0f)) {
			Ambiental->setSoundVolume(0.2f);
			Pos1->setSoundVolume(0.15f);
		}
		else {
			Pos1->setSoundVolume(0.0f);
		}

		// Activa el audio posicional (BANG)
		if ((camera.getCameraPosition().x <= 100.0f && camera.getCameraPosition().x >= 5.0f) && (camera.getCameraPosition().z >= 180.0f && camera.getCameraPosition().z <= 280.0f)) {
			printf("VOL 4 \n");
			Ambiental->setSoundVolume(0.0f);
			Pos2->setSoundVolume(0.45f);
		}
		else if ((camera.getCameraPosition().x <= 124.0f && camera.getCameraPosition().x >= -19.0f) && (camera.getCameraPosition().z >= 160.0f && camera.getCameraPosition().z <= 280.0f)) {
			printf("VOL 3 \n");
			Ambiental->setSoundVolume(0.05f);
			Pos2->setSoundVolume(0.35f);
		}
		else if ((camera.getCameraPosition().x <= 148.0f && camera.getCameraPosition().x >= -43.0f) && (camera.getCameraPosition().z >= 140.0f && camera.getCameraPosition().z <= 280.0f)) {
			printf("VOL 2 \n");
			Ambiental->setSoundVolume(0.15f);
			Pos2->setSoundVolume(0.25f);
		}
		else if ((camera.getCameraPosition().x <= 172.0f && camera.getCameraPosition().x >= -67.0f) && (camera.getCameraPosition().z >= 120.0f && camera.getCameraPosition().z <= 280.0f)) {
			printf("VOL 1 \n");
			Ambiental->setSoundVolume(0.2f);
			Pos2->setSoundVolume(0.15f);
		}
		else {
			Pos2->setSoundVolume(0.0f);
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//printf("mov1: %f mov2: %f \n", movM1, movM2);

		if (movM1 > 87.0f || movM1 < 10.0f) {
			movM1 = 22.0f;
		}

		if (movM2 > 77.0f || movM2 < 8.0f) {
			movM2 = 12.0f;
		}

		if (up1) {
			if (movM1 < 85.0f) {
				movM1 += movOffSetM * deltaTime;
			}
			else {
				up1 = !up1;
			}
		}
		else {
			if (movM1 > 20.f) {
				movM1 -= movOffSetM * deltaTime;
			}
			else {
				up1 = !up1;
			}
		}

		if (up2) {
			if (movM2 < 75.0f) {
				movM2 += movOffSetM * deltaTime;
			}
			else {
				up2 = !up2;
			}
		}
		else {
			if (movM2 > 10.f) {
				movM2 -= movOffSetM * deltaTime;
			}
			else {
				up2 = !up2;
			}
		}

		if (scaleM > 1.3f || scaleM < 0.3f) {
			scaleM = 1.0f;
		}

		if (crece) {
			if (scaleM > 0.5f) {
				scaleM -= scalOffSetC * deltaTime;
			}
			else {
				crece = !crece;
			}
		}
		else {
			if (scaleM < 1.0f) {
				scaleM += scalOffSetC * deltaTime;
			}
			else {
				crece = !crece;
			}
		}

		if (burgir == 0) {
			burgir = 3;
		}

		if (!aire) { // Para el movimiento del lanzamiento de la burgir
			if (angArmB > -40) {
				angArmB -= 0.5f;
			}
			else {
				aire = !aire;
				burgir -= 1;
			}
		}
		else {
			if (angArmB < 25) {
				angArmB += 3.0f;
			}

			if (movburgir > -110) {
				movburgir -= movOffsetburgir * deltaTime;
				angburgir += 8.0f;
			}
			else {
				aire = !aire;
				movburgir = 35.8f;
			}
		}
	
		if (mover) { // Para mover la cabeza de Overgrown
			if (angleHead < 35) {
				angleHead += 0.5f;
			}
			else {
				mover = !mover;
			}
		}
		else {
			if (angleHead > -35) {
				angleHead -= 0.5f;
			}
			else {
				mover = !mover;
			}
		}

		if (saludo) { // Para el saludo de Calamardo
			if (angArm < 45) {
				angArm += 1.0f;
			}
			else {
				saludo = !saludo;
			}
		}
		else {
			if (angArm > -45) {
				angArm -= 1.0f;
			}
			else {
				saludo = !saludo;
			}
		}

		

		if ( (movbicix > 41 && movbicix < 261) && (movbiciz < 77 && movbiciz > -261)) {
			if (avanza) { // Para mover la bici
				if (movbiciz < 75) {
					movbiciz += movbiciOffset * deltaTime;
					rotllantaB += 7.0f;
				}
				else {
					avanza = !avanza;
				}
			}
			else if (dig) {
				if (movbiciz > -250) {
					movbicix = (movbiciz + 330.11) / 1.558;
					movbiciz -= movbiciOffset * deltaTime;
					rotllantaB += 7.0f;
				}
				else {
					dig = !dig;
				}
			}
			else if (ret) {
				if (movbicix < 259) {
					movbicix += movbiciOffset * deltaTime;
					rotllantaB += 7.0f;
				}
				else {
					avanza = !avanza;
					dig = !dig;
				}
			}
		}
		else {
			movbicix = 255;
			movbiciz = -259;
		}

		if (movcangre >= -250 && movcangre < 94) { // Para el movimiento del coche
			movcangre += movOffSetC * deltaTime;
			rotllantaC += rotllantaOffSetC * deltaTime;
		}
		else if (movcangre >= 94 && movcangre < 160){
			movcangre += movOffsetC2 * deltaTime;
			ang += 5.0f;
		}
		else {
			movcangre = -250.0f;
		}

		if (movnubeYoffset > 360.0f) {
			movnubeYoffset = 0.0f;
		}

		movnubeY += movnubeYoffset;

		if (now >= day*(n + 2)) {
			n = n + 2;
		}

		if (now >= day * n && now < day*(n + 1)) { // DIA
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else if (now >= day*(n+1) && now <= day * (n + 2)) { // NOCHE
			skyboxNigth.DrawSkybox(camera.calculateViewMatrix(), projection);
		}

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
		
		//información al shader de fuentes de iluminación

		if (now >= day * n && now < day * (n + 1)) {
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetSpotLights(spotLights, spotLightCount-3);
		}
		else if (now >= day * (n + 1) && now <= day * (n + 2)) {
			shaderList[0].SetDirectionalLight(&mainLightNigth);
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}

		//PointLights

		if (mainWindow.getLuces() == 0.0) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (mainWindow.getLuces() == 1.0) {
			shaderList[0].SetPointLights(pointLights2, pointLightCount2);
		}
		else if (mainWindow.getLuces() == 2.0) {
			shaderList[0].SetPointLights(pointLights3, pointLightCount3);
		}
		else if(mainWindow.getLuces() == 3.0){
			shaderList[0].SetPointLights(pointLights4, pointLightCount4);
		}
		else if (mainWindow.getLuces() == 4.0) {
			shaderList[0].SetPointLights(pointLights4, pointLightCount4-3);
		}
    
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


		// *********************************************************************
			// Camara Saitama
		// *********************************************************************
		//++++++++++++++++++

		RenderSaitama(model, modelaux, uniformModel, uniformTextureOffset, toffset);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	
		if (now >= day * n && now < day * (n + 1)) {
			pisoTexture.UseTexture();
		}
		else if (now >= day * (n + 1) && now <= day * (n + 2)) {
			pisoNigth.UseTexture();
		}
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = modelaux;

		//*****************************************************************
				// CARGA LOS EDIFICIOS
		//*****************************************************************
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		RenderEdificios(model, modelaux, uniformModel);

		//*****************************************************************
				// CARGA LOS PERSONAJES
		//*****************************************************************

		RenderPersonajes(model, modelaux, uniformModel, now);

		//*****************************************************************
				// CARGA LOS VEHÍCULOS
		//*****************************************************************

		RenderVehiculos(model, modelaux, uniformModel);
		//*****************************************************************
				// CARGA LA DECORACIÓN
		//*****************************************************************

		RenderDecoracion(model, modelaux, uniformModel);

		//*****************************************************************
				// CARGA PAREDES EXTERNAS DE ESCENA
		//*****************************************************************

		RenderOutsideWalls(model, uniformModel, uniformColor, &brickTexture, uniformSpecularIntensity, uniformShininess, uniformTextureOffset, toffset);

		//*****************************************************************
				// CARGA MUROS INTERNOS
		//*****************************************************************
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		RenderInnerWalls(model, uniformModel, uniformColor, wallTextures, uniformSpecularIntensity, uniformShininess);
		
		//*****************************************************************
				// CARGA LAMPARAS
		//*****************************************************************

		RenderLamps(model, modelaux, uniformModel, now);

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
