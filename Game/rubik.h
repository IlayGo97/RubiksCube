#ifndef GAME_RUBIK_H
#define GAME_RUBIK_H


#include "game.h"
#include "block.h"
using namespace std;
class rubik {
public:
    rubik(int i, Scene* scene);
    void right_wall_rotation();
    void left_wall_rotation();

    void setClockDirection(int clockDirection);

    void setRotationDegree(float rotationDegree);

private:
    std::vector<block*> blocks;
    int max_row;
    int min_row;
    int clock_direction = 1;
    void some_wall_rotation(glm::vec3 axis);
    float rotation_degree = 90.f;
};


#endif //GAME_RUBIK_H
