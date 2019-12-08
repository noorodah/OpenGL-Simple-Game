#pragma once

// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <GL/freeglut.h> // Contains all the necessery OpenGL includes

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//to map image filenames to textureIds
#include <IL\il.h>
#include <IL\ilut.h>
#include <IL\ilu.h>

// assimp include files. These three are usually needed.
#include <assimp/Importer.hpp>	//OO version Header!
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

// Std. Includes
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

bool Import3DFromFile(const std::string& pFile);
void drawAiScene();


const std::string modelpath = "media/male.obj";

