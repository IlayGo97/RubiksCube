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
    AddTexture("../res/textures/box0.bmp",false);
    AddTexture("../res/textures/grass.bmp",false);
	pickedShape = 0;
    MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	//ReadPixel(); //uncomment when you are reading from the z-buffer
    bezierCurve = new Bezier1D(3, 91, 0, this);
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
        case GLFW_KEY_DOWN:
            bezierCurve->moveSelectedControlPointDown();
            break;
        case GLFW_KEY_UP:
            bezierCurve->moveSelectedControlPointUp();
            break;
        case GLFW_KEY_LEFT:
            bezierCurve->selectPreviousControlPoint();
            break;
        case GLFW_KEY_RIGHT:
            bezierCurve->selectNextControlPoint();
            break;
        case GLFW_KEY_SPACE:
            bezierCurve->ToggleAnimation();
            break;
        case GLFW_KEY_C:
            bezierCurve->toggle_continuity_state();
            break;
        case GLFW_KEY_2:
            bezierCurve->Restart(2, this);
            break;
        case GLFW_KEY_3:
            bezierCurve->Restart(3, this);
            break;
        case GLFW_KEY_4:
            bezierCurve->Restart(4, this);
            break;
        case GLFW_KEY_5:
            bezierCurve->Restart(5, this);
            break;
        case GLFW_KEY_6:
            bezierCurve->Restart(6, this);
            break;

        case GLFW_KEY_R:
            MoveCamera(0,xTranslate, 1);
            break;
        case GLFW_KEY_L:
            MoveCamera(0,xTranslate, -1);
            break;
        case GLFW_KEY_U:
            MoveCamera(0,yTranslate, 1);
            break;
        case GLFW_KEY_D:
            MoveCamera(0,yTranslate, -1);
            break;
        case GLFW_KEY_F:
            MoveCamera(0,zTranslate, -1);
            break;
        case GLFW_KEY_B:
            MoveCamera(0,zTranslate, 1);
            break;
    }
}

void Game::MouseProccessing(int button) {
    switch (button){
        case GLFW_MOUSE_BUTTON_LEFT:
            bezierCurve->fixCurve();
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            bezierCurve->TranslateSelectedPoint(glm::vec3(-xrel/20.0f,yrel/20.0f,0));
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
//    MoveCamera(0,zTranslate,yoffset * -1);
    if(yoffset < 0){
        bezierCurve->moveSelectedControlPointIn();
    } else {
        bezierCurve->moveSelectedControlPointOut();
    }

}

void Game::AddBezier1DShape(Shape *bezier_1D_line, int parent) {
    chainParents.push_back(parent);
    shapes.push_back(bezier_1D_line);
}
