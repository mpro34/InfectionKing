#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.hpp"

const char* ENGINE_TITLE = "Dracoa Engine v1.0";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gWindow = nullptr;
bool gWireFrame = false;

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main()
{
    if (!initOpenGL())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    /* begin region - Draw a Triangle */
    // Drawing two triangles using a single vertex buffer
    GLfloat vertices[] = {
        -0.5f,   0.5f,   0.0f,
        0.5f,    0.5f,   0.0f,
        0.5f,   -0.5f,   0.0f,
        -0.5f,  -0.5f,   0.0f
    };

    // Use an index or element buffer to avoid drawing the same vertex more than once!
    GLuint indices[] = {
        0, 1, 2,       // Triangle 0
        0, 2, 3,       // Triangle 1
    };

    // Send vertices to GPU and store in buffer, vao vs vbo?
    GLuint vbo, ibo, vao;
    // Create a chunk(buffer) of memory in the gpu for us
    glGenBuffers(1, &vbo);
    // Make the vbo the current buffer - 1 active at a time
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass vertices to created gpu vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create a vertex array object for faster caching
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Define the attributes of the vertex data (how should the GPU interpret the data?)
    // The "stride" determines where in the data array is this attributes data.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Generate and bind the index/element buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");

    /* end region */

    // Main loop
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);

        glfwPollEvents(); // Query the window for keyboard/mouse events

        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();

        GLfloat time = glfwGetTime();
        GLfloat blueColor = (sin(time) / 2) + 0.5f;
        glm::vec2 pos;
        pos.x = sin(time) / 2;
        pos.y = cos(time) / 2;
        // Must set uniforms AFTER the shader program is in use.
        shaderProgram.setUniform("vertColor", glm::vec4(0.0f, 0.0f, blueColor, 1.0f));
        shaderProgram.setUniform("posOffset", pos);

        glBindVertexArray(vao);
        // Tell opengl to draw triangles from the vao and how many vertices.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(gWindow); // Allows for double buffering, which eliminates screen tearing
    }

    // Clean up shader memory
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);

    glfwTerminate();
    return 0;
}

bool initOpenGL()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    // Set the min OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Force the application to use modern opengl functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window with glfw, fullscreen or windowed
    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, ENGINE_TITLE, nullptr, nullptr);
    if (gWindow == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the created window current to draw to.
    glfwMakeContextCurrent(gWindow);

    glfwSetKeyCallback(gWindow, glfw_onKey);

    glewExperimental = GL_TRUE; // Must set to true in order to use modern opengl
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed" << std::endl;

        return false;
    }

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

    return true;
}

// On a key press, this function is called
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        gWireFrame = !gWireFrame;
        if (gWireFrame)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
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