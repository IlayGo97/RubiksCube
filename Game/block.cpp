#include "block.h"

block::block(int index, glm::vec3 pos, Scene *scene) {
    scene->AddShape(scene->Cube,-1,scene->TRIANGLES);
    scene->AddTexture("../res/textures/plane.png",false);
    scene->SetShapeTex(index, index);
    scene->shapes[index]->MyScale(glm::vec3(0.5,0.5,0.5));
    scene->shapes[index]->MyTranslate(pos, 0);
}
