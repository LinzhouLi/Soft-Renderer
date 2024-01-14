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
#include "Texture.h"
#include "Camera.h"
#include "Light.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int fps = 0;

Scene scene(SCR_WIDTH, SCR_HEIGHT);
Camera camera(glm::vec3(-3.0f, 3.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f,  0.1f, 100.0f);

void ShowFps(GLFWwindow* window) {
    while (1) {
        Sleep(1000);
        std::string text = "MyRender fps:" + std::to_string(fps);
        glfwSetWindowTitle(window, text.c_str());
        fps = 0;
    }
}

#include <math.h>
int main()
{
    
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyShader", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Renderer
    scene.init();
    scene.add(&camera);
    DirectionLight dirctionLight1 = DirectionLight(glm::vec3(-1, -1, -1), glm::vec3(255, 255, 255));
    DirectionLight dirctionLight2 = DirectionLight(glm::vec3(1, 1, 1), glm::vec3(255, 255, 255));
    scene.add(&dirctionLight1);
    scene.add(&dirctionLight2);
    scene.setBackgroundColor(glm::vec4(30, 80, 90, 255));

    //Mesh box1 = createBox(glm::vec3(0.0f, 0.0f, 0.0f), 0.5);
    //Mesh box2 = createBox(glm::vec3(2.0f, 0.0f, 0.0f), 0.3);
    //Shader shader;
    //Texture texture1("assets/container.jpg");
    //Texture texture2("assets/2.png");
    //Material material1(&shader, &texture1);
    //Material material2(&shader, &texture2);
    //Object obj1(&box1, &material1);
    //Object obj2(&box2, &material2);
    //scene.add(&obj1);
    //scene.add(&obj2);


    Shader shader;
    Mesh mesh;
    mesh.loadObj("./assets/monkey.obj");
    Texture texture("assets/2.png");
    Material material(&shader, &texture);
    Object obj(&mesh, &material);
    scene.add(&obj);


    std::thread t(ShowFps, window);
    t.detach();

    float angle = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);
        
        //obj.transform(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0, 1.0, 0.0)));
        scene.render();
        Sleep(10);
        angle += 1.0f;
        fps++;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.rotatePitch(1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera.rotateYaw(-1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.rotateYaw(1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.rotatePitch(-1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveForward(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveBackward(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveLeft(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveRight(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.moveUp(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.moveDown(0.1);
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    scene.resize(width, height);
    glViewport(0, 0, width, height);
}