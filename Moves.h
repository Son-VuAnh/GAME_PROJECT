#ifndef MOVES_H
#define MOVES_H

#include "Piece.h"

class Move
{
public:
    int start_x, start_y, end_x, end_y;
    Piece piece_moved, piece_captured;
    PieceType promotion;

    Move(int start_x, int start_y, int end_x, int end_y,
         Piece piece_moved, Piece piece_captured, PieceType promotion);
};

#endif
