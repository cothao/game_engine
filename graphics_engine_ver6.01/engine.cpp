#include "engine.h"

Engine::Engine()
{

}

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

    glEnable(GL_DEPTH_TEST);
}

void Engine::initObjects()
{
    this->shaderDirectory["pShader"] = Shader("./pShader.vs", "./pShader.fs");
    this->shaderDirectory["lShader"] = Shader("./lightShader.vs", "./lightShader.fs");
    this->shaderDirectory["mShader"] = Shader("./modelShader.vs", "./modelShader.fs");

    pObject = *new Player("./assets/guy/guy.obj", false, glm::vec3(1.0), glm::vec3(0.005));

    this->lights = { *new Light(this->shaderDirectory["lShader"], glm::vec3(6., 2., 3.))};
    this->models = {
        //*new Model("./assets/obj/Wolf_obj.obj", false,glm::vec3(1.0),glm::vec3(0.5)),
        //* new Model("C:/Users/colli/OneDrive/Documents/resources/backpack.obj", true,glm::vec3(1.0),glm::vec3(0.5)),
        //*new Model("C:/Users/colli/OneDrive/Documents/resources/donut/donut.obj", true,glm::vec3(1.0, 1.0, 5.0),glm::vec3(10.0)),

    };
    this->planes = { *new Plane(this->shaderDirectory["pShader"])};
    
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
        }

        for (Cube &cube : this->cubes)
        {
            cube.render(this->projection, this->view, this->camera.Position, this->lights[0].pos);
        }

        for (Model& model: this->models)
        {   
            glm::vec3 target = glm::vec3(0.);
            glm::vec3 modelDir = glm::normalize(model.Position - target);
            model.Right = glm::normalize(glm::cross(model.Up, modelDir));
            model.Up = glm::normalize(glm::cross(modelDir, model.Right));

            const float radius = 10.0f;
            float camX = sin(glfwGetTime()) * radius;
            float camZ = cos(glfwGetTime()) * radius;
            glm::mat4 view;
            view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            model.Draw(this->shaderDirectory["mShader"], this->projection,this->view, this->camera.Position);
            logs.push_back("Forward Vector: " + model.getPosition());
        }

        for (Plane plane : this->planes)
        {
            plane.render(this->projection, this->view, glm::vec3(10.0), glm::vec3(1., -5., 1.), 90.0, glm::vec3(1., 0., 0.), glm::vec3(0.8, 0.8, 0.8), this->lights[0].pos, this->camera.Position);
        }
        
        pObject.Draw(this->shaderDirectory["mShader"], this->projection, this->view, this->camera.Position);

    }

}

void Engine::processInputs(float DeltaTime)
{
    this->pObject.handleInput(DeltaTime);
}