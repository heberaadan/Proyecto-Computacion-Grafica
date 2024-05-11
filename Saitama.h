#pragma once
#include<stdio.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include<glew.h>
#include<glfw3.h>

class Saitama{
public:
	Saitama(float x, float y, float z);

private:
	float posx, posy, posz;
	glm::vec3 poSaitama;
};