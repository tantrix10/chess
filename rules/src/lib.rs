#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Piece {
    King,
    Queen,
    Rook,
    Knight,
    Bishop,
    Pawn,
    Empty,
}
#[derive(Clone, Copy, PartialEq)]
pub enum Colour {
    White,
    Black,
    Empty,
}

#[derive(Clone)]
pub enum GameResult {
    WhiteWin,
    BlackWin,
    Draw,
    InProgress,
}

#[derive(Clone)]
pub struct Square {
    pub piece: Piece,
    colour: Colour,
    square: [i8; 2],
    possible_moves: Vec<String>,
    has_moved: bool,
}

impl Square {
    pub fn new() -> Square {
        Square {
            piece: Piece::Empty,
            colour: Colour::Empty,
            square: [0, 0],
            possible_moves: vec![],
            has_moved: false,
        }
    }

    pub fn set(&mut self, piece: Piece, colour: Colour, x: i8, y: i8) {
        self.piece = piece;
        self.colour = colour;
        self.square = [x, y];
    }

    pub fn set_empty(&mut self) {
        self.piece = Piece::Empty;
        self.colour = Colour::Empty;
        self.possible_moves = vec![];
    }
}

#[derive(Clone)]
pub struct Board {
    // Just re-implementing old cpp code for now
    // 8x8 vector of square structs, each containing the piece info
    pub square: [[Square; 8]; 8],
    move_num: i32,
    move_colour: Colour,

    //fifty move counter for draws. Increments if no pawn move, check or
    //capture. If fifty_move_counter== 50, the game is a draw
    fifty_move_counter: i8,

    game_result: GameResult,

    // bool for if a player is in check, not really needed but nice
    // nice to display
    check: bool,

    // if en_passent is non-empty then this pawn can be taken en-passent
    // this may change tbh
    en_passent: (Square, bool),

    // King positions
    white_king: [i8; 2],
    black_king: [i8; 2],

    // Vector of all capture moves to check for checks
    moves: Vec<[[i8; 2]; 2]>,
}

impl Board {
    pub fn new() -> Board {
        Board {
            square: [
                [
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                ],
                [
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                ],
                [
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                ],
                [
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                ],
                [
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                ],
                [
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                ],
                [
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                ],
                [
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                    Square::new(),
                ],
            ],
            fifty_move_counter: 0,
            move_colour: Colour::White,
            game_result: GameResult::InProgress,
            check: false,
            en_passent: (Square::new(), false),
            move_num: 0,
            white_king: [4, 7],
            black_king: [4, 0],
            moves: vec![],
        }
    }

    pub fn set(&mut self) {
        // set the pawns
        for i in 0..8 {
            self.square[6][i].set(Piece::Pawn, Colour::Black, 6, i as i8);
            self.square[1][i].set(Piece::Pawn, Colour::White, 1, i as i8);
        }

        //set the empty squares
        for i in 2..6 {
            for j in 0..8 {
                self.square[i][j].set(Piece::Empty, Colour::Empty, i as i8, j as i8);
            }
        }

        // set the non-pawn and empty pieces
        self.square[0][0].set(Piece::Rook, Colour::Black, 0, 0);
        self.square[0][1].set(Piece::Knight, Colour::Black, 0, 1);
        self.square[0][2].set(Piece::Bishop, Colour::Black, 0, 2);
        self.square[0][3].set(Piece::Queen, Colour::Black, 0, 3);
        self.square[0][4].set(Piece::King, Colour::Black, 0, 4);
        self.square[0][5].set(Piece::Bishop, Colour::Black, 0, 5);
        self.square[0][6].set(Piece::Knight, Colour::Black, 0, 6);
        self.square[0][7].set(Piece::Rook, Colour::Black, 0, 7);

        self.square[7][0].set(Piece::Rook, Colour::White, 7, 0);
        self.square[7][1].set(Piece::Knight, Colour::White, 7, 1);
        self.square[7][2].set(Piece::Bishop, Colour::White, 7, 2);
        self.square[7][3].set(Piece::Queen, Colour::White, 7, 3);
        self.square[7][4].set(Piece::Knight, Colour::White, 7, 4);
        self.square[7][5].set(Piece::Bishop, Colour::White, 7, 5);
        self.square[7][6].set(Piece::Knight, Colour::White, 7, 6);
        self.square[7][7].set(Piece::Rook, Colour::White, 7, 7);

        // self.check_moves();
    }

    pub fn check_moves(&mut self, check_king: bool) {
        // This is dumb, but it works
        for i in 0..8 {
            for j in 0..8 {
                if self.square[i][j].colour == self.move_colour {
                    self.valid_moves(i, j, check_king)
                }
            }
        }
    }

    pub fn switch_colour(&mut self) {
        match self.move_colour {
            Colour::White => self.move_colour = Colour::Black,
            Colour::Black => self.move_colour = Colour::White,
            Colour::Empty => (),
        }
    }

    pub fn check_king_safe(
        &mut self,
        old_square: (usize, usize),
        new_square: (usize, usize),
    ) -> bool {
        let mut tmp_board = self.clone();
        tmp_board.make_move(old_square, new_square);
        tmp_board.check_moves(false);
        return !tmp_board.check;
    }

    pub fn straight_moves(&mut self, x: usize, y: usize) {}

    pub fn valid_moves(&mut self, x: usize, y: usize, check_king: bool) {
        let pe: Piece = self.square[x][y].piece;
        let col: Colour = self.square[x][y].colour;
        let mut out = vec![];
        let mut out_all: Vec<[i8; 2]> = vec![];
        // For now I'm just adding the possible moves to the square
        // let mut out: Vec<[i8;2]> = if matches!(col,Colour::White) {self.white_all_moves} else {self.black_all_moves};
        // let mut out_take: Vec<[i8;2]> = if matches!(col,Colour::White) {self.white_take_moves} else {self.black_take_moves};
        // Simply calculate all moves
        // We'll deal with checks later
        match pe {
            Piece::King => {}
            Piece::Queen => {}
            Piece::Rook => {}
            Piece::Bishop => {}
            Piece::Knight => {}
            Piece::Pawn => {}
            Piece::Empty => (),
        }
        self.square[x][y].possible_moves = out;
    }

    pub fn print_board(&self) {
        // just dump out the board to terminal
        for i in 0..8 {
            if i == 0 {
                println!("---------------------------------")
            }
            for j in 0..8 {
                if j == 0 {
                    print!("|")
                }
                match self.square[i][j].piece {
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

    pub fn print_pos(&self, square_pos: (usize, usize)) {
        let moves = &self.square[square_pos.0][square_pos.1].possible_moves;
        for temp in moves.iter() {
            println!("{}", temp);
        }
    }

    pub fn make_move(&mut self, square_1_pos: (usize, usize), square_2_pos: (usize, usize))-> Result<(usize,usize), &str> {
        let square_1: Square = self.square[square_1_pos.0][square_1_pos.1].clone();

        self.square[square_2_pos.0][square_2_pos.1].piece = square_1.piece;
        self.square[square_2_pos.0][square_2_pos.1].colour = square_1.colour;
        self.square[square_2_pos.0][square_2_pos.1].possible_moves = vec![];
        self.square[square_2_pos.0][square_2_pos.1].has_moved = true;

        self.square[square_1_pos.0][square_1_pos.1].set_empty();

        self.move_num += 1;
        Ok(square_2_pos)
    }

    pub fn notation_to_coord(str_move: String) -> [u8; 2] {
        if str_move.chars().count() == 2 {
            let x1: u8 = 97 - str_move.as_bytes()[0];
            let y1: u8 = 49 - str_move.as_bytes()[1];
            return [x1, y1];
        }
        return [9, 9];
    }

    pub fn coord_to_notation(coord: [u8; 2]) -> String {
        let comp = String::from("abcdefgh");
        let letter = comp.as_bytes()[coord[1] as usize] as char;
        let letter = letter.to_string();
        let number = (8 - coord[0]).to_string();
        let mut out = String::new();
        out += &letter;
        out += &number;
        out
    }
}
