#ifndef GAME_RUBIK_H
#define GAME_RUBIK_H


#include "game.h"
#include "block.h"
#include <map>
using namespace std;
class rubik {
public:
    rubik(int i, Scene* scene);
private:
    map<double, vector<block*>> x;
    map<double, vector<block*>> y;
    map<double, vector<block*>> z;
};


#endif //GAME_RUBIK_H
