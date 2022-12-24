#ifndef GAME_RUBIK_H
#define GAME_RUBIK_H


#include "scene.h"
#include "block.h"
using namespace std;
class rubik {
public:
    rubik(int i, Scene* scene);
    void right_wall_rotation();
    void left_wall_rotation();
    void up_wall_rotation();
    void down_wall_rotation();
    void back_wall_rotation();
    void front_wall_rotation();

    void double_rotation_degree();
    void halve_rotation_degree();
    void setClockDirection(int clockDirection);
    void flip_rotation();

private:
    std::vector<block*> blocks;
    float max_row;
    float min_row;
    int clock_direction = 1;
    void some_wall_rotation(glm::vec3 axis);
    float rotation_degree = 90.f;
};


#endif //GAME_RUBIK_H
