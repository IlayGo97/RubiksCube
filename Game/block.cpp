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
}

glm::vec3 block::get_position() {
    return position;
}

void block::rotate_around_axis_clockwise(Scene *scene, glm::vec3 axis) {
    anim.isAnimation = true;
    anim.degree = 90.f;
    anim.curr_Degree = 0.f;
    anim.axis = axis;
    anim.scene = scene;
}

void block::set_position(glm::vec3 npos) {
    position = npos;
}

int block::getIndx() const {
    return indx;
}

void block::continue_rotation_animation() {
    if (!anim.isAnimation)
        return;
    float framerate = 60.f;
    float delta_degree = (float)5 / framerate * anim.degree;
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), delta_degree, anim.axis);
    glm::vec3 new_point = glm::vec3(rotation_matrix * glm::vec4(get_position(),1.0f));
    anim.scene->shapes[getIndx()]->MyTranslate(new_point - get_position(), 0);
    set_position(new_point);
    anim.curr_Degree += delta_degree;
    anim.isAnimation = anim.curr_Degree < anim.degree;
}
