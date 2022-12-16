#include "rubik.h"

rubik::rubik(int row, Game* scene) : x(vector<vector<block>>()), y(vector<vector<block>>()), z(vector<vector<block>>()) {
    for (int i = 0; i < row; i ++){
        x.emplace_back();
        y.emplace_back();
        z.emplace_back();
    }
    int indx = 0;
    for (int i = -(row / 2); i <= row / 2; i++)
        for (int j = -(row / 2); j <= row / 2; j++)
            for (int k = -(row / 2); k <= row / 2; k++) {
                block b = block(indx, std::tuple<int,int, int>(i,j,k));

                indx ++;
            }
}
