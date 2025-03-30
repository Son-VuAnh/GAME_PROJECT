#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Board.h"
#include <string>

class Graphics
{
public:
    Graphics();

    bool init();
    void load_textures();
    void update_board(const Board& board);
    void render();
    void display_result(const std::string& result_text);
    void clean_up();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* piece_textures[12];
};

#endif
