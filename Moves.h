#ifndef MOVES_H
#define MOVES_H

#include "Piece.h"

class Move
{
    int start_x, start_y, end_x, end_y;
    Piece piece_moved, piece_captured;
    bool is_castling, is_enpassant;
    PieceType promotion;

    Move(int start_x, int start_y, int end_x, int end_y,
         Piece piece_moved, Piece piece_capture = Piece(EMPTY,NONE), PieceType promotion = EMPTY);

    bool is_capture();
    bool is_castling();
    bool is_enpassant();
    bool is_promotion();
    bool Move::operator==(const Move& other);
};

#endif
