#ifndef PIECE_H
#define PIECE_H

enum Color {NONE, WHITE, BLACK};
enum PieceType {EMPTY, KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN};

class Piece
{
public:
    Color color;
    PieceType type;
    Piece(PieceType _type = EMPTY, Color _color = NONE)
    {
        type = _type;
        color = _color;
    }
};

#endif
