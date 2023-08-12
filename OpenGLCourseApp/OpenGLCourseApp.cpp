// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//use the following in the newer glm versions
//glm::mat4 model(1.0f);
//glm::mat4 model = glm::mat4(1.0f);
//model = glm::mat(1.0f);

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection; //uniform model now (IBO - index buffer object)
const float toRadians = 3.14159265f / 180.0f;

bool direction = true;  //right = true, left = false
float triOffset = 0.0f;
float triMaxOffset = 0.8f;  //to switch the direction
float triInc = 0.0005f; //speed to move

float currAngle = 0.0f;

float currSize = 0.5f;
float minSize = 0.1f;
float maxSize = 0.8f;
bool sizeDirection = true;


//window size
const GLint WIDTH = 800, HEIGHT = 600;

using namespace std;

//creating the vertex shader
static const char* vShader = R"(
#version 330                                                                
                                                                            
layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;    //how the camera translates what can be seen the screen  

out vec4 vCol; //vertex color                                        
                                                                            
void main()                                                                 
{                                                                          
    gl_Position = projection * model * vec4(pos, 1.0); //we can pass pos vector directly, no need to access each axis
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);  
    //make the color to the position values (using clamp to deal with the negative values
    //set between 0 and 1)
}
)";


//fragment shader
static const char* fShader = R"(
#version 330                 

in vec4 vCol;   //get it from the vShader                                               
                                                                            
out vec4 colour;                                                            
                                                                            
void main()                                                                 
{                                                                           
    colour = vCol;                                      
}
)";

void CreateTriangle() {

    //specify the order
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0, //front
        0, 1, 2 //base
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, //bottom left
        0.0f, -1.0f, 1.0f,  //background
        1.0f, -1.0f, 0.0f,  //bottm right
        0.0f, 1.0f, 0.0f    //top center
    };

    //by default opengl has origin on the center of the window and 1 and -1 range on both axes

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //static mean we are not changing the values of the array

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);   //unbinding the previous one
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbind after the VAO
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {

    GLuint theShader = glCreateShader(shaderType);

    const GLchar* const* theCode = &shaderCode;
    GLint codeLength = static_cast<GLint>(strlen(shaderCode));


    glShaderSource(theShader, 1, theCode, &codeLength);
    glCompileShader(theShader);

    GLint result = 0;

    GLchar eLog[1024] = { 0 }; //to log the error if any

    //debugging

    glGetShaderiv(theShader, GL_COMPILE_STATUS, & result);

    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader! %s\n", shaderType,  eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}


void CompileShaders() {
    shader = glCreateProgram();
    if (!shader) {
        cout << "Error creating the shader programme!" << endl;
        glfwTerminate();
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;

    GLchar eLog[1024] = { 0 }; //to log the error if any

    //debugging
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking the programme! %s\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating the programme! %s\n", eLog);
        return;
    }

    //get the location of the uniform variable
    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");
}


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

    glEnable(GL_DEPTH_TEST); //enable depth to check which traingle has more depth and which should be at the front

    //setup view port
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f); //fov, aspect ratiom near, far

    //loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) {
        //get and handle user input events
        glfwPollEvents();

        if (direction) {
            triOffset += triInc;
        }
        else {
            triOffset -= triInc;
        }

        if (abs(triOffset) >= triMaxOffset) {
            direction = !direction;
        }

        currAngle += 0.01f;

        if (currAngle >= 360) {
            currAngle -= 360;
        }

        if (direction) {
            currSize += 0.0001f;
        }
        else {
            currSize -= 0.0001f;
        }

        if (currSize >= maxSize || currSize <= minSize) {
            sizeDirection = !sizeDirection;
        }

        //clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        glm::mat4 model = glm::mat4(1.0f); //identity matrix that is used to transform the position

        model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
        //model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));

        //glUniform1f(uniformXMove, triOffset); //1f meaning single point float
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); //12 because we have 4 indices that have 3 values for each
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }
    return 0;
}
