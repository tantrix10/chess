
#[derive(Clone)]
enum Piece {
    King,
    Queen,
    Rook,
    Knight,
    Bishop,
    Pawn,
    Empty,
}
#[derive(Clone)]
enum Colour {
    White,
    Black,
    Empty,
}

#[derive(Clone)]
struct Square {
    piece: Piece,
    colour: Colour,
    square: Vec<i8>,
    possible_moves: Vec<[i8;2]>,
}

impl Square {
    pub fn new() -> Square {
        Square {
            piece: Piece::Empty,
            colour: Colour::Empty,
            // this is a bit hacky, should be an array
            square: vec![0,0],
            possible_moves: vec![],
        }
    }

    pub fn set(&mut self, piece: Piece, colour: Colour, x: i8, y: i8  ){
        self.piece = piece;
        self.colour = colour;
        self.square = vec![x,y];
    }

    pub fn set_empty(&mut self) {
           self.piece = Piece::Empty;
           self.colour = Colour::Empty;
           self.possible_moves = vec![];
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

pub struct Board {
    // Just re-implementing old cpp code for now
    
    // 8x8 vector of square structs, each containing the piece info
    square: Vec<Vec<Square> >,

    //fifty move counter for draws. Increments if no pawn move, check or
    //capture. If fifty_move_counter== 50, the game is a draw
    fifty_move_counter: i8,

    // game state, if true game is not ended
    game_state: bool,

    // result +1 for white win, 0 for draw, -1 for black win
    game_result: i8,

    // bool for if a player is in check, not really needed but nice
    // nice to display
    check: bool,

    // if en_passent is non-empty then this pawn can be taken en-passent
    // this may change tbh
    en_passent: (Square, bool),

    // moves to prevent castling
    // this isn't just true/false because of pieces being in the way
    // /moving through a capture etc
    white_king_move: bool,
    black_king_move: bool,
    white_king_rook_move: bool,
    white_queen_rook_move: bool,
    black_king_rook_move: bool,
    black_queen_rook_move: bool,

    // King positions
    white_king: String,
    black_king: String,

    // Vector of all capture moves to check for checks
    white_take_moves: Vec<[i8;2]>,
    black_take_moves: Vec<[i8;2]>,

    // List of all moves for checking legality and 
    // generating engine trees
    white_all_moves: Vec<[i8;2]>,
    black_all_moves: Vec<[i8;2]>,

}

impl Board{
    pub fn new()-> Board{

        let mut tmp: Vec<Square> = vec![];
        let mut sqrs: Vec<Vec<Square>> = vec![];
        
        for _i in 0..8{ 
            tmp = vec![];
            for _j in 0..8{
                tmp.push(Square::new())
            }
            sqrs.push(tmp.clone())
        }
        Board{
            square: sqrs,
            fifty_move_counter: 0,
            game_state: true,
            game_result: 2,
            check: false,
            en_passent: (Square::new(), false),

            white_king_move: false,
            black_king_move: false,
            white_king_rook_move: false,
            white_queen_rook_move: false,
            black_king_rook_move: false,
            black_queen_rook_move: false,
            white_king: String::from("e1"),
            black_king: String::from("e8"),

            white_take_moves: vec![],
            black_take_moves: vec![],

            white_all_moves: vec![],
            black_all_moves: vec![],

        }
    }

    pub fn set(&mut self){

        // set the pawns
        for i in 0..8
        {
            self.square[6][i].set(Piece::Pawn, Colour::Black, 6, i as i8);
            self.square[1][i].set(Piece::Pawn, Colour::White, 1, i as i8);
        }

        //set the empty squares
        for i in 2..6
        {
            for j in 0..8
            {
                self.square[i][j].set(Piece::Empty, Colour::Empty, i as i8, j as i8);
            }
        }

        // set the non-pawn and empty pieces 
        self.square[0][0].set(Piece::Rook   ,Colour::Black,0,0);
        self.square[0][1].set(Piece::Knight ,Colour::Black,0,1);
        self.square[0][2].set(Piece::Bishop ,Colour::Black,0,2);
        self.square[0][3].set(Piece::Queen  ,Colour::Black,0,3);
        self.square[0][4].set(Piece::King   ,Colour::Black,0,4);
        self.square[0][5].set(Piece::Bishop ,Colour::Black,0,5);
        self.square[0][6].set(Piece::Knight ,Colour::Black,0,6);
        self.square[0][7].set(Piece::Rook   ,Colour::Black,0,7);

        self.square[7][0].set(Piece::Rook   ,Colour::White,7,0);
        self.square[7][1].set(Piece::Knight ,Colour::White,7,1);
        self.square[7][2].set(Piece::Bishop ,Colour::White,7,2);
        self.square[7][3].set(Piece::Queen  ,Colour::White,7,3);
        self.square[7][4].set(Piece::Knight ,Colour::White,7,4);
        self.square[7][5].set(Piece::Bishop ,Colour::White,7,5);
        self.square[7][6].set(Piece::Knight ,Colour::White,7,6);
        self.square[7][7].set(Piece::Rook   ,Colour::White,7,7);

        // self.check_moves();
    }

    pub fn check_moves(&mut self){
        // This is dumb, but it works
        for i in 0..8{
            for j in 0..8{
                self.valid_moves(i,j)
            }
        }

    }

    pub fn valid_moves(&mut self, x: usize, y: usize){
        let pe: Piece = self.square[x][y].piece;
        let col: Colour = self.square[x][y].colour;
        let mut out: Vec<[i8;2]> = vec![];
        // For now I'm just adding the possible moves to the square
        // let mut out: Vec<[i8;2]> = if matches!(col,Colour::White) {self.white_all_moves} else {self.black_all_moves};
        // let mut out_take: Vec<[i8;2]> = if matches!(col,Colour::White) {self.white_take_moves} else {self.black_take_moves};
        // Simply calculate all moves
        // We'll deal with checks later
        match pe{
            Piece::King => {
                // being disciplined and just copying
                // this is real bad though, change it later
                // so much copied code
                match col{
                    Colour::White =>{
                        if (!white_king_move && !white_king_rook_move && match!(self.square[7][6], Piece::Empty) && match(self.square[7][5], Piece::Empty)){out.append([7,6])}
                    }
                }
            },
            Piece::Queen => {},
            Piece::Rook => {},
            Piece::Bishop => {},
            Piece::Knight => {},
            Piece::Pawn => {},
            Piece::Empty => (),
        }

    }

    pub fn print_board(& self){
        // just dump out the board to terminal
        for i in 0..8{
            if i ==0{
                println!("---------------------------------")
            }
            for j in 0..8{
                if j==0{
                    print!("|")
                }
                match self.square[i][j].piece{
                    Piece::King => print!(" K |"),
                    Piece::Queen => print!(" Q |"),
                    Piece::Rook => print!(" R |"),
                    Piece::Bishop => print!(" B |"),
                    Piece::Knight => print!(" K |"),
                    Piece::Pawn => print!(" p |"),
                    Piece::Empty => print!("   |"),
                }
            }
            println!("");

            println!("---------------------------------")
        }
    }

    pub fn make_move(&mut self, square_1_pos: (usize,usize), square_2_pos: (usize,usize)){
        let square_1: Square = self.square[square_1_pos.0][square_1_pos.1].clone();

        self.square[square_2_pos.0][square_2_pos.1].piece = square_1.piece;
        self.square[square_2_pos.0][square_2_pos.1].colour = square_1.colour;
        self.square[square_2_pos.0][square_2_pos.1].possible_moves = vec![];

        self.square[square_1_pos.0][square_1_pos.1].set_empty();

    }
}


