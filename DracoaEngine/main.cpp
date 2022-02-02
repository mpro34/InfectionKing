#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char* ENGINE_TITLE = "Dracoa Engine v1.0";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool gFullscreen = false;

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);

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

    // Create a window with glfw, fullscreen or windowed
    GLFWwindow* pWindow = nullptr;
    if (gFullscreen)
    {
        GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
        if (pVmode != nullptr)
        {
            pWindow = glfwCreateWindow(pVmode->width, pVmode->height, ENGINE_TITLE, pMonitor, nullptr);
        }
    }
    else
    {
        pWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, ENGINE_TITLE, nullptr, nullptr);
    }

    if (pWindow == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the created window current to draw to.
    glfwMakeContextCurrent(pWindow);

    glfwSetKeyCallback(pWindow, glfw_onKey);

    glewExperimental = GL_TRUE; // Must set to true in order to use modern opengl
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed" << std::endl;

        return -1;
    }

    while (!glfwWindowShouldClose(pWindow))
    {
        showFPS(pWindow);
        glfwPollEvents(); // Query the window for keyboard/mouse events

        glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(pWindow); // Allows for double buffering, which eliminates screen tearing
    }

    glfwTerminate();
    return 0;
}

// On a key press, this function is called
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// 
void showFPS(GLFWwindow* window)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); // returns the number of seconds since glfw started, as a double.

    elapsedSeconds = currentSeconds - previousSeconds;

    // limit text update 4 times per second
    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        // Construct the window title with FPS counts
        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
            << ENGINE_TITLE << "  "
            << "FPS: " << fps << "  "
            << "Frame Time: " << msPerFrame << " (ms)";

        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }
    frameCount++;
}