#ifndef PLAYER_H
#define PLAYER_H

#include "model.h"

class Player
{
public:

	Model model;

	Player(Model& player);

	virtual void handleInput();
};

#endif PLAYER_H