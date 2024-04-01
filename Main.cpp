/*
Proyecto Compu Gráfica
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
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
// Edificios
Model KameHouse, Capsule, CasaBob, CasaCalamardo;
// Personajes
Model Roshi, Bob, Calamardo, Gary, Karin, Overgrown;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





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

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void LoadModels() {
	// Edificios
	KameHouse = Model();
	KameHouse.LoadModel("Models/DragonBall/KameHouse/kameHouse.obj");
	Capsule = Model();
	Capsule.LoadModel("Models/DragonBall/CapsuleCorp/CapsuleCorp.obj");
	CasaBob = Model();
	CasaBob.LoadModel("Models/BobEsponja/CasaBob/CasaBob.obj");
	CasaCalamardo = Model();
	CasaCalamardo.LoadModel("Models/BobEsponja/CasaCalamardo/CasaCalamardo.obj");
	// Personajes
	Roshi = Model();
	Roshi.LoadModel("Models/DragonBall/MaestroRoshi/Roshi.obj");
	Bob = Model();
	Bob.LoadModel("Models/BobEsponja/Bob/bob.obj");
	Calamardo = Model();
	Calamardo.LoadModel("Models/BobEsponja/Calamardo/Deltarune.obj");
	Gary = Model();
	Gary.LoadModel("Models/BobEsponja/Gary/Gary.obj");
	Karin = Model();
	Karin.LoadModel("Models/DragonBall/Karin/Karin.obj");
	Overgrown = Model();
	Overgrown.LoadModel("Models/OnePunchMan/Overgrown/Overgrown.obj");
}

void RenderEdificios(glm::mat4 model, glm::mat4 modelaux, glm::vec3 color, GLuint uniformModel, GLuint uniformColor) {

	// *********************************************************************
			// Capsule Corp
	// *********************************************************************
	color = glm::vec3(0.5f, 0.5f, 1.0f);
	model = glm::translate(model, glm::vec3(-250.0f, 0.0f, -200.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	Capsule.RenderModel();

	// *********************************************************************
			// Capsule Bob Esponja
	// *********************************************************************

	model = modelaux;
	color = glm::vec3(1.0f, 1.0f, 0.5f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -300.0f));
	model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	CasaBob.RenderModel();

	// *********************************************************************
			// Casa Calamardo
	// *********************************************************************

	model = modelaux;
	color = glm::vec3(0.5f, 0.5f, 1.0f);
	model = glm::translate(model, glm::vec3(150.0f, 0.0f, -300.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	CasaCalamardo.RenderModel();

	// *********************************************************************
			// Kame House
	// *********************************************************************
	
	model = modelaux;
	color = glm::vec3(1.0f, 0.5f, 0.5f);
	model = glm::translate(model, glm::vec3(300.0f, 0.0f, -300.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	KameHouse.RenderModel();
}

void RenderPersonajes(glm::mat4 model, glm::mat4 modelaux, glm::vec3 color, GLuint uniformModel, GLuint uniformColor) {

	// *********************************************************************
			// Maestro Roshi 
	// *********************************************************************

	model = modelaux;
	color = glm::vec3(1.0f, 0.5f, 1.0f);
	model = glm::translate(model, glm::vec3(100.0f, 0.0f, -230.0f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	Roshi.RenderModel();

	// *********************************************************************
			// Bob Esponja
	// *********************************************************************

	model = modelaux;
	color = glm::vec3(1.0f, 0.5f, 1.0f);
	model = glm::translate(model, glm::vec3(20.0f, 0.0f, -250.0f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	Bob.RenderModel();

	// *********************************************************************
			// Calamardo
	// *********************************************************************

	model = modelaux;
	color = glm::vec3(1.0f, 0.5f, 1.0f);
	model = glm::translate(model, glm::vec3(150.0f, 0.0f, -250.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	Calamardo.RenderModel();

	// *********************************************************************
			// Gary
	// *********************************************************************

	model = modelaux;
	color = glm::vec3(1.0f, 0.5f, 1.0f);
	model = glm::translate(model, glm::vec3(200.0f, 0.0f, 50.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	Gary.RenderModel();

	// *********************************************************************
			// Karin
	// *********************************************************************

	model = modelaux;
	color = glm::vec3(1.0f, 0.5f, 1.0f);
	model = glm::translate(model, glm::vec3(50.0f, 0.0f, 70.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	Karin.RenderModel();

	// *********************************************************************
			// Overgrown
	// *********************************************************************

	model = modelaux;
	color = glm::vec3(0.5f, 1.0f, 0.5f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1200.0f, 1200.0f, 1200.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	Overgrown.RenderModel();
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	LoadModels();

	camera = Camera(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 3.0f, 1.0f);

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

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
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(40.0f, 1.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		model = modelaux;

		//*****************************************************************
				// CARGA LOS EDIFICIOS
		//*****************************************************************
		
		RenderEdificios(model, modelaux,color, uniformModel, uniformColor);

		//*****************************************************************
				// CARGA LOS EDIFICIOS
		//*****************************************************************

		RenderPersonajes(model, modelaux, color, uniformModel, uniformColor);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
