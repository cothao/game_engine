#include "engine.h"


Engine::Engine()
{

}
float r = 0.0;
int Engine::initEngine()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    this->window = glfwCreateWindow(this->SCR_WIDTH, this->SCR_HEIGHT, "Graphics Engine", NULL, NULL);

    if (this->window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

}

void Engine::initObjects()
{

    Shader pShader("./pShader.vs", "./pShader.fs");
    Shader lShader("./lightShader.vs", "./lightShader.fs");

    this->lights = { *new Light(lShader, glm::vec3(6., 2., 3.)) };

    this->planes = { *new Plane(pShader) };
}

void Engine::render()
{

    if (this->State == EDIT)
    {
        this->view = this->camera.GetViewMatrix();
        this->projection = glm::perspective(glm::radians(this->camera.Zoom), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT, 0.1f, 100.f);

        for (Light &light : this->lights)
        {
            light.render(this->projection, this->view, glm::vec3(0.3), glm::vec3(6., 0.8, sin(glfwGetTime())), 0.0, glm::vec3(1., 0., 0.), glm::vec3(1., 1., 1.));
            //this->lights[0].pos = glm::vec3(6., 0.8, 1.);
        }
        int i = 1;

        for (Cube &cube : this->cubes)
        {
            cube.render(this->projection, this->view, this->camera.Position, this->lights[0].pos);
            std::cout << "Cube" << " " << i << ":" << cube.Position.x << "\n";
            i++;
        }

        for (Plane plane : this->planes)
        {
            plane.render(this->projection, this->view, glm::vec3(10.0), glm::vec3(1., -5., 1.), 90.0, glm::vec3(1., 0., 0.), glm::vec3(0.8, 0.8, 0.8), this->lights[0].pos, this->camera.Position);
        }
    }
}