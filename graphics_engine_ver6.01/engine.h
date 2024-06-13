#ifndef ENGINE_H
#define ENGINE_H

#include "camera.h"
#include <vector>
#include <map>
#include "light.h"
#include "cube.h"
#include "plane.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "shader.h"
#include "model.h"
#define WIN32_LEAN_AND_MEAN
#define NO_MIN_MAX
#define NOMINMAX
#include "Windows.h"

enum ENGINE_STATE
{
	VIEW,
	EDIT
};

class Engine
{
public:
	ENGINE_STATE					State{ EDIT };
	std::vector<Light>				lights;
	std::vector<Cube>				cubes;
	std::vector<Plane>				planes;
	std::vector<Model>				models;
	std::vector<std::string>		logs;
	Camera							camera		=	Camera(glm::vec3(10.0f, 3.0f, 1.0f));
	glm::mat4						view		=	this->camera.GetViewMatrix();
	const unsigned int				SCR_WIDTH	=	GetSystemMetrics(SM_CXSCREEN);
	const unsigned int				SCR_HEIGHT	=	GetSystemMetrics(SM_CYSCREEN);
	glm::mat4						projection	=	glm::perspective(glm::radians(this->camera.Zoom), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT, 0.1f, 100.f);
	GLFWwindow*						window;
	std::map<std::string, Shader>	shaderDirectory;

	Engine();

	virtual int			initEngine();
	virtual void		initObjects();
	virtual void		render();

};

#endif