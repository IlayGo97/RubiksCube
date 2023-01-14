#include <iostream>
#include <thread>
#include "block.h"
#include "glm/gtc/matrix_transform.hpp"
# define M_PI           3.14159265358979323846f

block::block(int index, glm::vec3 pos, Scene::shape shapes, Scene::modes modes) : Shape(shapes, modes) {
    MyScale(glm::vec3(0.5,0.5,0.5));
    MyTranslate(pos, 0);
    indx = index;
    position = pos;
    anim.isAnimation = false;
}

glm::vec3 block::get_position() {
    return position;
}

glm::vec3 block::rotate_around_axis(glm::vec3 axis, float degree, int clockwise) {
    if (axis == position | anim.isAnimation)
        return position;
    anim.isAnimation = true;
    anim.degree = degree * (float)clockwise;
    anim.curr_Degree = 0.f;
    anim.axis = axis;
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), anim.degree, anim.axis);
    return glm::vec3(rotation_matrix * glm::vec4(get_position(),1.0f));
}

void block::set_position(glm::vec3 npos) {
    MyTranslate(npos - position, 0);
    position = npos;
}

int block::getIndx() const {
    return indx;
}

void block::continue_rotation_animation() {
    if (!anim.isAnimation)
        return;
    float framerate = 60.f * 2.f;
    float delta_degree = (float)1 / framerate * anim.degree;
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), delta_degree, anim.axis);
    glm::vec3 new_point = glm::vec3(rotation_matrix * glm::vec4(get_position(),1.0f));
    MyTranslate(new_point - get_position(), 0);
    MyRotate(delta_degree, anim.axis,0);
    set_position(new_point);
    anim.curr_Degree += delta_degree;
    anim.isAnimation = std::abs(anim.curr_Degree) <= std::abs(anim.degree);
}

void block::Draw(const std::vector<Shader *> shaders, const std::vector<Texture *> textures, bool isPicking) {
    Shape::Draw(shaders, textures, isPicking);
    continue_rotation_animation();
}

bool block::is_animation_going() {
    return anim.isAnimation;
}
