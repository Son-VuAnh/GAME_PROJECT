#include "Moves.h"

Move::Move(int cur_x, int cur_y, int to_x, int to_y,
           Piece p_moved, Piece p_captured = {EMPTY,NONE}, PieceType promote_to = {QUEEN})
         : start_x(cur_x), start_y(cur_y), end_x(to_x), end_y(to_y),
           piece_moved(p_moved), piece_captured(p_captured), promotion(promote_to) {}
