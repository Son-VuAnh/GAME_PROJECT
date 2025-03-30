#include "Rules.h"
#include "Board.h"
#include <math.h>

bool Rules::in_bounds(int x, int y)
{
    return x >= 0 && x <= 7 && y >= 0 && y <= 7;
}

bool Rules::square_attacked(const Board& board, int x, int y, Color attacker)
{
    // Check King
    for (int direction_x = -1; direction_x <= 1; direction_x++)
    {
        for (int direction_y = -1; direction_y <= 1; direction_y++)
        {
            if (direction_x == 0 && direction_y == 0) {continue;}

            int new_x = x + direction_x; int new_y = y + direction_y;
            if (in_bounds(new_x, new_y) && board.check_piece_at(new_x, new_y) == Piece(KING, attacker))
            {
                return true;
            }
        }
    }
    // Check Pawn
    int pawn_direction = (attacker == WHITE)? 1 : -1;
    if ((in_bounds(x - 1, y + pawn_direction) && board.check_piece_at(x - 1, y + pawn_direction) == Piece(PAWN, attacker)) ||
        (in_bounds(x + 1, y + pawn_direction) && board.check_piece_at(x + 1, y + pawn_direction) == Piece(PAWN, attacker)))
    {
        return true;
    }
    //Check Knight
    const int KNIGHT_offsets[8][2] = {{2,1}, {2,-1}, {-2,1}, {-2,-1}, {1,2}, {1,-2}, {-1,2}, {-1,-2}};
    for (int i = 0; i < 8; i++)
    {
        int new_x = x + KNIGHT_offsets[i][0];
        int new_y = y + KNIGHT_offsets[i][1];
        if (in_bounds(new_x, new_y) && board.check_piece_at(new_x, new_y) == Piece(KNIGHT, attacker))
        {
            return true;
        }
    }

    // Check Rook or Queen straight moves
    const int ROOK_offsets[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for (int i = 0; i < 4; i++)
    {
        int dx = ROOK_offsets[i][0];
        int new_x = x + dx;
        int dy = ROOK_offsets[i][1];
        int new_y = y + dy;
        while (in_bounds(new_x, new_y))
        {
            Piece piece = board.check_piece_at(new_x, new_y);
            if (piece.type != EMPTY)
            {
                if (piece.color == attacker && (piece.type == ROOK || piece.type == QUEEN))
                {
                    return true;
                }
                break;
            }
            new_x += dx;
            new_y += dy;
        }
    }

    // Check Bishop or Queen diag moves
    const int BISHOP_offsets[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for (int i = 0; i < 4; i++)
    {
        int dx = BISHOP_offsets[i][0];
        int new_x = x + dx;
        int dy = BISHOP_offsets[i][1];
        int new_y = y + dy;
        while (in_bounds(new_x, new_y))
        {
            Piece piece = board.check_piece_at(new_x, new_y);
            if (piece.type != EMPTY)
            {
                if (piece.color == attacker && (piece.type == BISHOP || piece.type == QUEEN))
                {
                    return true;
                }
                break;
            }
            new_x += dx;
            new_y += dy;
        }
    }

    return false;
}

bool Rules::king_checked(const Board& board, Color color)
{
    int king_x, king_y;
    if (color == WHITE)
    {
        king_x = board.white_king_x;
        king_y = board.white_king_y;
    }
    else
    {
        king_x = board.black_king_x;
        king_y = board.black_king_y;
    }

    Color attacker = (color == WHITE)? BLACK:WHITE;
    return square_attacked(board, king_x, king_y, attacker);
}

bool Rules::is_legal_move(const Board& board, const Move& move_)
{
    // 1.King safety
    Board temp_board = board;
    temp_board.board[move_.end_y][move_.end_x] = move_.piece_moved;
    temp_board.board[move_.start_y][move_.start_x] = Piece(EMPTY, NONE);

    bool king_in_check;
    Color attacker = (move_.piece_moved.color == WHITE)? BLACK : WHITE;

    if (move_.piece_moved.type == KING) king_in_check = square_attacked(temp_board, move_.end_x, move_.end_y, attacker);
    else
    {
        if (move_.piece_moved.color == WHITE) king_in_check = square_attacked(temp_board, temp_board.white_king_x, temp_board.white_king_y, attacker);
        else king_in_check = square_attacked(temp_board, temp_board.black_king_x, temp_board.black_king_y, attacker);
    }

    if (king_in_check) return false;

    // 2.Only capture piece of diff color
    if (move_.piece_moved.color == move_.piece_captured.color) return false;

    // 3.Moves of each piece type following chess rules
    int dist_x = move_.end_x - move_.start_x;
    int dist_y = move_.end_y - move_.start_y;
    if (dist_x == 0 && dist_y == 0) return false;

    int pawn_direction = (move_.piece_moved.color == WHITE)? -1 : 1;
    int start_rank = (move_.piece_moved.color == WHITE)? 6 : 1;
    int promotion_rank = (move_.piece_moved.color == WHITE)? 0 : 7;

    if (move_.piece_moved.type == KING)
    {
        if (abs(dist_x) <= 1 && abs(dist_y) <= 1) return true; //normal move

        if (dist_x == 2 && dist_y == 0 && Rules::can_castle(board, move_.piece_moved.color, true)) return true; //castling
        if (dist_x == -2 && dist_y == 0 && Rules::can_castle(board, move_.piece_moved.color, false)) return true;

        return false;
    }

    else if (move_.piece_moved.type == KNIGHT)
    {
        return ((abs(dist_x) == 2 && abs(dist_y) == 1) || (abs(dist_x) == 1 && abs(dist_y) == 2));
    }

    else if (move_.piece_moved.type == BISHOP)
    {
        int dir_x = (move_.end_x > move_.start_x) - (move_.end_x < move_.start_x);
        int dir_y = (move_.end_y > move_.start_y) - (move_.end_y < move_.start_y);
        int x = move_.start_x + dir_x;
        int y = move_.start_y + dir_y;

        while (x != move_.end_x || y != move_.end_y)
        {
            if (board.check_piece_at(x,y).type != EMPTY) return false;
            x += dir_x;
            y += dir_y;
        }

        return (abs(dist_x) == abs(dist_y)); //normal move
    }

    else if (move_.piece_moved.type == ROOK)
    {
        int dir_x = (move_.end_x > move_.start_x) - (move_.end_x < move_.start_x);
        int dir_y = (move_.end_y > move_.start_y) - (move_.end_y < move_.start_y);
        int x = move_.start_x + dir_x;
        int y = move_.start_y + dir_y;

        while (x != move_.end_x || y != move_.end_y)
        {
            if (board.check_piece_at(x,y).type != EMPTY) return false;
            x += dir_x;
            y += dir_y;
        }

        return (abs(dist_x) == 0 || abs(dist_y == 0));
    }

    else if (move_.piece_moved.type == QUEEN)
    {
        int dir_x = (move_.end_x > move_.start_x) - (move_.end_x < move_.start_x);
        int dir_y = (move_.end_y > move_.start_y) - (move_.end_y < move_.start_y);
        int x = move_.start_x + dir_x;
        int y = move_.start_y + dir_y;

        while (x != move_.end_x || y != move_.end_y)
        {
            if (board.check_piece_at(x,y).type != EMPTY) return false;
            x += dir_x;
            y += dir_y;
        }

        return (abs(dist_x) == abs(dist_y) || abs(dist_x) == 0 || abs(dist_y == 0));
    }

    else if (move_.piece_moved.type == PAWN)
    {
        if (dist_x == 0 && dist_y == pawn_direction && move_.end_y != promotion_rank &&
            board.check_piece_at(move_.end_x, move_.end_y).type == EMPTY) return true; //normal move

        if (abs(dist_x) == 1 && dist_y == pawn_direction &&
            board.check_piece_at(move_.end_x, move_.end_y).color != NONE) return true; //normal capture

        if (move_.end_y == promotion_rank && (dist_x == 0 || abs(dist_x) == 1)) return true; //promotion

        if (dist_x == 0 && move_.start_y == start_rank && dist_y == 2*pawn_direction &&
            board.check_piece_at(move_.start_x, move_.start_y + pawn_direction).type == EMPTY &&
            board.check_piece_at(move_.end_x, move_.end_y).type == EMPTY) return true; // double move at starting rank

        if (abs(dist_x) == 1 && dist_y == pawn_direction && can_enpassant(board, move_)) return true; // en passant

    }

    else return false;

    return false;
}

bool Rules::can_castle(const Board& board, Color color, bool king_side)
{
    if (king_checked(board, color)) return false;

    if (color == WHITE)
    {
        if (board.white_king_moved || board.white_rook_kingside_moved || board.white_rook_queenside_moved) return false;
    }
    else
    {
        if (board.black_king_moved || board.black_rook_kingside_moved || board.black_rook_queenside_moved) return false;
    }
    // Check if there are any pieces in between
    int y = (color == WHITE)? 7 : 0;
    int king_x = 4;
    int rook_x = (king_side)? 7 : 0;
    int direction = (king_side)? 1 : -1;
    for (int x = king_x + direction; x != rook_x; x += direction)
    {
        if (board.check_piece_at(x, y) != Piece(EMPTY,NONE)) return false;
    }

    //Check if any squares in between are attacked
    Color attacker = (color == WHITE)? BLACK:WHITE;
    for (int x = king_x + direction; x != king_x + 2*direction; x += direction)
    {
        if (square_attacked(board, x, y, attacker)) return false;
    }

    return true;
}

bool Rules::can_enpassant(const Board& board, const Move& move_)
{
    Move pre_move = board.last_move;
    if (pre_move.piece_moved.type != PAWN) return false;

    int opp_start_rank = (pre_move.piece_moved.color == WHITE)? 6 : 1;

    if (pre_move.start_y != opp_start_rank || abs(pre_move.end_y - pre_move.start_y) != 2) return false;

    if (move_.piece_moved.type == PAWN &&
        abs(move_.start_x - pre_move.end_x) == 1 &&
        move_.end_x == pre_move.end_x &&
        move_.start_y == pre_move.end_y &&
        move_.end_y == pre_move.end_y + ((move_.piece_moved.color == WHITE)? -1 : 1)) return true;

    return false;
}

bool Rules::has_legal_moves(const Board& board, Color color)
{
    for (int start_y = 0; start_y < 8; start_y++)
    {
        for (int start_x = 0; start_x < 8; start_x++)
        {
            Piece piece = board.check_piece_at(start_x, start_y);
            if (piece.color == color)
            {
                for (int end_y = 0; end_y < 8; end_y++)
                {
                    for (int end_x = 0; end_x < 8; end_x++)
                    {
                        Move move_(start_x, start_y, end_x, end_y, piece, board.check_piece_at(end_x, end_y), EMPTY);
                        if (is_legal_move(board, move_)) {return true;}
                    }
                }
            }
        }
    }

    return false;
}

bool Rules::is_checkmate(const Board& board, Color color)
{
    bool king_in_check = king_checked(board, color);
    bool no_legal_moves = !has_legal_moves(board, color);
    return king_in_check && no_legal_moves;
}

bool Rules::is_stalemate(const Board& board, Color color)
{
    bool king_in_check = king_checked(board, color);
    bool no_legal_moves = !has_legal_moves(board, color);
    return !king_in_check && no_legal_moves;
}

bool Rules::insufficient_material(const Board& board)
{
    int white_pieces = 0, white_minor_pieces = 0;
    int black_pieces = 0, black_minor_pieces = 0;

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Piece piece = board.check_piece_at(x, y);
            if (piece.color == NONE) continue;
            if (piece.color == WHITE)
            {
                white_pieces++;
                if (piece.type == BISHOP || piece.type == KNIGHT) white_minor_pieces++;
                if (white_minor_pieces > 1) return false;
            }
            else
            {
                black_pieces++;
                if (piece.type == BISHOP || piece.type == KNIGHT) black_minor_pieces++;
                if (black_minor_pieces > 1) return false;
            }
        }
    }

    if (white_pieces == 1 && black_pieces == 1) return true;
    if ((white_pieces == 2 && white_minor_pieces == 1 && black_pieces == 1) ||
        (black_pieces == 2 && black_minor_pieces == 1 && white_pieces == 1)) return true;

    return false;
}
