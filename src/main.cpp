// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Planet.h"
#include "View.h"

const double SUN_MASS = 1.989e30; // Masse du Soleil en kg
const double DAY = 86400; // Secondes dans une journée

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Position de la lumière (Soleil)
    GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Lumière ambiante faible
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Lumière diffuse intense
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Lumière spéculaire intense

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    initLighting(); // Initialiser l'éclairage

    std::vector<Planet> planets;
    // Soleil
    planets.emplace_back(0.0, 0.0, 0.0, 696340000.0, SUN_MASS, 1.0f, 1.0f, 0.0f, "textures/sun.jpeg"); 

    // Mercure
    double mercuryDistance = 0.39 * AU;
    double mercuryOrbitalSpeed = sqrt(G * SUN_MASS / mercuryDistance);
    planets.emplace_back(mercuryDistance, 0.0, 0.0, 2439700.0, 3.3011e23, 0.5f, 0.5f, 0.5f, "textures/mercury.jpg");
    planets.back().vy = mercuryOrbitalSpeed;

    // Vénus
    double venusDistance = 0.72 * AU;
    double venusOrbitalSpeed = sqrt(G * SUN_MASS / venusDistance);
    planets.emplace_back(venusDistance, 0.0, 0.0, 6051800.0, 4.8675e24, 1.0f, 0.5f, 0.0f, "textures/venus.jpg");
    planets.back().vy = venusOrbitalSpeed;

    // Terre
    double earthDistance = AU; // Distance entre la Terre et le Soleil en mètres
    double earthOrbitalSpeed = sqrt(G * SUN_MASS / earthDistance);
    planets.emplace_back(earthDistance, 0.0, 0.0, 6371000.0, 5.972e24, 0.0f, 0.0f, 1.0f, "textures/earth.jpeg");   
    planets.back().vy = earthOrbitalSpeed;

    // Lune
    double moonDistance = 384400 * 1000; // Distance Terre-Lune en mètres
    double moonOrbitalSpeed = sqrt(G * 5.972e24 / moonDistance);
    planets.emplace_back(earthDistance + moonDistance, 0.0, 0.0, 1737100.0, 7.347e22, 1.0f, 1.0f, 1.0f, "textures/moon.jpeg");
    planets.back().vy = earthOrbitalSpeed + moonOrbitalSpeed;

    // Mars
    double marsDistance = 1.524 * AU; // Distance entre Mars et le Soleil en mètres
    double marsOrbitalSpeed = sqrt(G * SUN_MASS / marsDistance);
    planets.emplace_back(marsDistance, 0.0, 0.0, 3389500.0, 6.39e23, 1.0f, 0.0f, 0.0f, "textures/mars.jpeg");   
    planets.back().vy = marsOrbitalSpeed;

    // Jupiter
    double jupiterDistance = 5.2 * AU;
    double jupiterOrbitalSpeed = sqrt(G * SUN_MASS / jupiterDistance);
    planets.emplace_back(jupiterDistance, 0.0, 0.0, 69911000.0, 1.8982e27, 1.0f, 0.5f, 0.0f, "textures/jupiter.jpeg");
    planets.back().vy = jupiterOrbitalSpeed;

    // Saturne
    double saturnDistance = 9.58 * AU;
    double saturnOrbitalSpeed = sqrt(G * SUN_MASS / saturnDistance);
    planets.emplace_back(saturnDistance, 0.0, 0.0, 58232000.0, 5.6834e26, 1.0f, 1.0f, 0.5f, "textures/saturn.jpeg");
    planets.back().vy = saturnOrbitalSpeed;

    // Uranus
    double uranusDistance = 19.2 * AU;
    double uranusOrbitalSpeed = sqrt(G * SUN_MASS / uranusDistance);
    planets.emplace_back(uranusDistance, 0.0, 0.0, 25362000.0, 8.6810e25, 0.5f, 1.0f, 1.0f, "textures/uranus.jpeg");
    planets.back().vy = uranusOrbitalSpeed;

    // Neptune
    double neptuneDistance = 30.05 * AU;
    double neptuneOrbitalSpeed = sqrt(G * SUN_MASS / neptuneDistance);
    planets.emplace_back(neptuneDistance, 0.0, 0.0, 24622000.0, 1.02413e26, 0.5f, 0.0f, 1.0f, "textures/neptune.jpeg");
    planets.back().vy = neptuneOrbitalSpeed;

    double simulationTime = 0.0; // Temps écoulé en secondes

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    while (!glfwWindowShouldClose(window)) {
        handleInput(window); // Gérer les entrées de l'utilisateur

        // Calculer les forces gravitationnelles
        for (size_t i = 0; i < planets.size(); ++i) {
            for (size_t j = i + 1; j < planets.size(); ++j) {
                double fx, fy, fz;
                computeGravitationalForce(planets[i], planets[j], fx, fy, fz);
                planets[i].applyForce(fx, fy, fz);
                planets[j].applyForce(-fx, -fy, -fz);
            }
        }

        // Mettre à jour les positions des planètes
        double dt = 60 * 60 * 24; // Intervalle de temps en secondes (1 jour)
        for (auto& planet : planets) {
            planet.update(dt);
        }

        // Mettre à jour le temps de simulation
        simulationTime += dt;

        // Afficher les planètes
        display(planets);
        glfwPollEvents();

        // Afficher le temps de simulation en jours
        std::cout << "Simulation Time: " << simulationTime / DAY << " days" << std::endl;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
