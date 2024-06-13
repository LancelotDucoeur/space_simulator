// Planet.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Planet.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#include <cmath>
#include <iostream>

Planet::Planet(double _x, double _y, double _z, double _radius, double _mass, float _r, float _g, float _b, const char* texturePath, double _rotationSpeed)
    : x(_x), y(_y), z(_z), radius(_radius), mass(_mass), r(_r), g(_g), b(_b),
      vx(0.0), vy(0.0), vz(0.0), ax(0.0), ay(0.0), az(0.0), rotationSpeed(_rotationSpeed), rotationAngle(0.0) {

    // Charger la texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    stbi_image_free(data);

    // Configurer les paramètres de texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Planet::applyForce(double fx, double fy, double fz) {
    ax += fx / mass;
    ay += fy / mass;
    az += fz / mass;
}

void Planet::update(double dt) {
    vx += ax * dt;
    vy += ay * dt;
    vz += az * dt;
    x += vx * dt;
    y += vy * dt;
    z += vz * dt;
    ax = ay = az = 0.0;  // Réinitialiser l'accélération après mise à jour

    // Mettre à jour l'angle de rotation
    rotationAngle += rotationSpeed * dt;
    if (rotationAngle > 2 * M_PI) {
        rotationAngle -= 2 * M_PI; // Maintenir l'angle entre 0 et 2π
    }

    // Ajouter la position actuelle à la trajectoire
    trajectory.push_back({x / AU, y / AU}); // Convertir en unités astronomiques pour le tracé
    if (trajectory.size() > 1000) { // Limiter la longueur de la trajectoire
        trajectory.erase(trajectory.begin());
    }
}

void Planet::draw() const {
    // Activer l'éclairage et la texture
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Dessiner la planète sans modifier la couleur de base
    glColor3f(1.0f, 1.0f, 1.0f); // Mettre la couleur de base à blanc
    glPushMatrix();
    glTranslatef(x / AU, y / AU, z / AU); // Convertir en unités astronomiques pour l'affichage
    glRotatef(rotationAngle * 180.0 / M_PI, 0.0, 1.0, 0.0); // Appliquer la rotation
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE); // Activer le texturage
    gluSphere(quad, radius / AU, 32, 32); // Convertir en unités astronomiques pour l'affichage
    gluDeleteQuadric(quad);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Dessiner la trajectoire
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& point : trajectory) {
        glVertex3f(point.first, point.second, 0.0);
    }
    glEnd();
}

void computeGravitationalForce(const Planet& p1, const Planet& p2, double& fx, double& fy, double& fz) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dz = p2.z - p1.z;
    double dist = sqrt(dx*dx + dy*dy + dz*dz);
    if (dist < 1e3) { // Distance minimale pour éviter des forces infinies (en mètres)
        dist = 1e3; 
    }
    double force = G * p1.mass * p2.mass / (dist * dist);
    fx = force * dx / dist;
    fy = force * dy / dist;
    fz = force * dz / dist;
}
