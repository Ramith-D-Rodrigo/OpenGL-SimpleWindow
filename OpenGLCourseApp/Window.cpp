#include "Window.h"


Window::Window() {
	this -> width = 800;
    this -> height = 600;

    this -> xChange = 0.0f;
    this -> yChange = 0.0f;

    this->bufferHeight = 0.0f;
    this->bufferWidth = 0.0f;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = false;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	this->width = windowWidth;
	this->height = windowHeight;

    this -> xChange = 0.0f;
    this -> yChange = 0.0f;

    this->bufferHeight = 0.0f;
    this->bufferWidth = 0.0f;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = false;
    }
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

    //handle keys + mouse input
    this -> createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //disable cursor inside the window

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

    glfwSetWindowUserPointer(mainWindow, this); //"this" is the owner of the mainWindow
}

void Window::createCallbacks() {
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange() {
    GLfloat theChange = this -> xChange;
    this -> xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange() {
    GLfloat theChange = this -> yChange;
    this -> yChange = 0.0f;
    return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {   //esc key is pressed
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) { //a valid key (should fit to the array)
        if (action == GLFW_PRESS) {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos; //avoid inverted movement

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}