#include "Board.h"
#include "Rules.h"
#include <math.h>

const Piece Board::startingBoard[8][8] = {
        {{ROOK,BLACK}, {KNIGHT,BLACK}, {BISHOP,BLACK}, {QUEEN,BLACK}, {KING,BLACK}, {BISHOP,BLACK}, {KNIGHT,BLACK}, {ROOK,BLACK}},
        {{PAWN,BLACK}, {PAWN,BLACK}, {PAWN,BLACK}, {PAWN,BLACK}, {PAWN,BLACK}, {PAWN,BLACK}, {PAWN,BLACK}, {PAWN,BLACK}},
        {{EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}},
        {{EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}},
        {{EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}},
        {{EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}, {EMPTY,NONE}},
        {{PAWN,WHITE}, {PAWN,WHITE}, {PAWN,WHITE}, {PAWN,WHITE}, {PAWN,WHITE}, {PAWN,WHITE}, {PAWN,WHITE}, {PAWN,WHITE}},
        {{ROOK,WHITE}, {KNIGHT,WHITE}, {BISHOP,WHITE}, {QUEEN,WHITE}, {KING,WHITE}, {BISHOP,WHITE}, {KNIGHT,WHITE}, {ROOK,WHITE}}
};

Board::Board()
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            board[y][x] = startingBoard[y][x];
        }
    }

    white_king_moved = black_king_moved = false;
    white_king_x = 4; white_king_y = 7;
    black_king_x = 4; black_king_y = 0;

    white_rook_kingside_moved = white_rook_queenside_moved = false;
    black_rook_kingside_moved = black_rook_queenside_moved = false;

    last_move = Move(-1, -1, -1, -1, {EMPTY,NONE}, {EMPTY,NONE}, QUEEN);
}

Piece Board::check_piece_at(int x, int y) const
{
    return board[y][x];
}

void Board::make_move(const Move& move_)
{
    // Special moves first
    if (move_.piece_moved.type == KING && abs(move_.end_x - move_.start_x) == 2
        && Rules::can_castle(*this, move_.piece_moved.color, move_.end_x > move_.start_x))
    { // only need to move rook in this
        int rook_x = (move_.end_x > move_.start_x)? 7:0 ;
        int new_rook_x = (move_.end_x > move_.start_x)? 5:3 ;

        board[move_.start_y][new_rook_x] = board[move_.end_y][rook_x];
        board[move_.start_y][rook_x] = {EMPTY,NONE};
    }

    if (move_.piece_moved.type == PAWN && Rules::can_enpassant(*this, move_)) //en passant
    { //only need to remove the captured pawn here
        Move pre_move = last_move;
        board[pre_move.end_y][pre_move.end_x] = {EMPTY, NONE};
    }

    board[move_.end_y][move_.end_x] = move_.piece_moved;
    board[move_.start_y][move_.start_x] = {EMPTY,NONE};

    if (move_.piece_moved.type == PAWN && (move_.end_y == 0 || move_.end_y == 7))
    {
        board[move_.end_y][move_.end_x].type = QUEEN;
        board[move_.end_y][move_.end_x].color = move_.piece_moved.color;
        board[move_.start_y][move_.start_x] = {EMPTY,NONE};
    }
    // Toggle king and rook's flags
    if (move_.piece_moved.type == KING)
    {
        if (move_.piece_moved.color == WHITE)
        {
            white_king_x = move_.end_x;
            white_king_y = move_.end_y;
            white_king_moved = true;
        }
        else
        {
            black_king_x = move_.end_x;
            black_king_y = move_.end_y;
            black_king_moved = true;
        }
    }

    if (move_.piece_moved.type == ROOK) {
        if (move_.piece_moved.color == WHITE)
        {
            if (move_.start_x == 7 && move_.start_y == 7) white_rook_kingside_moved = true;
            if (move_.start_x == 0 && move_.start_y == 7) white_rook_queenside_moved = true;
        }
        else
        {
            if (move_.start_x == 7 && move_.start_y == 0) black_rook_kingside_moved = true;
            if (move_.start_x == 0 && move_.start_y == 0) black_rook_queenside_moved = true;
        }
    }
    //Record last move
    last_move = move_;
}
