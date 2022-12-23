#include "game.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <ratio>
#include <thread>

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}
block * b1;
void Game::Init()
{

	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");
	pickedShape = 0;
    MoveCamera(0,zTranslate,10);
	pickedShape = -1;
    AddBlock(glm::vec3(0.f, 0.f, 1.f));
    int index = AddBlock(glm::vec3(1.f, 1.f, 1.f));
    b1 = (block *)shapes[index];
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::keyListener(int key) {
    switch(key){
        case GLFW_KEY_A:
            b1->rotate_around_axis_clockwise(glm::vec3(0.f, 0.f, 1.f));
        break;
    }
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
