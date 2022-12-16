#ifndef GAME_RUBIK_H
#define GAME_RUBIK_H


#include <vector>
#include "block.h"
#include "game.h"

using namespace std;
class rubik {
public:
    rubik(int i, Game* scene);
private:
    vector<vector<block>> x;
    vector<vector<block>> y;
    vector<vector<block>> z;
};


#endif //GAME_RUBIK_H
