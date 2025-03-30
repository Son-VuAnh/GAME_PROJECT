#ifndef RULES_H
#define RULES_H

#include "Moves.h"

class Board;

class Rules
{
public:
    static bool in_bounds(int x, int y); // helper

    static bool is_legal_move(const Board& board, const Move& move_);
    static bool square_attacked(const Board& board, int x, int y, Color attacker);
    static bool king_checked(const Board& board, Color color);
    static bool has_legal_moves(const Board& board, Color color);
    static bool can_castle(const Board& board, Color color, bool king_side);
    static bool can_enpassant(const Board& board, const Move& move_);
    // Game ending conditions
    static bool is_checkmate(const Board& board, Color color);
    static bool is_stalemate(const Board& board, Color color);
    static bool insufficient_material(const Board& board);
};

#endif
