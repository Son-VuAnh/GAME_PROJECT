#ifndef BOARD_H
#define BOARD_H

#include "Moves.h"

class Rules;

class Board
{
public:
    Board();

    void reset();
    Piece check_piece_at(int x, int y) const;
    void make_move(const Move& move_);

private:
    friend class Rules;

    static const Piece startingBoard[8][8];

    Piece board[8][8];

    bool white_king_moved;
    int white_king_x, white_king_y;
    int black_king_x, black_king_y;
    bool black_king_moved;
    bool white_rook_kingside_moved;
    bool white_rook_queenside_moved;
    bool black_rook_kingside_moved;
    bool black_rook_queenside_moved;

    Move last_move = Move(-1, -1, -1, -1, {EMPTY,NONE}, {EMPTY,NONE}, QUEEN);
};

#endif
