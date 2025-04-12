#include "Game.h"
#include <string>
using namespace std;

const int BOARD_SIZE = 600;

Game::Game() : game_state(MENU), current_turn(WHITE), running(true), need_update(false), game_over(false), result_text("")
{
    graphics.init();
    sound.init();
}

Game::~Game() {}

Color Game::get_current_turn() const
{
    return current_turn;
}

void Game::switch_turn()
{
    current_turn = (current_turn == WHITE)? BLACK : WHITE;
}

void Game::request_update()
{
    need_update = true;
}

void Game::reset_game()
{
    board.reset();
    current_turn = WHITE;
    running = true;
    need_update = false;
    game_over = false;
    result_text = "";
}

void Game::quit()
{
    running = false;
}

void Game::run()
{
    Input input(*this);

    SDL_Event event;
    while (running)
    {
        if (game_state == MENU)
        {
            graphics.render_menu();
            graphics.display();

            while (SDL_PollEvent(&event))
            {
                input.handle_menu_input(event);
            }
        }

        else if (game_state == ABOUT)
        {
            graphics.render_about();
            graphics.display();

            while (SDL_PollEvent(&event))
            {
                input.handle_about_input(event);
            }
        }

        else // INGAME
        {
            while (SDL_PollEvent(&event))
            {
                input.handle_ingame_input(event, *this, board);
            }

            if (need_update) // after a legal move is played -> check game ending contidions
            {
                if (!game_over)
                {
                    Color opponent_turn = (current_turn == WHITE)? BLACK : WHITE;
                    if (Rules::is_checkmate(board, opponent_turn))
                    {
                        game_over = true;
                        result_text = (opponent_turn == WHITE)? "Black wins by checkmate!" : "White wins by checkmate!";
                        sound.play_winning_sound();
                    }
                    else if (Rules::is_stalemate(board, opponent_turn))
                    {
                        game_over = true;
                        result_text = "Draw by stalemate!";
                        sound.play_drawing_sound();
                    }
                    else if (Rules::insufficient_material(board))
                    {
                        game_over = true;
                        result_text = "Draw by insufficient material!";
                        sound.play_drawing_sound();
                    }

                    if (!game_over) switch_turn();
                }
                need_update = false;
            }

            // Display graphics
            graphics.render_board(board);

            if (game_over)
            {
                graphics.render_result(result_text);
            }
            else
            {
                if (current_turn == WHITE) graphics.render_white_turn_circle();
                else graphics.render_black_turn_circle();
            }

            graphics.display();

            // Stay still after finishing a game
            if (game_over)
            {
                bool waiting = true;
                while (waiting)
                {
                    SDL_Event after_game;
                    while (SDL_PollEvent(&after_game))
                    {
                        if (after_game.type == SDL_MOUSEBUTTONDOWN && after_game.button.x > BOARD_SIZE)
                        {
                            input.handle_ingame_input(after_game, *this, board);
                            waiting = false;
                        }
                    }
                }
            }
        }
    }
}
