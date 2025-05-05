#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Scene.h"
#include "PLYReader.h"
#include "Application.h"


Scene::Scene()
{
	cube = NULL;
	mesh = NULL;
}

Scene::~Scene()
{
	if(cube != NULL)
		delete cube;
	if(mesh != NULL)
		delete mesh;
	if(object != NULL)
		delete object;
	for(vector<TriangleMeshInstance *>::iterator it=walls.begin(); it!=walls.end(); it++)
		delete *it;
}


// Initialize the scene. This includes the cube we will use to render
// the floor and walls, as well as the camera.

void Scene::init()
{
	cube = new TriangleMesh();
	cube->buildCube();
	cube->sendToOpenGL();
	currentTime = 0.0f;
	initRoom();
	
	camera.init(glm::vec3(0.f, 0.75f, 2.f));
}

// Load the map & all its associated models

bool Scene::loadMap(const string &filename)
{
	ifstream fin;
	string model_filename;
	
	fin.open(filename);
	if(!fin.is_open())
		return false;
	fin >> model_filename;
	fin.close();
	
	return loadMesh(model_filename);
}

// Loads the mesh into CPU memory and sends it to GPU memory (using GL)

bool Scene::loadMesh(const string &filename)
{
#pragma warning( push )
#pragma warning( disable : 4101)
	PLYReader reader;
#pragma warning( pop ) 

	if(mesh != NULL)
	{
		mesh->free();
		delete mesh;
	}
	mesh = new TriangleMesh();
	bool bSuccess = reader.readMesh(filename, *mesh);
	if(bSuccess)
	{
		object = new TriangleMeshInstance();
		object->init(mesh, glm::vec4(1.0f), glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		mesh->sendToOpenGL();
	}
	
	return bSuccess;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

// Render the scene. First the room, then the mesh it there is one loaded.

void Scene::render()
{
	camera.render();
 	renderRoom();
 	object->render();
}

VectorCamera &Scene::getCamera()
{
	return camera;
}

// Init & render the room. Both the floor and the walls are instances of the
// same initial cube scaled and translated to build the room.

void Scene::initRoom()
{
	glm::mat4 transform;
	TriangleMeshInstance *instance;

	// Build model transform matrix, create instance from cube, and add it to walls vector	
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, -0.5f, 0.f));
	transform = glm::scale(transform, glm::vec3(20.f, 1.f, 20.f));
	instance = new TriangleMeshInstance();
	instance->init(cube, glm::vec4(1.0f), transform);
	walls.push_back(instance);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 1.f, -9.5f));
	transform = glm::scale(transform, glm::vec3(20.f, 2.f, 1.f));
	instance = new TriangleMeshInstance();
	instance->init(cube, glm::vec4(1.0f), transform);
	walls.push_back(instance);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 1.f, 9.5f));
	transform = glm::scale(transform, glm::vec3(20.f, 2.f, 1.f));
	instance = new TriangleMeshInstance();
	instance->init(cube, glm::vec4(1.0f), transform);
	walls.push_back(instance);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-9.5f, 1.f, 0.f));
	transform = glm::scale(transform, glm::vec3(1.f, 2.f, 20.f));
	instance = new TriangleMeshInstance();
	instance->init(cube, glm::vec4(1.0f), transform);
	walls.push_back(instance);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(9.5f, 1.f, 0.f));
	transform = glm::scale(transform, glm::vec3(1.f, 2.f, 20.f));
	instance = new TriangleMeshInstance();
	instance->init(cube, glm::vec4(1.0f), transform);
	walls.push_back(instance);
}

void Scene::renderRoom()
{
	for(vector<TriangleMeshInstance *>::iterator it=walls.begin(); it!=walls.end(); it++)
		(*it)->render();
}




