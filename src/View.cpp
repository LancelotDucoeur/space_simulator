// View.cpp
#include "View.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#include <iostream>
#include <cmath>

// Définir des variables globales pour le zoom et la rotation
static double zoomFactor = 0.0001; // Facteur de zoom initial (en unités astronomiques)
static const double zoomIncrement = 0.001; // Incrément de zoom
int planetFocus = 3; // Index de la planète à focaliser (Mercure)

static double cameraTheta = 0.0; // Angle de rotation autour de l'axe Y (horizontal)
static double cameraPhi = 0.0;   // Angle de rotation autour de l'axe X (vertical)
static double lastMouseX = 0.0;  // Dernière position de la souris en X
static double lastMouseY = 0.0;  // Dernière position de la souris en Y
static bool isDragging = false;  // Indique si la souris est en train de glisser (dragging)

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            isDragging = false;
        }
    }
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {
        double deltaX = xpos - lastMouseX;
        double deltaY = ypos - lastMouseY;
        lastMouseX = xpos;
        lastMouseY = ypos;

        // Sensibilité de la souris
        double sensitivity = 0.005;
        cameraTheta += deltaX * sensitivity;
        cameraPhi -= deltaY * sensitivity;

        // Limiter l'angle de rotation vertical
        if (cameraPhi > M_PI / 2) cameraPhi = M_PI / 2;
        if (cameraPhi < -M_PI / 2) cameraPhi = -M_PI / 2;
    }
}

void display(const std::vector<Planet>& planets) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.00001, 100.0); // Ajuster les plans de découpe pour l'usage en AU

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Calculer la position de la caméra en utilisant les angles de rotation
    double cameraX = planets[planetFocus].x / AU + zoomFactor * cos(cameraPhi) * sin(cameraTheta);
    double cameraY = planets[planetFocus].y / AU + zoomFactor * sin(cameraPhi);
    double cameraZ = planets[planetFocus].z / AU + zoomFactor * cos(cameraPhi) * cos(cameraTheta);

    gluLookAt(cameraX, cameraY, cameraZ,  // Position de la caméra
              planets[planetFocus].x / AU, planets[planetFocus].y / AU, planets[planetFocus].z / AU,  // Point de référence (planète)
              0.0, 1.0, 0.0);         // Vecteur "up"

    for (const auto& planet : planets) {
        planet.draw();
    }

    glfwSwapBuffers(glfwGetCurrentContext());

    std::cout << "zoom: " << zoomFactor << std::endl;
    std::cout << "radius: " << planets[1].radius / AU << std::endl;
}

void handleInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        zoomFactor -= zoomIncrement; // Zoom avant
        if (zoomFactor < 0.00001) zoomFactor = 0.00001; // Limiter le zoom avant
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        zoomFactor += zoomIncrement; // Zoom arrière
        if (zoomFactor > 100.0) zoomFactor = 100.0; // Limiter le zoom arrière
    }

    for (int i = GLFW_KEY_0; i <= GLFW_KEY_9; ++i) {
        if (glfwGetKey(window, i) == GLFW_PRESS) {
            planetFocus = i - GLFW_KEY_0;
        }
    }
}
