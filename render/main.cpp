#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Windows.h>
#include <thread>
#include "Renderer.h"
#include "test.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int fps = 0;

Renderer renderer(SCR_WIDTH, SCR_HEIGHT);

void ShowFps(GLFWwindow* window) {
    while (1) {
        Sleep(1500);
        std::string text = "MyRender fps:" + std::to_string(fps);
        glfwSetWindowTitle(window, text.c_str());
        fps = 0;
    }
}

int main()
{
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyShader", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Renderer
    renderer.init();
    
    Mesh box = createBox(glm::vec3(0.0f, 0.0f, 0.0f), 0.5);

    renderer.setViewMatrix(getViewMatrix(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)));
    renderer.setProjectMatrix(getPerspectiveMatrix(glm::radians(60.0f), float(SCR_WIDTH) / SCR_HEIGHT, 0.3f, 100.0f));

    std::thread t(ShowFps, window);
    t.detach();

    float angle = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        renderer.fillColorBuffer(glm::vec4(30, 80, 90, 255));

        renderer.setModelMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, -1.0f, 0.0f)));
        renderer.drawMesh(box);
        
        renderer.display();
        angle += 1.0f;
        fps++;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    renderer.resize(width, height);
    glViewport(0, 0, width, height);
}