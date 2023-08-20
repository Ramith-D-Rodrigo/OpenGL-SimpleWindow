#include "Window.h"


Window::Window() {
	width = 800;
	height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	this->width = windowWidth;
	this->height = windowHeight;
}

int Window::Initialize() {
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


    mainWindow = glfwCreateWindow(this -> width, this -> height, "First Window", NULL, NULL);
    if (!mainWindow) {
        cout << "GLFW window creation failed!" << endl;
        glfwTerminate();
        return 1;
    }

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

    glEnable(GL_DEPTH_TEST); //enable depth to check which traingle has more depth and which should be at the front

    //setup view port
    glViewport(0, 0, bufferWidth, bufferHeight);
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}