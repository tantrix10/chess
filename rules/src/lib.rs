enum Piece {
    King,
    Queen,
    Rook,
    Knight,
    Bishop,
    Pawn,
    Empty,
}
enum Colour {
    White,
    Black,
    Empty,
}

struct Square {
    piece: Piece,
    colour: Colour,
    square: i8,
    // this possible moves might change/move
    possible_moves: Vec<i8>,
}

impl Square {
    pub fn new(piece: Piece, colour: Colour, square: i8) -> Square {
        Square {
            piece: piece,
            colour: colour,
            square: square,
            possible_moves: vec![],
        }
    }

    // I like getters and setters, sorry
    pub fn move_piece(&mut self, new_square:i8){
        self.square = new_square;
        self.possible_moves = vec![];
    }

    pub fn update_moves(&mut self, moves: Vec<i8>){
        self.possible_moves = moves;

    }
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
    game_over: bool,
    move_number: i32,
    // A vector of vectors of dim , first square to second.
    // this rep might change
    // These are on pieces themselves atm, but capture pieces remain 
    // because it's easy to check checks etc
    // Perhaps it should be just King captures?
    // move_list: Vec<Vec<i8>>,
    // black_moves: Vec<Vec<i8>>,
    // white_moves: Vec<Vec<i8>>,
    black_capture_moves: Vec<Vec<i8>>,
    white_capture_moves: Vec<Vec<i8>>,
}

impl Board{
    pub fn new()-> Board{
        Board{
            
        }
    }
}

pub fn adds_hopefully(x: i32, y: i32) -> i32 {
    // Making sure tests work!
    x + y
}
