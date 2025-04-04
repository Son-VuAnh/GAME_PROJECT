#include "Graphics.h"
using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BOARD_SIZE = 600; //75x8
const int SQUARE_SIZE = 75;

Graphics::Graphics() : window(nullptr), renderer(nullptr), menu_texture(nullptr), side_panel(nullptr)
{
    for (int i = 0; i < 12; i++)
    {
        piece_textures[i] = nullptr;
    }
}

void Graphics::init()
{
    window = SDL_CreateWindow("Chess 1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();

    //Load menu
    menu_texture = IMG_LoadTexture(renderer, "assets/image/Menu.png");

    //Load side panel
    side_panel = IMG_LoadTexture(renderer, "assets/image/side_panel.png");

    //Load pieces
    const string piece_paths[12] = {
        "assets/image/black_pawn.png", "assets/image/black_knight.png", "assets/image/black_bishop.png", "assets/image/black_rook.png",
        "assets/image/black_queen.png", "assets/image/black_king.png", "assets/image/white_pawn.png", "assets/image/white_knight.png",
        "assets/image/white_bishop.png", "assets/image/white_rook.png", "assets/image/white_queen.png", "assets/image/white_king.png"
    };

    for (int i = 0; i < 12; i++)
    {
        piece_textures[i] = IMG_LoadTexture(renderer, piece_paths[i].c_str());
    }
}

void Graphics::render_menu()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, menu_texture, nullptr, nullptr);

    SDL_RenderPresent(renderer);
}

void Graphics::update_board(const Board& board)
{
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

    SDL_Rect side_panel_rect = {600, 0, 200, 600};
    SDL_RenderCopy(renderer, side_panel, nullptr, &side_panel_rect);

    SDL_RenderPresent(renderer);
}

void Graphics::display_result(const string& result_text)
{
    SDL_Color text_color = {0, 94, 225, 255}; // dark blue

    TTF_Font* font = TTF_OpenFont("assets/font/Cinzel-Bold.ttf", 32);

    SDL_Surface* text_surface = TTF_RenderText_Solid(font, result_text.c_str(), text_color);

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);

    int text_width, text_height;
    SDL_QueryTexture(text_texture, nullptr, nullptr, &text_width, &text_height);

    SDL_Rect text_rect = {(BOARD_SIZE - text_width)/2, (BOARD_SIZE - text_height)/2, text_width, text_height};
    SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
}

Graphics::~Graphics()
{
    if (menu_texture)
    {
        SDL_DestroyTexture(menu_texture);
        menu_texture = nullptr;
    }

    for (int i = 0; i < 12; i++)
    {
        if (piece_textures[i])
        {
            SDL_DestroyTexture(piece_textures[i]);
            piece_textures[i] = nullptr;
        }
    }

    if (side_panel)
    {
        SDL_DestroyTexture(side_panel);
        side_panel = nullptr;
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
