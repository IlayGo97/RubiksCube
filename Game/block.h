#ifndef GAME_BLOCK_H
#define GAME_BLOCK_H


#include <tuple>
#include "glm/vec3.hpp"
#include "scene.h"

class block {
public:
    block(int index, glm::vec3 pos, Scene* scene);
    int indx;
    glm::vec3 position;
};


#endif //GAME_BLOCK_H
