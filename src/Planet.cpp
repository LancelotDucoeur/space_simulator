// Planet.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Planet.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#include <cmath>
#include <iostream>

Planet::Planet(double _x, double _y, double _z, double _radius, double _mass, float _r, float _g, float _b, const char* texturePath, double _rotationSpeed, const char* ringTexturePath)
    : x(_x), y(_y), z(_z), radius(_radius), mass(_mass), r(_r), g(_g), b(_b),
      vx(0.0), vy(0.0), vz(0.0), ax(0.0), ay(0.0), az(0.0), rotationSpeed(_rotationSpeed), rotationAngle(0.0), ringTexture(0) {

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

        // Charger la texture des anneaux si spécifiée
    if (ringTexturePath) {
        glGenTextures(1, &ringTexture);
        glBindTexture(GL_TEXTURE_2D, ringTexture);

        data = stbi_load(ringTexturePath, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load ring texture: " << ringTexturePath << std::endl;
        }
        stbi_image_free(data);

        // Configurer les paramètres de texture des anneaux
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
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
    glRotatef(rotationAngle * 180.0 / M_PI, 0.0, 0.0, 1.0); // Appliquer la rotation
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE); // Activer le texturage
    gluSphere(quad, radius / AU, 32, 32); // Convertir en unités astronomiques pour l'affichage
    gluDeleteQuadric(quad);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Dessiner les anneaux pour Saturne
    if (mass == 5.6834e26) { // Vérifiez si c'est Saturne
        drawRings();
        std::cout << "test" << std::endl;
    }

    // Dessiner la trajectoire
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& point : trajectory) {
        glVertex3f(point.first, point.second, 0.0);
    }
    glEnd();
}

void Planet::drawRings() const {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ringTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Couleur des anneaux avec transparence
    glPushMatrix();
    glTranslatef(x / AU, y / AU, z / AU); // Convertir en unités astronomiques pour l'affichage
    glRotatef(90, 1.0, 0.0, 0.0); // Aligner les anneaux sur le plan XY

    double innerRadius = 122170000.0 / AU; // 122,170 km en mètres
    double outerRadius = 136775000.0 / AU; // 136,775 km en mètres
    int numSegments = 100;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= numSegments; ++i) {
        double theta = 2.0 * M_PI * i / numSegments;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        glTexCoord2f(i / (float)numSegments, 0.0f);
        glVertex3f(innerRadius * cosTheta, 0, innerRadius * sinTheta);
        glTexCoord2f(i / (float)numSegments, 1.0f);
        glVertex3f(outerRadius * cosTheta, 0, outerRadius * sinTheta);
    }
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
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
