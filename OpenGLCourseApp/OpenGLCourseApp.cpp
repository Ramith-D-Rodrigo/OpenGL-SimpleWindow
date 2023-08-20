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
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

using namespace std;

//use the following in the newer glm versions
//glm::mat4 model(1.0f);
//glm::mat4 model = glm::mat4(1.0f);
//model = glm::mat(1.0f);

const float toRadians = 3.14159265f / 180.0f;

vector<Mesh*> meshList;
vector<Shader> shaderList;
Window mainWindow;

//window size
const GLint WIDTH = 800, HEIGHT = 600;

//creating the vertex shader
static const char* vShader = "Shaders/shader.vert";


//fragment shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() {

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

    Mesh* obj1 = new Mesh();

    obj1 -> CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();

    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
}

void CreateShaders() {
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}


int main()
{
    mainWindow = Window(1280, 720);
    mainWindow.Initialize();

    CreateObjects();
    CreateShaders();

    GLuint uniformModel = 0, uniformProjection = 0;

    glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f); //fov, aspect ratiom near, far

    //loop until window closed
    while (!mainWindow.getShouldClose()) {
        //get and handle user input events
        glfwPollEvents();

        //clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();

        glm::mat4 model = glm::mat4(1.0f); //identity matrix that is used to transform the position

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        //model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        //glUniform1f(uniformXMove, triOffset); //1f meaning single point float
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f); //new one (clearning out the other transformations)
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.3f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

        glUseProgram(0);

        mainWindow.swapBuffers();
    }
    return 0;
}
