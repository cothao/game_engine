#include "player.h"

Player::Player()
    :Model()
{

}

Player::Player(string const& path, bool flip, glm::vec3 pos, glm::vec3 scale, bool gamma)
    :
    Model(path, flip, pos, scale, gamma)
{
    loadModel(path);
    std::cout << this->ID << "\n";
};

void Player::handleInput(float DeltaTime)
{
    if (this->Keys[GLFW_KEY_W])
    {
        glm::vec3 direction = glm::vec3(cos(this->RotateY), sin(this->RotateY), 0.f);
        glm::vec3 right = glm::vec3(1, 0., 0.);
        glm::vec3 z = glm::vec3(0., 1., 0.);
        float xDot = glm::dot(direction, right);
        float zDot = glm::dot(direction, z);

        this->Position.x += (zDot * 2.0) * DeltaTime;
        this->Position.z += (xDot * 2.0) * DeltaTime;
    }
    if (this->Keys[GLFW_KEY_A])
    {
        this->RotateY += 3.0f * DeltaTime;
    }
    if (this->Keys[GLFW_KEY_D])
    {
        this->RotateY -= 3.0f * DeltaTime;

    }
}