#include <iostream>
#include "rubik.h"

rubik::rubik(int row, Scene* scene): max_row(row / 2), min_row(-row / 2) {
    for (int i = -(row / 2); i <= row / 2; i++) {
        for (int j = -(row / 2); j <= row / 2; j++) {
            for (int k = -(row / 2); k <= row / 2; k++) {
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
    else if(axis.y != 0)
        pos = 1;
    else pos = 2;
    return (int)point1[pos] == (int)axis[pos];
}

void rubik::some_wall_rotation(glm::vec3 axis) {
    for(block* b : blocks){
        if(check_relation(axis, b->get_position())){
            b->rotate_around_axis(axis, rotation_degree, clock_direction);
        }
    }
}

void rubik::right_wall_rotation() {
    some_wall_rotation(glm::vec3(max_row,0.f,0.f));
}

void rubik::left_wall_rotation() {
    some_wall_rotation(glm::vec3(min_row,0.f,0.f));
}

void rubik::setRotationDegree(float rotationDegree) {
    rotation_degree = rotationDegree;
}
