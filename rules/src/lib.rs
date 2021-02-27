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
    square: Vec<i8>,
    possible_moves: Vec<i8>,
}

impl Square {
    pub fn new() -> Square {
        Square {
            piece: Piece::Empty,
            colour: Colour::Empty,
            square: vec![0,0],
            possible_moves: vec![],
        }
    }

    pub fn set(&self, piece: Piece, colour: Colour, square: Vec<i8>  ){
        self.piece = piece;
        self.colour = colour;
        self.square = square;
    }

    // // I like getters and setters, sorry
    // pub fn move_piece(&mut self, new_square:i8){
    //     self.square = new_square;
    //     self.possible_moves = vec![];
    // }

    // pub fn update_moves(&mut self, moves: Vec<i8>){
    //     self.possible_moves = moves;

    // }
}

struct Board {
    // Just re-implementing old cpp code for now
    
    // 8x8 vector of square structs, each containing the piece info
    square: vec![vec![Square; 8]; 8],

    //fifty move counter for draws. Increments if no pawn move, check or
    //capture. If fifty_move_counter== 50, the game is a draw
    fifty_move_counter: i8,

    // game state, if true game is not ended
    game_state: bool,

    // result +1 for white win, 0 for draw, -1 for black win
    game_result: i8,

    // bool for if a player is in check, not really needed but nice
    // nice to display
    white_check: bool,
    black_check: bool,

    // if en_passent is non-empty then this pawn can be taken en-passent
    // this may change tbh
    en_passent: Square,

    // moves to prevent castling
    // this isn't just true/false because of pieces being in the way
    // /moving through a capture etc
    white_king_move: bool,
    black_king_move: bool,
    white_king_rook_move: bool,
    white_queen_rook_move: bool,
    black_king_rook_move: bool,
    black_queen_rook_move: bool,

}

impl Board{
    pub fn new()-> Board{
        Board{

        }
    }
}


