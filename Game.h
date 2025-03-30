#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Graphics.h"
#include "Input.h"
#include "Rules.h"
#include <string>

class Game
{
public:
    Game();

    bool init();
    Color get_current_turn() const;
    void switch_turn();
    void quit();
    bool is_running() const;
    void run();

private:
    Graphics graphics;
    Color current_turn;
    Board board;
    bool running;
    bool game_over;
    std::string result_text;
};

#endif
