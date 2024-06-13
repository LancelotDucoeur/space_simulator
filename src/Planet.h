// Planet.h
#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include <utility>
#include <GL/glew.h>

const double G = 6.67430e-11; // m^3 kg^-1 s^-2
const double AU = 1.496e11; // Unité astronomique en mètres (distance moyenne Terre-Soleil)
const double DISTANCE_SCALE = 1.0; // Échelle pour les distances réelles
const double SIZE_SCALE = 1.0; // Échelle pour les tailles réelles

class Planet {
public:
    double x, y, z;      // Position de la planète (en mètres)
    double vx, vy, vz;   // Vitesse de la planète (en mètres par seconde)
    double ax, ay, az;   // Accélération de la planète (en mètres par seconde carré)
    double mass;         // Masse de la planète (en kg)
    double radius;       // Rayon de la planète (en mètres)
    float r, g, b;       // Couleur de la planète
    GLuint texture;      // Texture de la planète
    std::vector<std::pair<double, double>> trajectory; // Trajectoire pour le tracé

    Planet(double _x, double _y, double _z, double _radius, double _mass, float _r, float _g, float _b, const char* texturePath);

    void applyForce(double fx, double fy, double fz);
    void update(double dt);
    void draw() const;
};

void computeGravitationalForce(const Planet& p1, const Planet& p2, double& fx, double& fy, double& fz);

#endif // PLANET_H
