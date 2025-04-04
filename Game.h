#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Graphics.h"
#include "Input.h"
#include "Rules.h"
#include <string>

enum GameState {MENU, INGAME};

class Game
{
public:
    Game();
    ~Game();

    Color get_current_turn() const;
    void switch_turn();

    void run();
    void reset_game();
    void quit();

    GameState game_state;

private:
    Graphics graphics;
    Sound sound;
    Color current_turn;
    Board board;
    bool running;
    bool game_over;
    std::string result_text;
};

#endif
