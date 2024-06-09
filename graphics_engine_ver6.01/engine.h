#ifndef ENGINE_H
#define ENGINE_H

#include "camera.h"
#include <vector>
#include "light.h"
#include "cube.h"
#include "plane.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"

enum ENGINE_STATE
{
	VIEW,
	EDIT
};

class Engine
{
public:
	ENGINE_STATE		State{ EDIT };
	std::vector<Light>	lights;
	std::vector<Cube>	cubes;
	std::vector<Plane>	planes;
	Camera				camera		=	Camera(glm::vec3(10.0f, 3.0f, 1.0f));
	glm::mat4			view		=	this->camera.GetViewMatrix();
	const unsigned int	SCR_WIDTH	=	1600;
	const unsigned int	SCR_HEIGHT	=	1200;
	glm::mat4			projection	=	glm::perspective(glm::radians(this->camera.Zoom), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT, 0.1f, 100.f);
	GLFWwindow*			window;


	Engine();

	virtual int			initEngine();
	virtual void		initObjects();
	virtual void		render();

};

#endif