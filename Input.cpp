#include "Input.h"
#include "Game.h"

using namespace std;

const int SQUARE_SIZE = 75;

Input::Input(Game& game): game(game), dragging(false), selected_x(-1), selected_y(-1) {}

void Input::handle_input(SDL_Event& event, Game& game, Board& board)
{
    if (event.type == SDL_QUIT)
    {
        game.quit();
        return;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int x = event.button.x / SQUARE_SIZE;
        int y = event.button.y / SQUARE_SIZE;
        Piece clicked_piece = board.check_piece_at(x, y);

        if (clicked_piece.type != EMPTY && clicked_piece.color == game.get_current_turn())
        {
            selected_x = x;
            selected_y = y;
            dragging = true;
        }
    }

    if (event.type == SDL_MOUSEBUTTONUP)
    {
        int x = event.button.x / SQUARE_SIZE;
        int y = event.button.y / SQUARE_SIZE;

        if (x >= 0 && x <= 7 && y >= 0 && y <= 7)
        {
            Piece moved_piece = board.check_piece_at(selected_x, selected_y);
            Piece captured_piece = board.check_piece_at(x, y);

            Move move_(selected_x, selected_y, x, y, moved_piece, captured_piece, QUEEN);

            if (Rules::is_legal_move(board, move_))
            {
                board.make_move(move_);
                game.switch_turn();
            }
        }

        dragging = false;
        selected_x = -1;
        selected_y = -1;
    }
}
