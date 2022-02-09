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

const char* ENGINE_TITLE = "Dracoa Engine v1.3";
int g_window_width = 1024;
int g_window_height = 768;
GLFWwindow* g_window = nullptr;
bool g_wire_frame = false;
bool g_flash_light_on = true;

FPSCamera g_fps_camera(glm::vec3(0.0f, 3.0f, 10.0f));
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

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
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
    lightingShader.loadShaders("shaders/lighting_spot.vert", "shaders/lighting_spot.frag");

    // Load meshes and textures
    const int numModels = 7;
    Mesh mesh[numModels];
    Texture2D texture[numModels];

    mesh[0].loadOBJ("models/crate.obj");
    mesh[1].loadOBJ("models/woodcrate.obj");
    mesh[2].loadOBJ("models/robot.obj");
    mesh[3].loadOBJ("models/floor.obj");
    mesh[4].loadOBJ("models/bowling_pin.obj");
    mesh[5].loadOBJ("models/bunny.obj");
    mesh[6].loadOBJ("models/lampPost.obj");

    texture[0].loadTexture("textures/crate.jpg", true);
    texture[1].loadTexture("textures/woodcrate_diffuse.jpg", true);
    texture[2].loadTexture("textures/robot_diffuse.jpg", true);
    texture[3].loadTexture("textures/tile_floor.jpg", true);
    texture[4].loadTexture("textures/AMF.tga", true);
    texture[5].loadTexture("textures/bunny_diffuse.jpg", true);
    texture[6].loadTexture("textures/lamp_post_diffuse.png", true);

    // Model positions
    glm::vec3 modelPos[] = {
        glm::vec3(-3.5f, 0.0f, 0.0f),	// crate1
        glm::vec3(3.5f, 0.0f, 0.0f),	// crate2
        glm::vec3(0.0f, 0.0f, -2.0f),	// robot
        glm::vec3(0.0f, 0.0f, 0.0f),	// floor
        glm::vec3(0.0f, 0.0f, 2.0f),	// pin
        glm::vec3(-2.0f, 0.0f, 2.0f),	// bunny
        glm::vec3(-5.0f, 0.0f, 0.0f)	// lamp post
    };

    // Model scale
    glm::vec3 modelScale[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),	// crate1
        glm::vec3(1.0f, 1.0f, 1.0f),	// crate2
        glm::vec3(1.0f, 1.0f, 1.0f),	// robot
        glm::vec3(10.0f, 1.0f, 10.0f),	// floor
        glm::vec3(0.1f, 0.1f, 0.1f),	// pin
        glm::vec3(0.7f, 0.7f, 0.7f),	// bunny
        glm::vec3(1.0f, 1.0f, 1.0f)		// lamp post
    };

    double lastTime = glfwGetTime();
    float angle = 0.0f;

    // Main loop
    while (!glfwWindowShouldClose(g_window))
    {
        showFPS(g_window);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime; // time per frame
        
        glfwPollEvents(); // Query the window for keyboard/mouse events
        update(deltaTime); // Update movement

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0), view(1.0), projection(1.0);
        
        view = g_fps_camera.getViewMatrix();

        projection = glm::perspective(glm::radians(g_fps_camera.getFOV()), (float)g_window_width / (float)g_window_height, 0.1f, 200.0f);    

        glm::vec3 viewPos;
        viewPos.x = g_fps_camera.getPosition().x;
        viewPos.y = g_fps_camera.getPosition().y;
        viewPos.z = g_fps_camera.getPosition().z;

        // the light
        glm::vec3 lightPos = g_fps_camera.getPosition(); // Spot light acts as player flashlight
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        lightPos.y -= 0.5f; // Move light down from camera to give the illusion holding a flashlight

        // Must be called BEFORE setting uniforms because setting uniforms is done on the currently active shader program.
        lightingShader.use();

        // Pass Model, View and Projection matricies to vertex shader
        lightingShader.setUniform("view", view);
        lightingShader.setUniform("projection", projection);
        lightingShader.setUniform("viewPos", viewPos);

        // Spot Light (flashlight)
        lightingShader.setUniform("light.position", lightPos);
        lightingShader.setUniform("light.direction", g_fps_camera.getLook());
        lightingShader.setUniform("light.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setUniform("light.diffuse", lightColor);
        lightingShader.setUniform("light.specular", glm::vec3(0.0f, 0.0f, 1.0f));
        lightingShader.setUniform("light.constant", 0.3f);
        lightingShader.setUniform("light.linear", 0.07f);
        lightingShader.setUniform("light.exponent", 0.017f);
        lightingShader.setUniform("light.cosInnerCone", glm::cos(glm::radians(15.0f)));
        lightingShader.setUniform("light.cosOuterCone", glm::cos(glm::radians(20.0f)));
        lightingShader.setUniform("light.on", g_flash_light_on);

        for (int i = 0; i < numModels; i++)
        {
            model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
            lightingShader.setUniform("model", model);

            lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            lightingShader.setUniformSampler("material.diffuseMap", 0);
            lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            lightingShader.setUniform("material.shininess", 32.0f);

            texture[i].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
            mesh[i].draw();			// Render the OBJ mesh
            texture[i].unbind(0);
        }

        glfwSwapBuffers(g_window); // Allows for double buffering, which eliminates screen tearing
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
    g_window = glfwCreateWindow(g_window_width, g_window_height, ENGINE_TITLE, nullptr, nullptr);
    if (g_window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the created window current to draw to.
    glfwMakeContextCurrent(g_window);

    // Must set to true in order to use modern opengl
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed" << std::endl;
        return false;
    }

    glfwSetKeyCallback(g_window, glfw_onKey);
    glfwSetScrollCallback(g_window, glfw_onMouseScroll);

    // Hides and grabs cursor, unlimited movement
    glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(g_window, g_window_width / 2.0f, g_window_height / 2.0f);

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

    glViewport(0, 0, g_window_width, g_window_height);
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

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        g_wire_frame = !g_wire_frame;
        if (g_wire_frame)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        g_flash_light_on = !g_flash_light_on;
    }
}

//
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height)
{
    g_window_width = width;
    g_window_height = height;
    glViewport(0, 0, width, height);
}

// Called by GLFW when the mouse wheel is rotated
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
    double fov = g_fps_camera.getFOV() + deltaY * ZOOM_SENSITIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    g_fps_camera.setFOV((float)fov);
}

// Update stuff every frame
void update(double elapsedTime)
{
    // Camera orientation
    double mouseX, mouseY;

    // Get the current mouse cursor position delta
    glfwGetCursorPos(g_window, &mouseX, &mouseY);

    // Rotate the camera the difference in mouse distance from the center screen. Multiply this delta by a speed scaler
    g_fps_camera.rotate((float)(g_window_width / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float(g_window_height / 2.0 - mouseY) * MOUSE_SENSITIVITY));

    // Clamp mouse cursor to center of screen
    glfwSetCursorPos(g_window, g_window_width / 2.0, g_window_height / 2.0);

    // Camera FPS Movement

    // Forward/Backward
    if (glfwGetKey(g_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        g_fps_camera.move(MOVE_SPEED * (float)elapsedTime * g_fps_camera.getLook());
    }
    else if (glfwGetKey(g_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        g_fps_camera.move(MOVE_SPEED * (float)elapsedTime * -g_fps_camera.getLook());
    }

    // Strafe Left/Right
    if (glfwGetKey(g_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        g_fps_camera.move(MOVE_SPEED * (float)elapsedTime * g_fps_camera.getRight());
    }
    else if (glfwGetKey(g_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        g_fps_camera.move(MOVE_SPEED * (float)elapsedTime * -g_fps_camera.getRight());
    }

    // Up/Down
    if (glfwGetKey(g_window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        g_fps_camera.move(MOVE_SPEED * (float)elapsedTime * g_fps_camera.getUp());
    }
    else if (glfwGetKey(g_window, GLFW_KEY_X) == GLFW_PRESS)
    {
        g_fps_camera.move(MOVE_SPEED * (float)elapsedTime * -g_fps_camera.getUp());
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