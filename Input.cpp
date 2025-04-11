#include "Input.h"
#include "Game.h"

using namespace std;

const int SQUARE_SIZE = 75;
const int BOARD_SIZE = 600;

// Menu buttons positions
const int BUTTON_HEIGHT_IN_MENU = 35;

const int PLAY_BUTTON_X = 345;
const int PLAY_BUTTON_Y = 295;
const int PLAY_BUTTON_WIDTH = 112;

const int ABOUT_BUTTON_X = 315;
const int ABOUT_BUTTON_Y = 370;
const int ABOUT_BUTTON_WIDTH = 167;

const int QUIT_BUTTON_X = 342;
const int QUIT_BUTTON_Y = 446;
const int QUIT_BUTTON_WIDTH = 121;

// About button position
const int ABOUT_CLOSE_X = 642;
const int ABOUT_CLOSE_Y = 90;
const int ABOUT_CLOSE_SIZE = 27;

//Side panel button positions 10,78
const int BUTTON_WIDTH_IN_PANEL = 177;
const int BUTTON_HEIGHT_IN_PANEL = 65;
const int BUTTON_X_IN_PANEL = 610;

const int MENU_BUTTON_Y_IN_PANEL = 78;
const int QUIT_BUTTON_Y_IN_PANEL = 172;
const int RESET_BUTTON_Y_IN_PANEL = 266;


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

        if (PLAY_BUTTON_X <= x && x <= PLAY_BUTTON_X + PLAY_BUTTON_WIDTH &&
            PLAY_BUTTON_Y <= y && y <= PLAY_BUTTON_Y + BUTTON_HEIGHT_IN_MENU)
        {
            sound.play_button_sound();
            game.game_state = INGAME;
        }
        else if (ABOUT_BUTTON_X <= x && x <= ABOUT_BUTTON_X + ABOUT_BUTTON_WIDTH &&
                ABOUT_BUTTON_Y <= y && y <= ABOUT_BUTTON_Y + BUTTON_HEIGHT_IN_MENU)
        {
            sound.play_button_sound();
            game.game_state = ABOUT;
        }
        else if (QUIT_BUTTON_X <= x && x <= QUIT_BUTTON_X + QUIT_BUTTON_WIDTH &&
                QUIT_BUTTON_Y <= y && y <= QUIT_BUTTON_Y + BUTTON_HEIGHT_IN_MENU)
        {
            sound.play_button_sound();
            game.quit();
        }
    }
}

void Input::handle_about_input(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        int x = event.button.x;
        int y = event.button.y;

        if (ABOUT_CLOSE_X <= x && x <= ABOUT_CLOSE_X + ABOUT_CLOSE_SIZE &&
            ABOUT_CLOSE_Y <= y && y <= ABOUT_CLOSE_Y + ABOUT_CLOSE_SIZE)
        {
            sound.play_button_sound();
            game.game_state = MENU;
        }
    }
}

void Input::handle_ingame_input(SDL_Event& event, Game& game, Board& board)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int x = event.button.x;
        int y = event.button.y;
        if (x <= BOARD_SIZE) // inside chessboard
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
        else // on side panel
        {
            if (BUTTON_X_IN_PANEL <= x && x <= BUTTON_X_IN_PANEL + BUTTON_WIDTH_IN_PANEL &&
                MENU_BUTTON_Y_IN_PANEL <= y && y <= MENU_BUTTON_Y_IN_PANEL + BUTTON_HEIGHT_IN_PANEL)
            {
                sound.play_button_sound();
                game.game_state = MENU;
                game.reset_game();
            }
            else if (BUTTON_X_IN_PANEL <= x && x <= BUTTON_X_IN_PANEL + BUTTON_WIDTH_IN_PANEL &&
                     QUIT_BUTTON_Y_IN_PANEL <= y && y <= QUIT_BUTTON_Y_IN_PANEL + BUTTON_HEIGHT_IN_PANEL)
            {
                sound.play_button_sound();
                game.quit();
            }
            else if (BUTTON_X_IN_PANEL <= x && x <= BUTTON_X_IN_PANEL + BUTTON_WIDTH_IN_PANEL &&
                     RESET_BUTTON_Y_IN_PANEL <= y && y <= RESET_BUTTON_Y_IN_PANEL + BUTTON_HEIGHT_IN_PANEL)
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

        if (Rules::in_bounds(square_x, square_y))
        {
            Piece moved_piece = board.check_piece_at(selected_x, selected_y);
            Piece captured_piece = board.check_piece_at(square_x, square_y);

            Move move_(selected_x, selected_y, square_x, square_y, moved_piece, captured_piece, QUEEN);

            if (Rules::is_legal_move(board, move_))
            {
                sound.play_move_sound();
                board.make_move(move_);
                game.request_update();
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
