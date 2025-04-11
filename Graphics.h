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
    void display();
    void render_menu();
    void render_about();
    void render_board(const Board& board);
    void render_white_turn_circle();
    void render_black_turn_circle();
    void render_result(const std::string& result_text);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* menu_texture;
    SDL_Texture* about_texture;
    SDL_Texture* side_panel;
    SDL_Texture* white_turn_circle;
    SDL_Texture* black_turn_circle;
    SDL_Texture* piece_textures[12];
};

#endif
