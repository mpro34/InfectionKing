#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"

const char* ENGINE_TITLE = "Dracoa Engine v1.2";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow* gWindow = nullptr;
bool gWireFrame = false;

FPSCamera fpsCamera(glm::vec3(0.0f, 3.0f, 10.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0f; // units per second
const float MOUSE_SENSITIVITY = 0.1f;

// Function Prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main()
{
    if (!initOpenGL())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    ShaderProgram lightShader;
    lightShader.loadShaders("shaders/basic.vert", "shaders/basic.frag");

    ShaderProgram lightingShader;
    lightingShader.loadShaders("shaders/lighting.vert", "shaders/lighting.frag");

    // Load meshes and textures
    const int numModels = 6;
    Mesh mesh[numModels];
    Texture2D texture[numModels];

    mesh[0].loadOBJ("models/crate.obj");
    mesh[1].loadOBJ("models/woodcrate.obj");
    mesh[2].loadOBJ("models/robot.obj");
    mesh[3].loadOBJ("models/floor.obj");
    mesh[4].loadOBJ("models/bowling_pin.obj");
    mesh[5].loadOBJ("models/bunny.obj");

    texture[0].loadTexture("textures/crate.jpg", true);
    texture[1].loadTexture("textures/woodcrate_diffuse.jpg", true);
    texture[2].loadTexture("textures/robot_diffuse.jpg", true);
    texture[3].loadTexture("textures/tile_floor.jpg", true);
    texture[4].loadTexture("textures/AMF.tga", true);
    texture[5].loadTexture("textures/bunny_diffuse.jpg", true);

    // Model Positions
    glm::vec3 modelPos[] = {
        glm::vec3(-3.5f, 0.0f, -2.0f),	// crate1
        glm::vec3(3.5f, 0.0f, -2.0f),	// crate2
        glm::vec3(0.0f, 0.0f, -2.0f),	// robot
        glm::vec3(0.0f, 0.0f, 0.0f),	// floor
        glm::vec3(0.0f, 0.0f, 2.0f),    // pin
        glm::vec3(-2.0f, 0.0f, 2.0f)    // bunny
    };

    // Model scale
    glm::vec3 modelScale[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),	// crate1
        glm::vec3(1.0f, 1.0f, 1.0f),	// crate2
        glm::vec3(1.0f, 1.0f, 1.0f),	// robot
        glm::vec3(10.0f, 1.0f, 10.0f),	// floor
        glm::vec3(0.1f, 0.1f, 0.1f),    // pin
        glm::vec3(0.7f, 0.7f, 0.7f)     // bunny
    };

    Mesh lightMesh;
    lightMesh.loadOBJ("models/light.obj");

    double lastTime = glfwGetTime();
    float angle = 0.0f;

    // Main loop
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime; // time per frame
        
        glfwPollEvents(); // Query the window for keyboard/mouse events
        update(deltaTime); // Update movement

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0), view(1.0), projection(1.0);
        
        view = fpsCamera.getViewMatrix();

        projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);    

        glm::vec3 viewPos;
        viewPos.x = fpsCamera.getPosition().x;
        viewPos.y = fpsCamera.getPosition().y;
        viewPos.z = fpsCamera.getPosition().z;

        // the light
        glm::vec3 lightPos(0.0f, 1.0f, 10.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

        // move the light
        angle += (float)deltaTime * 50.0;
        lightPos.x += 8.0f * sinf(glm::radians((double)angle));


        lightingShader.use();

        // Pass Model, View and Projection matricies to vertex shader
        lightingShader.setUniform("view", view);
        lightingShader.setUniform("projection", projection);
        lightingShader.setUniform("lightColor", lightColor);
        lightingShader.setUniform("lightPos", lightPos);
        lightingShader.setUniform("viewPos", viewPos);

        for (int i = 0; i < numModels; i++)
        {
            model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
            lightingShader.setUniform("model", model);

            texture[i].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
            mesh[i].draw();			// Render the OBJ mesh
            texture[i].unbind(0);
        }

        // render the light
        model = glm::translate(glm::mat4(1.0), lightPos);
        lightShader.use();
        lightShader.setUniform("lightColor", lightColor);
        lightShader.setUniform("model", model);
        lightShader.setUniform("view", view);
        lightShader.setUniform("projection", projection);
        lightMesh.draw();

        glfwSwapBuffers(gWindow); // Allows for double buffering, which eliminates screen tearing
        lastTime = currentTime;
    }

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

    // Must set to true in order to use modern opengl
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed" << std::endl;
        return false;
    }

    glfwSetKeyCallback(gWindow, glfw_onKey);
    glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

    // Hides and grabs cursor, unlimited movement
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0f, gWindowHeight / 2.0f);

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

// Called by GLFW when the mouse wheel is rotated
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
    double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    fpsCamera.setFOV((float)fov);
}

// Update stuff every frame
void update(double elapsedTime)
{
    // Camera orientation
    double mouseX, mouseY;

    // Get the current mouse cursor position delta
    glfwGetCursorPos(gWindow, &mouseX, &mouseY);

    // Rotate the camera the difference in mouse distance from the center screen. Multiply this delta by a speed scaler
    fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY));

    // Clamp mouse cursor to center of screen
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    // Camera FPS Movement

    // Forward/Backward
    if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
    }
    else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());
    }

    // Strafe Left/Right
    if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
    }
    else if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
    }

    // Up/Down
    if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
    {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
    }
    else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
    {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
    }
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