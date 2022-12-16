#ifndef GAME_BLOCK_H
#define GAME_BLOCK_H


#include <tuple>

class block {
public:
    explicit block(int index, std::tuple<int, int, int> pos);
    int indx;
    std::tuple<int, int, int> position;
};


#endif //GAME_BLOCK_H
