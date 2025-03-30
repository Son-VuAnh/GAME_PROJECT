#ifndef PIECE_H
#define PIECE_H

enum Color {NONE, WHITE, BLACK};
enum PieceType {EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

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

    bool operator == (const Piece& other) const
    {
        return (type == other.type) && (color == other.color);
    }

    bool operator != (const Piece& other) const
    {
        return !(type == other.type) || !(color == other.color);
    }
};

#endif
