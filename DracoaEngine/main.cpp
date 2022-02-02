#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char* ENGINE_TITLE = "Dracoa Engine v1.0";
const int gWindowWidth = 800;
const int gWindowHeight = 600;

int main()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    // Set the min OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Force the application to use modern opengl functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window with glfw
    GLFWwindow* pWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, ENGINE_TITLE, nullptr, nullptr);
    if (pWindow == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the created window current to draw to.
    glfwMakeContextCurrent(pWindow);

    glewExperimental = GL_TRUE; // Must set to true in order to use modern opengl
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed" << std::endl;

        return -1;
    }

    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents(); // Query the window for keyboard/mouse events


    }

    glfwTerminate();
    return 0;
}