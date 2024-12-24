
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> 
#include <vector>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "loadShaders.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "objloader.hpp"  
#include <random>


GLuint
VaoId,
VboId,
ProgramId,
nrVertLocation,
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

// Variabila pentru numarul de varfuri
int nrVertices;

std::vector<GLuint> VaoIds, VboIds;

// Vectori pentru varfuri, coordonate de texturare, normale
std::vector<glm::vec3> vertices;
std::vector<std::vector<glm::vec3>> caseVector;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

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
int nrCase = 4;

float umbraOffsetX = 0.f, umbraOffsetY = 0.f;

// umbra
float matrUmbra[4][4];

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case '-':
		dist -= 0.25;	//	apasarea tastelor `+` si `-` schimba pozitia observatorului (se departeaza / aproprie);
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
	}
	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)				//	Procesarea tastelor 'LEFT', 'RIGHT', 'UP', 'DOWN';
	{							//	duce la deplasarea observatorului pe suprafata sferica in jurul cubului;
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
	}
}


void CreateVBO(int VaoIndex, std::vector<glm::vec3>& vert)
{

	// Generare VAO;
	glGenVertexArrays(1, &VaoIds[VaoIndex]);
	glBindVertexArray(VaoIds[VaoIndex]);

	glGenBuffers(1, &VboIds[VaoIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, VboIds[VaoIndex]);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vert.size() * sizeof(glm::vec3), &vert[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), normals.size() * sizeof(glm::vec3), &normals[0]);

	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = normale
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(vert.size() * sizeof(glm::vec3)));

}

void CreateFloorVBO() {
	GLfloat Vertices[] = {
	   -1500.0f,  -0.0f, -1500.0f,
		1500.0f,  -0.0f, -1500.0f,
		1500.0f,  -0.0f,  1500.0f,
	   -1500.0f,  -0.0f,  1500.0f,
	};

	GLfloat Normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	VaoIds.push_back(0);
	glGenVertexArrays(1, &VaoIds[nrCase]);
	glBindVertexArray(VaoIds[nrCase]);

	VboIds.push_back(0);
	glGenBuffers(1, &VboIds[nrCase]);
	glBindBuffer(GL_ARRAY_BUFFER, VboIds[nrCase]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices) + sizeof(Normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), &Vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices), sizeof(Normals), &Normals[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Normals), (GLvoid*)(sizeof(Vertices)));

}

//  Eliminarea obiectelor de tip VBO dupa rulare;
void DestroyVBO(void)
{
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

//  Crearea si compilarea obiectelor de tip shader;
void CreateShaders(void)
{
	ProgramId = LoadShaders("10_01_Shader.vert", "10_01_Shader.frag");
	glUseProgram(ProgramId);
}

// Elimina obiectele de tip shader dupa rulare;
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

//  Functia de eliberare a resurselor alocate de program;
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
};

//  Setarea parametrilor necesari pentru fereastra de vizualizare;
void Initialize(void)
{
	glClearColor(0.95f, 0.82f, 0.4f, 1.0f); // culoarea de fond a ecranului

	// Incarcarea modelului 3D in format OBJ, trebuie sa fie in acelasi director cu proiectul actual;
	bool model = loadOBJ("casa.obj", vertices, uvs, normals);
	nrVertices = vertices.size();

	// Crearea VBO / shadere-lor

	std::random_device rd;
	std::mt19937_64 mt(rd());
	std::uniform_real_distribution<float> dist(-3.0, 3.0);
	float zOffset = 1.5f;
	for (int vaoIndex = 0; vaoIndex < nrCase; vaoIndex++) {
		VaoIds.push_back(0);
		VboIds.push_back(0);
		std::vector<glm::vec3> newVertices = vertices;
		caseVector.push_back(newVertices);
		std::cout << caseVector.size() << '\n';
		float xOffset = dist(mt);
		for (int i = 0; i < vertices.size(); i++) {
			caseVector[vaoIndex][i][0] += xOffset;
			caseVector[vaoIndex][i][2] += zOffset - (float)vaoIndex * 1.1f;
		}
		CreateVBO(vaoIndex, caseVector[vaoIndex]);
	}

	CreateFloorVBO();

	CreateShaders();

	// Locatii ptr shader
	nrVertLocation = glGetUniformLocation(ProgramId, "nrVertices");
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	viewLocation = glGetUniformLocation(ProgramId, "view");
	projLocation = glGetUniformLocation(ProgramId, "projection");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
	xLLocation = glGetUniformLocation(ProgramId, "xL");
	yLLocation = glGetUniformLocation(ProgramId, "yL");
	zLLocation = glGetUniformLocation(ProgramId, "zL");
	fCeataLocation = glGetUniformLocation(ProgramId, "fCeata");


	// Variabile ce pot fi transmise catre shader
	glUniform1i(ProgramId, nrVertices);
}

void RenderFunction(void)
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
	glm::vec3 obs = glm::vec3(obsX, obsY, obsZ);        // Pozitia observatorului;    
	glm::vec3 pctRef = glm::vec3(refX, refY, refZ);     // Pozitia punctului de referinta;
	glm::vec3 vert = glm::vec3(vX, vY, vZ);             // Verticala din planul de vizualizare; 
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

	matrUmbra[0][0] = zL + D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
	matrUmbra[1][0] = 0; matrUmbra[1][1] = zL + D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
	matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
	matrUmbra[3][0] = -D * xL; matrUmbra[3][1] = -D * yL; matrUmbra[3][2] = -D * zL; matrUmbra[3][3] = zL;
	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);


	glUniform1i(codColLocation, 1);

	// Legarea VAO, desenare;
	for (int indexCasa = 0; indexCasa < nrCase; indexCasa++) {
		glBindVertexArray(VaoIds[indexCasa]);
		glBindBuffer(GL_ARRAY_BUFFER, VboIds[indexCasa]);
		glEnableVertexAttribArray(0);  // atributul 0 = pozitie
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}


	glUniform1i(codColLocation, 2);

	glBindVertexArray(VaoIds[nrCase]);
	glBindBuffer(GL_ARRAY_BUFFER, VboIds[nrCase]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3 * 4);


	glUniform1i(codColLocation, 0);

	for (int indexCasa = 0; indexCasa < nrCase; indexCasa++) {
		glBindVertexArray(VaoIds[indexCasa]);
		glBindBuffer(GL_ARRAY_BUFFER, VboIds[indexCasa]);
		glEnableVertexAttribArray(0);  // atributul 0 = pozitie
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}


	glutSwapBuffers();
	glFlush();
}

//	Punctul de intrare in program, se ruleaza rutina OpenGL;
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Utilizarea unui model predefinit in format OBJ");
	glewInit();
	Initialize();
	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

