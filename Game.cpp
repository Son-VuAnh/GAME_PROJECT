#include "Game.h"
#include <string>
using namespace std;

Game::Game() : game_state(MENU), current_turn(WHITE), running(true), game_over(false), result_text("")
{
    graphics.init();
    sound.init();
}

Game::~Game() {}

void Game::reset_game()
{
    board.reset();
    current_turn = WHITE;
    running = true;
    game_over = false;
    result_text = "";
}

Color Game::get_current_turn() const
{
    return current_turn;
}

void Game::switch_turn()
{
    current_turn = (current_turn == WHITE)? BLACK : WHITE;
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

            while (SDL_PollEvent(&event))
            {
                input.handle_menu_input(event);
            }
        }
        else
        {
            while (SDL_PollEvent(&event))
            {
                input.handle_ingame_input(event, *this, board);
            }

            if (!game_over) //Check endgame conditions
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

            if (game_over)
            {
                graphics.display_result(result_text);
                if (result_text.find("wins") != string::npos)
                {
                    sound.play_winning_sound();
                }
                else if (result_text.find("Draw") != string::npos)
                {
                    sound.play_drawing_sound();
                }

                //Stay still after finishing a game
                bool waiting = true;
                while (waiting)
                {
                    SDL_Event after_game;
                    while(SDL_PollEvent(&after_game))
                    {
                        if (after_game.type == SDL_MOUSEBUTTONDOWN && after_game.button.x > 600)
                        {
                            input.handle_ingame_input(after_game, *this, board);
                            waiting = false;
                        }
                    }
                }

            }

            SDL_Delay(10); // 1000/10 = 100 fps
        }
    }
}
