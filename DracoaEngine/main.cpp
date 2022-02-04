#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

const char* ENGINE_TITLE = "Dracoa Engine v1.0";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow* gWindow = nullptr;
bool gWireFrame = false;
const std::string texture1 = "textures/wooden_crate.jpg";
//const std::string texture2 = "textures/crate.jpg";

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
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
        // position             // texture uv coords
        
        // front face
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

         // back face
         -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
          1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
          1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
         -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
          1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

          // left face
          -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
          -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
          -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
          -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
          -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
          -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

          // right face
           1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
           1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
           1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
           1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
           1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
           1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

           // top face
          -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
           1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
           1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
          -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
          -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
           1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

           // bottom face
          -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
           1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
          -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
          -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
           1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
    };

    glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -5.0f);

    // Send vertices to GPU and store in buffer, vao vs vbo?
    GLuint vbo, vao;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    // Texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");

    Texture2D plane_texture;
    plane_texture.loadTexture(texture1, true);

    //Texture2D crate_texture;
    //crate_texture.loadTexture(texture2, true);

    /* end region */

    float cubeAngle = 0.0f;
    double lastTime = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime; // time per frame
        
        glfwPollEvents(); // Query the window for keyboard/mouse events

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        plane_texture.bind(0);
        //crate_texture.bind(1);

        cubeAngle += (float)(deltaTime * 50.0f);
        if (cubeAngle >= 360)
        {
            cubeAngle = 0;
        }
        glm::mat4 model(1.0), view(1.0), projection(1.0);
        // Translate model back by cubePos and rotate it by cubeAngle, every frame. Pass it to vertex shader. Rotate first, then translate (evaluated Right -> Left)
        model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0));
        
        glm::vec3 camPos(0.0f, 0.0f, 0.0f);
        glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        view = glm::lookAt(camPos, targetPos, up);

        projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);    

        shaderProgram.use();

        // Pass Model, View and Projection matricies to vertex shader
        shaderProgram.setUniform("model", model);
        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);

        glBindVertexArray(vao);
        // Tell opengl to draw triangles from the vao and how many vertices.
        glDrawArrays(GL_TRIANGLES, 6, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(gWindow); // Allows for double buffering, which eliminates screen tearing
        lastTime = currentTime;
    }

    // Clean up shader memory
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

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
    glViewport(0, 0, gWindowWidth, gWindowHeight);
    //Enable the depth buffer to show depth in 3D
    glEnable(GL_DEPTH_TEST);

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
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height)
{
    gWindowWidth = width;
    gWindowHeight = height;
    glViewport(0, 0, width, height);
}

// Show the FPS and other stats for the current window
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