#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Windows.h>
#include <thread>

#include "test.h"
#include "Scene.h"
#include "Material.h"
#include "Object.h"
#include "Shader.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int fps = 0;

Scene scene(SCR_WIDTH, SCR_HEIGHT);

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
    scene.init();
    scene.setBackgroundColor(glm::vec4(30, 80, 90, 255));

    Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.3f, 100.0f);
    scene.add(&camera);

    Mesh box = createBox(glm::vec3(0.0f, 0.0f, 0.0f), 0.5);
    Shader shader;
    Material material(&shader);
    Object obj(&box, &material);
    scene.add(&obj);

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
        
        obj.transform(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0, 1.0, 0.0)));
        scene.render();

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
    scene.resize(width, height);
    glViewport(0, 0, width, height);
}