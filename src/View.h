#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include "Planet.h"
#include <GLFW/glfw3.h>

void display(const std::vector<Planet>& planets);
void handleInput(GLFWwindow* window);

// Déclarations des fonctions de rappel de la souris
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

#endif // VIEW_H
