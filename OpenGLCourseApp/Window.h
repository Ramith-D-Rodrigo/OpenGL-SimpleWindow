#pragma once
#include <stdio.h>
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

using namespace std;

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; ///ascii keys	(to check whether the key is pressed)

	GLfloat lastX, lastY, xChange, yChange;  //store mosue movement coordinates
	bool mouseFirstMoved; //very first movement

	void createCallbacks();

	static void handleKeys(GLFWwindow * window, int key, int code, int action, int mode); //callback to inputs 
	//key is between 0 and 1023

	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

