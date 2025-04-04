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
    ~Graphics();

    void init();
    void render_menu();
    void update_board(const Board& board);
    void display_result(const std::string& result_text);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* menu_texture;
    SDL_Texture* side_panel;
    SDL_Texture* piece_textures[12];

};

#endif
