#ifndef __GAME_H__
#define __GAME_H__
#include "TextConsole.h"
struct block{
    int shape;
    std::vector<std::pair<int,int>> coords;
};

class MyGame: public CClient {
public:
    MyGame(TextConsole& con): CClient(con) {}

    void run() override;
    int genRandom();
    void getBlock(int random, block& blok);
    void printBlock();
    void printNextBlock(int randomNumber);
    bool isBlockValid(const block &b);
    void rotate(block &b);
    void clearBlock(const block &b);
    void clearNextBlock();
    void falling();
    void done(int puntos, int lineas);
    std::vector<std::pair<int,int>> valoresTomados;
};

#endif