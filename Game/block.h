#ifndef GAME_BLOCK_H
#define GAME_BLOCK_H


#include <tuple>
#include "glm/vec3.hpp"
#include "scene.h"

struct rotation_animation {
    bool isAnimation;
    float curr_Degree;
    float degree;
    glm::vec3 axis;
    Scene* scene;
};

class block : public Shape{
public:

    block(int i, glm::vec3 tvec31, Scene::shape shape1, Scene::modes modes1);

    glm::vec3 get_position();
    void set_position(glm::vec3 npos);
    void rotate_around_axis_clockwise(glm::vec3 axis);
    void continue_rotation_animation();
private:
    int indx;
public:
    int getIndx() const;

    void Draw(const std::vector<Shader *> shaders, const std::vector<Texture *> textures, bool isPicking) override;

private:
    glm::vec3 position;
    rotation_animation anim;
};


#endif //GAME_BLOCK_H
