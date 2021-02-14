enum Piece {
    King,
    Queen,
    Rook,
    Knight,
    Bishop,
    Pawn,
}
enum Colour {
    White,
    Black,
}

struct Square {
    piece: Piece,
    colour: Colour,
    square: i8,

    // this possible moves might change/move
    possible_moves: (i8,i8),
}

struct Board {
    // Piece, piece colour and square 0-63
    // may wish to push this up from i8 is I want a huge board

    // Having a square struct too does allow for easy access to the pieces
    // Having said that, I can just store legal moves in a smarter way
    white_pieces: Vec<Square>,
    black_pieces: Vec<Square>,
    while_castle_king_side: bool,
    black_castle_king_side: bool,
    while_castle_queen_side: bool,
    black_castle_queen_side: bool,
    check: bool,
    en_passent: bool,
    fifty_move_count: i8,
}

pub fn adds_hopefully(x: i32, y: i32) -> i32 {
    // Making sure tests work!
    x + y
}
