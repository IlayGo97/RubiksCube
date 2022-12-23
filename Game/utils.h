
#ifndef GAME_UTILS_H
#define GAME_UTILS_H


#include <iostream>
#include "glm/vec3.hpp"

class utils {

};

void print_vec3(glm::vec3 point){
    std::cout << point.x << ", " << point.y << ", " << point.z << "\n";
}

#endif //GAME_UTILS_H
