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

GLuint
        VaoId,
        VboId,
        myMatrixLocation,
        viewLocation,
        projLocation,
        codColLocation,
        matrUmbraLocation,
        xLLocation,
        yLLocation,
        zLLocation,
        fCeataLocation;

float PI = 3.141592;

constexpr int screenWidth = 1600;
constexpr int screenHeight = 900;

Shader *shader;
Model *houseModel;

Ground *ground;
std::vector<House *> houses{};

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

float umbraOffsetX = 0.f, umbraOffsetY = 0.f;

// umbra
float matrUmbra[4][4];

void init()
{
    glClearColor(0.95f, 0.82f, 0.4f, 1.0f);
    shader = new Shader("MainShader.vert", "MainShader.frag");
    houseModel = new Model("casa.obj");

    myMatrixLocation = shader->GetUniform("myMatrix");
    viewLocation = shader->GetUniform("view");
    projLocation = shader->GetUniform("projection");
    codColLocation = shader->GetUniform("codCol");
    matrUmbraLocation = shader->GetUniform("matrUmbra");
    xLLocation = shader->GetUniform("xL");
    yLLocation = shader->GetUniform("yL");
    zLLocation = shader->GetUniform("zL");
    fCeataLocation = shader->GetUniform("fCeata");
}

void createObjects()
{
    ground = new Ground(1500, shader);

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_real_distribution<float> dist(-3.0, 3.0);
    for (int i = 0; i < 4; i++)
    {
        houses.push_back(new House(houseModel, dist(mt), 1.5f - static_cast<float>(i) * 1.1f, shader));
    }
}

void render()
{
   // Initializare ecran + test de adancime;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

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

    // ceata
    float fCeataArg = 0.01;
    glUniform1f(fCeataLocation, std::exp(-fCeataArg * (refZ + dist)));

    // matricea pentru umbra
    float D = -0.01f, xL = 300.f + umbraOffsetX, yL = -400.f + umbraOffsetY, zL = 400.f;

    std::cout << umbraOffsetX << ' ' << umbraOffsetY << '\n';

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
    glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);

    ground->Render();
    for (const auto house : houses)
    {
        house->Render();
    }

    glutSwapBuffers();
    glFlush();
}

void input_normal(const unsigned char key, [[maybe_unused]] const int x, [[maybe_unused]] const int y)
{
    switch (key)
    {
        case '-':
            dist -= 0.25;
            break;
        case '+':
            dist += 0.25;
            break;

        case 'a':
            umbraOffsetX -= 10.f;
            break;
        case 'd':
            umbraOffsetX += 10.f;
            break;
        case 'w':
            umbraOffsetY += 10.f;
            break;
        case 's':
            umbraOffsetY -= 10.f;
            break;

        default:
            break;
    }
    if (key == 27)
        exit(0);
}

void input_special(const int key, [[maybe_unused]] const int x, [[maybe_unused]] const int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            beta -= 0.03;
            break;
        case GLUT_KEY_RIGHT:
            beta += 0.03;
            break;
        case GLUT_KEY_UP:
            alpha += incrAlpha1;
            if (abs(alpha - PI / 2) < 0.05)
            {
                incrAlpha1 = 0.f;
            }
            else
            {
                incrAlpha1 = 0.03f;
            }
            break;
        case GLUT_KEY_DOWN:
            alpha -= incrAlpha2;
            if (abs(alpha + PI / 2) < 0.05)
            {
                incrAlpha2 = 0.f;
            }
            else
            {
                incrAlpha2 = 0.03f;
            }
            break;

        default:
            break;
    }
}

void close()
{
    delete shader;
    delete houseModel;
    delete ground;
}

int main(int argc, char *argv[])
{
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
