#ifndef GAME_H
#define GAME_H

#include "Graphics.h"
#include "Input.h"
#include "Rules.h"
#include <string>

enum GameState {MENU, ABOUT, INGAME};

class Game
{
public:
    Game();
    ~Game();

    GameState game_state;
    Color get_current_turn() const;
    void switch_turn();
    void request_update();

    void run();
    void reset_game();
    void quit();

private:
    Graphics graphics;
    Sound sound;
    Color current_turn;
    Board board;
    bool running;
    bool need_update;
    bool game_over;
    std::string result_text;
};

#endif
