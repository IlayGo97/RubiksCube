#ifndef GAME_RUBIK_H
#define GAME_RUBIK_H


#include "game.h"
#include "block.h"
using namespace std;
class rubik {
public:
    rubik(int i, Scene* scene);
private:
    vector<vector<block>> x;
    vector<vector<block>> y;
    vector<vector<block>> z;
};


#endif //GAME_RUBIK_H
