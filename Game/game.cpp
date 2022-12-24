#include "game.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <ratio>
#include <thread>
#define CLOCKWISE_ROTATE -1
#define COUNTERCLOCKWISE_ROTATE 1


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
void Game::Init()
{
	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");
	pickedShape = 0;
    MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	//ReadPixel(); //uncomment when you are reading from the z-buffer
    rub = new rubik(3, this);
    rub->setClockDirection(CLOCKWISE_ROTATE);
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
        case GLFW_KEY_SPACE:
            rub->flip_rotation();
            break;
        case GLFW_KEY_Z:
            rub->halve_rotation_degree();
            break;
        case GLFW_KEY_A:
            rub->double_rotation_degree();
            break;
        case GLFW_KEY_R:
            rub->right_wall_rotation();
            break;
        case GLFW_KEY_L:
            rub->left_wall_rotation();
            break;
        case GLFW_KEY_U:
            rub->up_wall_rotation();
            break;
        case GLFW_KEY_D:
            rub->down_wall_rotation();
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

void Game::scrollCallback(double xoffset, double yoffset) {
    MoveCamera(0,zTranslate,yoffset * -1);
}
