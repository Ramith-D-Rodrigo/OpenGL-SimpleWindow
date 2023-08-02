// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//window size
const GLint WIDTH = 800, HEIGHT = 600;

using namespace std;

int main()
{
    //initialize GLFW
    if (!glfwInit()) {
        cout << "GLFW initialization failed!" << endl;
        glfwTerminate();
        return 1;
    }

    //setup GLFW window properties 
    //OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //core profile = no backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //need to be forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "First Window", NULL, NULL);
    if (!mainWindow) {
        cout << "GLFW window creation failed!" << endl;
        glfwTerminate();
        return 1;
    }

    //get buffer size info
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    //allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialization failed!" << endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //setup view port
    glViewport(0, 0, bufferWidth, bufferHeight);

    //loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) {
        //get and handle user input events
        glfwPollEvents();

        //clear the window
        glClearColor(0.3f, 0.2f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }
    return 0;
}
