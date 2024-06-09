#ifndef CUBE_H
#define CUBE_H
#include <glm/glm.hpp>
#include "shader.h"
#include "game_object.h"
#include <vector>

class Cube: public GameObject
{
public:

	Shader				shader;
	unsigned int		VAO;
	float				Rotate;
	glm::vec3			lightPos;
	glm::vec3			Scale;
	glm::vec3			Position;
	glm::vec3			RotateDir;
	glm::vec3			Color;
	std::string			Type = "cube";

	Cube(const Shader& shader,
		glm::vec3 scale,
		glm::vec3 position,
		float rotate,
		glm::vec3 rotateDir,
		glm::vec3 color
        );

	virtual void initData();
	virtual void setLightPos(glm::vec3 lightPos);
	virtual void render
	(
		glm::mat4 projection, 
		glm::mat4 view, 
		glm::vec3 cameraPos,
		glm::vec3 lightPos
	);
};


#endif // !CUBE_H
