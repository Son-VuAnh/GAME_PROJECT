#include "Moves.h"
#include <math.h>

Move::Move(int cur_x, int cur_y, int to_x, int to_y,
         Piece p_moved, Piece p_captured = {EMPTY,NONE},
         bool castle = false, bool enpassant = false, PieceType promote_to = {EMPTY})
         : start_x(cur_x), start_y(cur_y), end_x(to_x), end_y(to_y),
         piece_moved(p_moved), piece_captured(p_captured),
         is_castling(castle), is_enpassant(enpassant), promotion(promote_to) {}

bool Move::is_capture()
{
    return piece_captured.type() != EMPTY;
}

bool Move::is_promotion()
{
    return promotion != EMPTY;
}

bool Move::is_castling()
{
    return piece_moved.type() == KING && abs(end_x - start_x) == 2;
}

bool Move::is_enpassant()
{
    return piece_moved.type() == PAWN && piece_captured.type() == PAWN && start_x != end_x;
}

bool Move::operator==(const Move& other) // Checking 3fold repetition
{
    return start_x == other.start_x && start_y == other.start_y &&
           end_x == other.end_x && end_y == other.end_y &&
           piece_moved == other.piece_moved;
}
