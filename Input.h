#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <SDL_image.h>
#include "Board.h"
#include "Sound.h"

class Game;

class Input
{
public:
    Input(Game& game);
    ~Input();

    void handle_menu_input(SDL_Event& event);
    void handle_ingame_input(SDL_Event& event, Game& game, Board& board);

private:
    Game& game;
    bool dragging;
    int selected_x;
    int selected_y;
    Sound sound;
};

#endif
