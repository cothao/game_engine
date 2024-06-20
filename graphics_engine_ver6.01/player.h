#ifndef PLAYER_H
#define PLAYER_H

#include "model.h"
#include "cube.h"
#include <GLFW/glfw3.h>

class Player : virtual public Model
{
public:

	bool Keys[1024];
	bool IsCollision = false;

	Player(string const& path, bool flip = true, glm::vec3 pos = glm::vec3(1.0), glm::vec3 scale = glm::vec3(1.0), bool gamma = false);

	Player();

	virtual void handleInput(float DeltaTime, bool& IsEditing);
	virtual bool handleCollision(Cube cube);
};

#endif PLAYER_H