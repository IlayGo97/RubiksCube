#include <iostream>
#include "rubik.h"

rubik::rubik(int row, Scene* scene) {
    if(row % 2 == 1){
        max_row = row / 2;
        min_row = - row / 2;
    } else {
        max_row = row / 2 - 0.5f;
        min_row = - row / 2 + 0.5f;
    }
    for (float i = min_row; i <= max_row; i++) {
        for (float j = min_row; j <= max_row; j++) {
            for (float k = min_row; k <= max_row; k++) {
                int index = scene->AddBlock(glm::vec3(i,j,k));
                blocks.push_back(dynamic_cast<block *>(scene->shapes[index]));
            }
        }
    }
}

void rubik::setClockDirection(int clockDirection) {
    clock_direction = clockDirection;
}

bool check_relation(glm::vec3 point1, glm::vec3 axis){
    int pos;
    if(axis.x != 0)
        pos = 0;
    else if(axis.y != 0) {
        pos = 1;
    }
    else pos = 2;
    return point1[pos] - 0.1f <= axis[pos] && axis[pos] <= point1[pos] + 0.1f;
}

void rubik::some_wall_rotation(glm::vec3 axis) {
    for(block* b : blocks) { // to avoid 2 rotations at the same time so game doesn't break :(
        if(b->is_animation_going()){
            return;
        }
    }
    for(block* b : blocks){
        if(check_relation(b->get_position(), axis)){
            b->rotate_around_axis(axis, rotation_degree, clock_direction);
        }
    }
}

void rubik::flip_rotation() {
    rotation_degree = rotation_degree * -1;
}

void rubik::right_wall_rotation() {
    some_wall_rotation(glm::vec3(max_row,0.f,0.f));
}

void rubik::left_wall_rotation() {
    some_wall_rotation(glm::vec3(min_row,0.f,0.f));
}

void rubik::up_wall_rotation() {
    some_wall_rotation(glm::vec3(0.f,max_row,0.f));
}

void rubik::down_wall_rotation() {
    some_wall_rotation(glm::vec3(0.f,min_row,0.f));
}

void rubik::back_wall_rotation() {
    some_wall_rotation(glm::vec3(0.f,0.f,min_row));
}

void rubik::front_wall_rotation() {
    some_wall_rotation(glm::vec3(0.f,0.f,max_row));
}

void rubik::double_rotation_degree() {
    rotation_degree = std::min(rotation_degree * 2.f, 180.f);
}

void rubik::halve_rotation_degree() {
    rotation_degree = std::max(rotation_degree / 2.f, 1.f);
}