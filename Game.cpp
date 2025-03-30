#include "Game.h"

Game::Game() : current_turn(WHITE), running(true), game_over(false), result_text("") {}

bool Game::init()
{
    return graphics.init();
}

Color Game::get_current_turn() const
{
    return current_turn;
}

void Game::switch_turn()
{
    current_turn = (current_turn == WHITE) ? BLACK : WHITE;
}

void Game::quit()
{
    running = false;
}

bool Game::is_running() const
{
    return running;
}

void Game::run()
{
    Input input(*this);
    graphics.init();
    graphics.load_textures();

    while (is_running())
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            input.handle_input(event, *this, board);
        }

        if (!game_over)
        {
            if (Rules::is_checkmate(board, current_turn))
            {
                game_over = true;
                result_text = (current_turn == WHITE)? "Black wins by checkmate!" : "White wins by checkmate!";
            }
            else if (Rules::is_stalemate(board, current_turn))
            {
                game_over = true;
                result_text = "Draw by stalemate!";
            }
            else if (Rules::insufficient_material(board))
            {
                game_over = true;
                result_text = "Draw by insufficient material!";
            }
        }

        graphics.update_board(board);
        graphics.render();

        if (game_over)
        {
            graphics.display_result(result_text);
            SDL_Delay(3000);
            quit();
        }

        SDL_Delay(16);
    }

    graphics.clean_up();
}
