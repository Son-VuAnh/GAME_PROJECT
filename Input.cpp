#include "Input.h"
#include "Game.h"

using namespace std;

const int SQUARE_SIZE = 75;

Input::Input(Game& game): game(game), dragging(false), selected_x(-1), selected_y(-1)
{
    sound.init();
}

Input::~Input() {}

void Input::handle_menu_input(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        int x = event.button.x;
        int y = event.button.y;

        if (x >= 330 && x <= 470 && y >= 290 && y <= 340) //PLAY button
        {
            sound.play_button_sound();
            game.game_state = INGAME;
        }
        else if (x >= 330 && x <= 470 && y >= 370 && y <= 420) //QUIT button
        {
            sound.play_button_sound();
            game.quit();
        }
    }
}

void Input::handle_ingame_input(SDL_Event& event, Game& game, Board& board)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int x = event.button.x;
        int y = event.button.y;
        if (x <= 600) // chessboard
        {
            int square_x = event.button.x / SQUARE_SIZE;
            int square_y = event.button.y / SQUARE_SIZE;
            Piece clicked_piece = board.check_piece_at(square_x, square_y);

            if (clicked_piece.type != EMPTY && clicked_piece.color == game.get_current_turn())
            {
                selected_x = square_x;
                selected_y = square_y;
                dragging = true;
            }
        }
        else //side panel
        {
            if (x >= 618 && x <= 780 && y >= 40 && y <= 89) //MENU button
            {
                sound.play_button_sound();
                game.game_state = MENU;
                game.reset_game();
            }
            else if (x >= 618 && x <= 780 && y >= 132 && y <= 185) //QUIT button
            {
                sound.play_button_sound();
                game.quit();
            }
            else if (x >= 618 && x <= 780 && y >= 227 && y <= 276) //RESET button
            {
                sound.play_button_sound();
                game.reset_game();
            }
        }
    }

    if (event.type == SDL_MOUSEBUTTONUP)
    {
        int square_x = event.button.x / SQUARE_SIZE;
        int square_y = event.button.y / SQUARE_SIZE;

        if (square_x >= 0 && square_x <= 7 && square_y >= 0 && square_y <= 7)
        {
            Piece moved_piece = board.check_piece_at(selected_x, selected_y);
            Piece captured_piece = board.check_piece_at(square_x, square_y);

            Move move_(selected_x, selected_y, square_x, square_y, moved_piece, captured_piece, QUEEN);

            if (Rules::is_legal_move(board, move_))
            {
                sound.play_move_sound();
                board.make_move(move_);
                game.switch_turn();
            }
            else
            {
                sound.play_illegal_move_sound();
            }
        }

        dragging = false;
        selected_x = -1;
        selected_y = -1;
    }
}
