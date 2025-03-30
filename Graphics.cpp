#include "Graphics.h"
#include <iostream>

using namespace std;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int SQUARE_SIZE = 75;

Graphics::Graphics() : window(nullptr), renderer(nullptr)
{
    for (int i = 0; i < 12; i++)
    {
        piece_textures[i] = nullptr;
    }
}

bool Graphics::init()
{
    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();

    load_textures();

    return true;
}

void Graphics::load_textures()
{
    const string piece_paths[12] = {
        "assets/black_pawn.png", "assets/black_knight.png", "assets/black_bishop.png", "assets/black_rook.png",
        "assets/black_queen.png", "assets/black_king.png", "assets/white_pawn.png", "assets/white_knight.png",
        "assets/white_bishop.png", "assets/white_rook.png", "assets/white_queen.png", "assets/white_king.png"
    };

    for (int i = 0; i < 12; i++)
    {
        SDL_Surface* surface = IMG_Load(piece_paths[i].c_str());
        piece_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void Graphics::update_board(const Board& board)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
    SDL_RenderClear(renderer);

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            SDL_Rect square = {x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            if ((x + y) % 2 == 0)
            {
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255); // beige, for white squares
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255); // brown, for black squares
            }
            SDL_RenderFillRect(renderer, &square);

            Piece piece = board.check_piece_at(x, y);
            if (piece.type != EMPTY)
            {
                int texture_index = ((piece.color == WHITE)? 6 : 0) + static_cast<int>(piece.type) - 1;
                SDL_Rect piece_rect = {x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
                SDL_RenderCopy(renderer, piece_textures[texture_index], nullptr, &piece_rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Graphics::render()
{
    SDL_RenderPresent(renderer);
}

void Graphics::display_result(const string& result_text)
{
    SDL_Color text_color = {75, 87, 219, 255}; // dark blue

    TTF_Font* font = TTF_OpenFont("assets/font/Cinzel-Bold.ttf", 30);

    SDL_Surface* text_surface = TTF_RenderText_Solid(font, result_text.c_str(), text_color);

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);

    int text_width, text_height;
    SDL_QueryTexture(text_texture, nullptr, nullptr, &text_width, &text_height);

    SDL_Rect text_rect = {(WINDOW_WIDTH - text_width)/2, (WINDOW_HEIGHT - text_height)/2, text_width, text_height};
    SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
}

void Graphics::clean_up()
{
    for (int i = 0; i < 12; i++)
    {
        if (piece_textures[i])
        {
            SDL_DestroyTexture(piece_textures[i]);
            piece_textures[i] = nullptr;
        }
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
