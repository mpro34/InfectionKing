#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char* ENGINE_TITLE = "Dracoa Engine v1.0";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gWindow = nullptr;

// Triangle Vertex Shader (XYZW)
const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"void main()"
"{"
"    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

// Triangle Fragment Shader (RGBA)
const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 color;"
"void main()"
"{"
"    color = vec4(0.35f, 0.96f, 0.3f, 1.0f);"
"}";


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
    // Each vertiex defined in 3d space: x, y, z
    GLfloat vertices[] = {
        0.0f, 0.5f, 0.0f, // Top
        0.5f, -0.5f, 0.0f, // Right
        -0.5f, -0.5f, 0.0f, // Left
    };

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    GLint result;
    GLchar infoLog[512];
    // Create, assign, and compile the vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vs, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
    }

    // Create, assign, and compile the fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fs, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
    }

    // Create the shader program with all compiled shaders above (vertex and fragment)
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Error! Shader program linker failure. " << infoLog << std::endl;
    }

    // Once shader program is created, we can free the shader memory before entering the game loop.
    glDeleteShader(vs);
    glDeleteShader(fs);

    /* end region */

    // Main loop
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);

        glfwPollEvents(); // Query the window for keyboard/mouse events

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        // Tell opengl to draw triangles from the vao and how many vertices.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(gWindow); // Allows for double buffering, which eliminates screen tearing
    }

    // Clean up shader memory
    glDeleteProgram(shaderProgram);
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

    return true;
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