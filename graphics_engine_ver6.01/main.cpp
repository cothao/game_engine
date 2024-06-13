#include "engine.h"
#include "gui.h"
#include <commdlg.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
bool editMode = false;
float deltaTime = 0.0f;
float lastFrame = 0.0f; 
float offset = 0.5f;
float alphaValue = 0.2f;
float xf = 1.0f;
float lastX = 1600, lastY = 600;
float yaw = -90.0f;
float pitch = -45.0f;
int k = 0;
bool firstMouse = true;
Engine engine = Engine();
Gui gui = Gui();

int main() {

    engine.initEngine();
    engine.initObjects();

    glfwSetFramebufferSizeCallback(engine.window, framebuffer_size_callback);

    glfwSetInputMode(engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(engine.window, mouse_callback);

    glfwSetScrollCallback(engine.window, scroll_callback);

    gui.initGui(engine);

    while (!glfwWindowShouldClose(engine.window))
    {


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        processInput(engine.window);
        
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gui.newGuiFrame();

        engine.render();

        gui.displayGui(engine, editMode);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(engine.window);
        glfwPollEvents();

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

	return 0;

}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (!editMode)
        {
            engine.camera.ProcessKeyboard(FORWARD, deltaTime);
        }


        // for now, movement will have to be done in the main file

        glm::vec3 direction = glm::vec3(cos(engine.models[0].RotateY), sin(engine.models[0].RotateY), 0.f);
        glm::vec3 right = glm::vec3(1, 0., 0.);
        glm::vec3 z = glm::vec3(0., 1., 0.);
        float xDot = glm::dot(direction, right);
        float zDot = glm::dot(direction, z);

        engine.models[0].Position.x += (zDot * 2.0) * deltaTime;
        engine.models[0].Position.z += (xDot * 2.0) * deltaTime;

        // WILL COME BACK TO THIS. SEE MODEL.CPP
        //engine.models[0].Position.x += (engine.models[0].zDot * 2.0) * deltaTime;
        //engine.models[0].Position.x += (engine.models[0].xDot * 2.0) * deltaTime;


    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        if (!editMode)
        {

        engine.camera.ProcessKeyboard(BACKWARD, deltaTime);
        }

    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (!editMode)
        {

        engine.camera.ProcessKeyboard(LEFT, deltaTime);
        }
        engine.models[0].RotateY += 3.0f * deltaTime;


    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (!editMode)
        {
        engine.camera.ProcessKeyboard(RIGHT, deltaTime);

        }
        engine.models[0].RotateY -= 3.0f * deltaTime;


    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)  {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        editMode = true;

    }
        //glfwSetCursorPosCallback(window, NULL);
        //std::cout << editMode << "\n";

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        editMode = false;
    }

        
        
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    if (!editMode)
    {
    engine.camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!editMode)
    {
        engine.camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

void processMousInput()
{

}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
