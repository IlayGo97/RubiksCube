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
    CreateRoute3DBezier1D( 3, 91, 0);
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
    AnimateCubeMovement(isActive);

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
//    switch(key){
//        case GLFW_KEY_SPACE:
//            rub->flip_rotation();
//            break;
//        case GLFW_KEY_Z:
//            rub->halve_rotation_degree();
//            break;
//        case GLFW_KEY_A:
//            rub->double_rotation_degree();
//            break;
//        case GLFW_KEY_R:
//            rub->right_wall_rotation();
//            break;
//        case GLFW_KEY_L:
//            rub->left_wall_rotation();
//            break;
//        case GLFW_KEY_U:
//            rub->up_wall_rotation();
//            break;
//        case GLFW_KEY_D:
//            rub->down_wall_rotation();
//            break;
//        case GLFW_KEY_F:
//            rub->front_wall_rotation();
//            break;
//        case GLFW_KEY_B:
//            rub->back_wall_rotation();
//            break;
//    }
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

void Game::AddBezier1DShape(Shape* bezier_1D_line, int parent)
{
    chainParents.push_back(parent);
    shapes.push_back(bezier_1D_line);
}

void Game::MouseScrolling(glm::vec3 delta, int mode)
{
    if (pickedShape == -1) {
        MyTranslate(delta, mode);
    }
    else {
        glm::mat4 rot_inverse = glm::inverse(getRot());
        glm::vec4 trans = rot_inverse * glm::vec4(delta.x, delta.y, delta.z, 1);
        shapes[pickedShape]->MyTranslate(glm::vec3(trans.x, trans.y, trans.z), mode);

        UpdateCurveByShapes();
    }
}

void Game::CreateRoute3DBezier1D(int segNum, int res, int mode) {

    // Cube + Octahedrons texture
    AddTexture("../res/textures/box0.bmp", false);

    // Bezier 1D texture
    AddTexture("../res/textures/grass.bmp", false);

    bezier_1D = new Bezier1D(segNum, res, Scene::LINE_STRIP);

    BuildAllShapes();
    NumberOfSegmentsToDisplay(segNum);

    // Building Bezier 1D
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
    AddBezier1DShape(bezier_1D, -1);
    SetShapeTex(shapes.size() - 1, 1);

    MoveCamera(0, Scene::zTranslate, 50);
}

void Game::AnimateCubeMovement(bool animate) {
    if (animate) {
        glm::vec3 velocity_before = glm::normalize(bezier_1D->GetVelosity(anim.cube_segment, anim.cube_t));

        if (anim.forward_direction) {
            if (anim.cube_t > 0.99f) {
                anim.cube_segment++;
                anim.cube_t = 0;
            }
            else {
                anim.cube_t += anim.cube_step_size;
            }
            if (anim.cube_segment == bezier_1D->GetSegmentsNum()) {
                anim.cube_t = 1;
                anim.cube_segment--;
                anim.forward_direction = false;
            }
        }
        else {
            if (anim.cube_t < 0.01f) {
                anim.cube_segment--;
                anim.cube_t = 1;
            }
            else {
                anim.cube_t -= anim.cube_step_size;
            }
            if (anim.cube_segment == -1) {
                anim.cube_t = 0;
                anim.cube_segment++;
                anim.forward_direction = true;
            }
        }

        // Translation
        glm::vec4 cube_center = shapes[anim.cube_shape_index]->getTrans()[3];
        glm::vec4 next_position = bezier_1D->GetPointOnCurve(anim.cube_segment, anim.cube_t);
        glm::vec4 move_vector = next_position - cube_center;
        shapes[anim.cube_shape_index]->MyTranslate(glm::vec3(move_vector.x, move_vector.y, move_vector.z), 0);

        // Rotation
        glm::vec3 velocity_after = glm::normalize(bezier_1D->GetVelosity(anim.cube_segment, anim.cube_t));
        float angle = glm::dot(velocity_before, velocity_after);
        glm::vec3 normal = glm::cross(velocity_before, velocity_after);

        // Not working yet
        glm::vec3 x = glm::vec3(next_position.x, next_position.y, next_position.z);
        x = glm::normalize(x);
        // x now points at target, unit length
        glm::vec3 y = glm::cross(glm::vec3(0, 0, 1), glm::vec3(x.x, x.y, x.z));
        y = glm::normalize(y);
        // y is now perpendicular to x, unit length
        glm::vec3 z = glm::cross(glm::vec3(x.x, x.y, x.z), y);
        // z is now perpendicular to both x and y, and unit length
        z = glm::normalize(z);

        glm::mat4 matrix;
        matrix[0] = glm::vec4(x, 0);
        matrix[1] = glm::vec4(y,0);
        matrix[2] = glm::vec4(z, 0);
        matrix[3] = glm::vec4(0, 0, 0, 1);

        bool condition = ((normal.x == 0) && (normal.y == 0) && (normal.z == 0));
        if (!condition) {
            shapes[anim.cube_shape_index]->MyRotate(angle/111.f, normal, 0);
        }
    }
}

void Game::UpdateCurveByShapes() {
    glm::vec4 p0 = shapes[0]->getTrans()[3];
    glm::vec4 p1 = shapes[1]->getTrans()[3];
    glm::vec4 p2 = shapes[2]->getTrans()[3];
    glm::vec4 p3 = shapes[3]->getTrans()[3];
    bezier_1D->ChangeFirstSegment(p0, p1, p2, p3);

    int segIndx = 1;
    for (int i = 4; i < shapes.size() - 2; i += 3) {
        glm::vec4 p1 = shapes[i]->getTrans()[3];
        glm::vec4 p2 = shapes[i + 1]->getTrans()[3];
        glm::vec4 p3 = shapes[i + 2]->getTrans()[3];

        bezier_1D->ChangeSegment(segIndx, p1, p2, p3);
        segIndx++;
    }
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
}

bool Game::OnCurvePoint(int index) {
    if (index % 3 == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool Game::HasLeft(int index) {
    return index > first_point_index;
}

bool Game::HasRight(int index) {
    return index < last_point_index;
}

int Game::NextShape() {
    picked_shape_index++;
    if (picked_shape_index > last_point_index) {
        picked_shape_index = first_point_index;
    }
    return picked_shape_index;
}

int Game::PreviousShape() {
    picked_shape_index--;
    if (picked_shape_index < first_point_index) {
        picked_shape_index = last_point_index;
    }
    return picked_shape_index;
}

void Game::NumberOfSegmentsToDisplay(int segNum) {
    int res = ((bezier_1D->getResT() - 1) / bezier_1D->GetSegmentsNum() * segNum) + 1;
    bezier_1D->setSegmentsNum(segNum);
    bezier_1D->setResT(res);

    int config_num = segNum - 2;

    // Reset Cube position
    glm::vec3 cube_new_position = bezier_configs[config_num][0];
    glm::vec4 cube_old_position = shapes[anim.cube_shape_index]->getTrans()[3];
    glm::vec3 movement = cube_new_position - glm::vec3(cube_old_position.x, cube_old_position.y, cube_old_position.z);
    shapes[anim.cube_shape_index]->MyTranslate(movement, 0);

    for (int i = 0; i < number_of_octahedrons; i++) {
        if (i < bezier_configs[config_num].size()) {
            shapes[i]->Unhide();

            // Reset Octahedrons posotions
            glm::vec3 point_new_position = bezier_configs[config_num][i];
            glm::vec4 point_old_position = shapes[i]->getTrans()[3];
            glm::vec3 movement = point_new_position - glm::vec3(point_old_position);
            shapes[i]->MyTranslate(movement, 0);
        }
        else {
            shapes[i]->Hide();
        }
    }

    last_point_index = bezier_configs[config_num].size() - 1;
    UpdateCurveByShapes();
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
}

void Game::BuildAllShapes() {
    int shape_index = 0;
    first_point_index = shape_index;

    // Octahedrons
    for (int i = 0; i < number_of_octahedrons; i++) {
        AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
        SetShapeTex(shape_index, 0);
        shapes[shape_index]->MyScale(glm::vec3(0.5, 0.5, 0.5));
        shapes[shape_index]->Hide();
        shape_index++;
    }

    // Cube
    AddShape(Scene::Cube, -1, Scene::TRIANGLES);
    SetShapeTex(shape_index, 0);

    last_point_index = shape_index - 1;
    anim.cube_shape_index = shape_index;

    // Setting segments
    glm::vec4 zero_vector = glm::vec4(0, 0, 0, 0);
    bezier_1D->AddFirstSegment(zero_vector, zero_vector, zero_vector, zero_vector);

    for (int i = 4; i < number_of_octahedrons; i += 3)
        bezier_1D->AddSegment(zero_vector, zero_vector, zero_vector);

    // Configurations
    // Configurations
    std::vector<glm::vec3> config2;
    config2.resize(7);
    config2[0] = glm::vec3(-9, -9, 0);
    config2[1] = glm::vec3(-9, -3, 0);
    config2[2] = glm::vec3(-6, 0, 0);
    config2[3] = glm::vec3(0, 0, 0);
    config2[4] = glm::vec3(6, 0, 0);
    config2[5] = glm::vec3(9, -3, 0);
    config2[6] = glm::vec3(9, -9, 0);

    std::vector<glm::vec3> config3;
    config3.resize(10);
    config3[0] = glm::vec3(-15, -9, 0);
    config3[1] = glm::vec3(-15, -3, 0);
    config3[2] = glm::vec3(-12, 0, 0);
    config3[3] = glm::vec3(-6, 0, 0);
    config3[4] = glm::vec3(-3, 0, 0);
    config3[5] = glm::vec3(3, 0, 0);
    config3[6] = glm::vec3(6, 0, 0);
    config3[7] = glm::vec3(12, 0, 0);
    config3[8] = glm::vec3(15, -3, 0);
    config3[9] = glm::vec3(15, -9, 0);

    std::vector<glm::vec3> config4;
    config4.resize(13);
    config4[0] = glm::vec3(-21, -9, 0);
    config4[1] = glm::vec3(-21, -3, 0);
    config4[2] = glm::vec3(-18, 0, 0);
    config4[3] = glm::vec3(-12, 0, 0);
    config4[4] = glm::vec3(-9, 0, 0);
    config4[5] = glm::vec3(-3, 0, 0);
    config4[6] = glm::vec3(0, 0, 0);
    config4[7] = glm::vec3(3, 0, 0);
    config4[8] = glm::vec3(9, 0, 0);
    config4[9] = glm::vec3(12, 0, 0);
    config4[10] = glm::vec3(18, 0, 0);
    config4[11] = glm::vec3(21, -3, 0);
    config4[12] = glm::vec3(21, -9, 0);

    std::vector<glm::vec3> config5;
    config5.resize(16);
    config5[0] = glm::vec3(-27, -9, 0);
    config5[1] = glm::vec3(-27, -3, 0);
    config5[2] = glm::vec3(-24, 0, 0);
    config5[3] = glm::vec3(-18, 0, 0);
    config5[4] = glm::vec3(-15, 0, 0);
    config5[5] = glm::vec3(-9, 0, 0);
    config5[6] = glm::vec3(-6, 0, 0);
    config5[7] = glm::vec3(-3, 0, 0);
    config5[8] = glm::vec3(3, 0, 0);
    config5[9] = glm::vec3(6, 0, 0);
    config5[10] = glm::vec3(9, 0, 0);
    config5[11] = glm::vec3(15, 0, 0);
    config5[12] = glm::vec3(18, 0, 0);
    config5[13] = glm::vec3(24, 0, 0);
    config5[14] = glm::vec3(27, -3, 0);
    config5[15] = glm::vec3(27, -9, 0);

    std::vector<glm::vec3> config6;
    config6.resize(19);
    config6[0] = glm::vec3(-33, -9, 0);
    config6[1] = glm::vec3(-33, -3, 0);
    config6[2] = glm::vec3(-30, 0, 0);

    config6[3] = glm::vec3(-24, 0, 0);
    config6[4] = glm::vec3(-21, 0, 0);
    config6[5] = glm::vec3(-15, 0, 0);

    config6[6] = glm::vec3(-12, 0, 0);
    config6[7] = glm::vec3(-9, 0, 0);
    config6[8] = glm::vec3(-3, 0, 0);
    config6[9] = glm::vec3(0, 0, 0);
    config6[10] = glm::vec3(3, 0, 0);
    config6[11] = glm::vec3(9, 0, 0);
    config6[12] = glm::vec3(12, 0, 0);

    config6[13] = glm::vec3(15, 0, 0);
    config6[14] = glm::vec3(21, 0, 0);
    config6[15] = glm::vec3(24, 0, 0);

    config6[16] = glm::vec3(30, 0, 0);
    config6[17] = glm::vec3(33, -3, 0);
    config6[18] = glm::vec3(33, -9, 0);

    // Add all configs
    bezier_configs.push_back(config2);
    bezier_configs.push_back(config3);
    bezier_configs.push_back(config4);
    bezier_configs.push_back(config5);
    bezier_configs.push_back(config6);
}