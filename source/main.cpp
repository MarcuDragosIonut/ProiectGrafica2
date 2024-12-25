#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <windows.h>

#include <GL/glew.h>

#include <GL/freeglut.h>

#include "Ground.h"
#include "House.h"
#include "Model.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// TODO v
GLuint
        myMatrixLocation,
        viewLocation,
        projLocation,
        matrUmbraLocation,
        shadowStrengthLocation,
        xLLocation,
        yLLocation,
        zLLocation;

float PI = 3.141592;
// TODO ^

constexpr int screenWidth = 1600;
constexpr int screenHeight = 900;

Shader *shader;
Model *houseModel;

Ground *ground;
std::vector<House *> houses{};

// TODO v
// Matrice utilizate
glm::mat4 myMatrix;
glm::mat4 view;
glm::mat4 projection;

//	Elemente pentru matricea de vizualizare;
float refX = 0.0f, refY = 0.0f, refZ = 0.0f,
        obsX, obsY, obsZ,
        vX = 0.0f, vY = 0.0f, vZ = 1.0f;
//	Elemente pentru deplasarea pe sfera;
float alpha = 0.0f, beta = 0.0f, dist = 6.0f,
        incrAlpha1 = 0.01, incrAlpha2 = 0.01;
//	Elemente pentru matricea de proiectie;
float width = 800, height = 600, dNear = 0.1f, fov = 60.f * PI / 180;

float timeOfDay = 8.f, daySplit = 24.f, dayStep = 0.25f,
        radius = 1000.f; // cat de departe e sursa de lumina (soarele) de scena

// umbra
float matrUmbra[4][4];
// TODO ^

void init() {
    glClearColor(0.95f, 0.82f, 0.4f, 1.0f);
    shader = new Shader("MainShader.vert", "MainShader.frag");
    houseModel = new Model("Cub.obj");

    // TODO v
    myMatrixLocation = shader->GetUniform("myMatrix");
    viewLocation = shader->GetUniform("view");
    projLocation = shader->GetUniform("projection");
    matrUmbraLocation = shader->GetUniform("matrUmbra");
    shadowStrengthLocation = shader->GetUniform("shadowStrength");
    xLLocation = shader->GetUniform("xL");
    yLLocation = shader->GetUniform("yL");
    zLLocation = shader->GetUniform("zL");
    // TODO ^
}

void createObjects() {
    ground = new Ground(1500, shader);

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_real_distribution<float> dist(-3.0, 3.0);
    for (int i = 0; i < 4; i++) {
        houses.push_back(new House(houseModel, dist(mt), 1.5f - static_cast<float>(i) * 1.1f, shader));
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Pentru alpha value la culori
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->Reset();

    // TODO v

    // Matricea de modelare
    myMatrix = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0.0, 1.0, 0.0))
               * glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0.0, 0.0, 1.0));
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

    // Vizualizare;
    // Pozitia observatorului - se deplaseaza pe sfera;
    obsX = refX + dist * cos(alpha) * cos(beta);
    obsY = refY + dist * cos(alpha) * sin(beta);
    obsZ = refZ + dist * sin(alpha);
    // Vectori pentru matricea de vizualizare;
    glm::vec3 obs = glm::vec3(obsX, obsY, obsZ); // Pozitia observatorului;
    glm::vec3 pctRef = glm::vec3(refX, refY, refZ); // Pozitia punctului de referinta;
    glm::vec3 vert = glm::vec3(vX, vY, vZ); // Verticala din planul de vizualizare;
    // Matricea de vizualizare, transmitere catre shader
    view = glm::lookAt(obs, pctRef, vert);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

    // Proiectie;
    projection = glm::infinitePerspective(GLfloat(fov), GLfloat(width) / GLfloat(height), dNear);
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);


    float
    // Coordonate directie lumina
    xL = 500.f, yL = 300.f, zL = 400.f,
    // offset Umbra
    D = -0.01f;

    // Simulare soare

    float daytimeElapsed = timeOfDay / daySplit;
    float sunAngle = daytimeElapsed * 2.0f * PI;

    yL = std::sin(sunAngle) * radius/2 + radius/2;
    xL = radius * cos(sunAngle);
    zL = radius * sin(sunAngle);

    float shadowStrength = 4 * daytimeElapsed * (1 - daytimeElapsed);
    shadowStrength = 1.f; // TODO

    glUniform1f(xLLocation, xL);
    glUniform1f(yLLocation, yL);
    glUniform1f(zLLocation, zL);

    matrUmbra[0][0] = zL + D;
    matrUmbra[0][1] = 0;
    matrUmbra[0][2] = 0;
    matrUmbra[0][3] = 0;
    matrUmbra[1][0] = 0;
    matrUmbra[1][1] = zL + D;
    matrUmbra[1][2] = 0;
    matrUmbra[1][3] = 0;
    matrUmbra[2][0] = -xL;
    matrUmbra[2][1] = -yL;
    matrUmbra[2][2] = D;
    matrUmbra[2][3] = -1;
    matrUmbra[3][0] = -D * xL;
    matrUmbra[3][1] = -D * yL;
    matrUmbra[3][2] = -D * zL;
    matrUmbra[3][3] = zL;
    glUniform1f(shadowStrengthLocation, shadowStrength);
    glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);

    // TODO ^

    ground->Render();
    for (const auto house: houses) {
        house->Render();
    }

    glutSwapBuffers();
    glFlush();
}

void input_normal(const unsigned char key, [[maybe_unused]] const int x, [[maybe_unused]] const int y) {
    int a = 22;
    // TODO v
    switch (key) {
        case '-':
            dist -= 0.25;
            break;
        case '+':
            dist += 0.25;
            break;

        case 'a':
            timeOfDay += dayStep;
            if (timeOfDay > daySplit) timeOfDay = 0.f;
            break;
        case 'd':
            timeOfDay -= dayStep;
            if (timeOfDay < 0.f) timeOfDay = daySplit;
            break;

        default:
            break;
    }
    // TODO ^
}

void input_special(const int key, [[maybe_unused]] const int x, [[maybe_unused]] const int y) {
    // TODO v
    switch (key) {
        case GLUT_KEY_LEFT:
            beta -= 0.03;
            break;
        case GLUT_KEY_RIGHT:
            beta += 0.03;
            break;
        case GLUT_KEY_UP:
            alpha += incrAlpha1;
            if (abs(alpha - PI / 2) < 0.05) {
                incrAlpha1 = 0.f;
            } else {
                incrAlpha1 = 0.03f;
            }
            break;
        case GLUT_KEY_DOWN:
            alpha -= incrAlpha2;
            if (abs(alpha + PI / 2) < 0.05) {
                incrAlpha2 = 0.f;
            } else {
                incrAlpha2 = 0.03f;
            }
            break;

        default:
            break;
    }
    // TODO ^
}

void close() {
    delete shader;
    delete houseModel;
    delete ground;
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition((1920 - screenWidth) / 2, (1080 - screenHeight) / 2 - 40);
    glutCreateWindow("Proiect 3D OpenGL");
    glewInit();

    init();
    createObjects();

    glutIdleFunc(render);
    glutDisplayFunc(render);
    glutKeyboardFunc(input_normal);
    glutSpecialFunc(input_special);
    glutCloseFunc(close);

    glutMainLoop();
}
