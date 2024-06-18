#ifndef PLAYER_H
#define PLAYER_H

#include "model.h"
#include <GLFW/glfw3.h>

class Player : virtual public Model
{
public:

	bool Keys[1024];

	Player(string const& path, bool flip = true, glm::vec3 pos = glm::vec3(1.0), glm::vec3 scale = glm::vec3(1.0), bool gamma = false);

	Player();

	virtual void handleInput(float DeltaTime);
};

#endif PLAYER_H