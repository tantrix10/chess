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
#[derive(Clone, Copy, PartialEq, Debug)]
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
    pub colour: Colour,
    square: [i8; 2],
    possible_moves: Vec<[i8;2]>,
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
    pub check: bool,

    // if en_passent is non-empty then this pawn can be taken en-passent
    // this may change tbh
    en_passent: (Square, bool),

    // King positions
    white_king: [i8; 2],
    black_king: [i8; 2],
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
        }
    }

    pub fn set(&mut self) {
        // set the pawns
        for i in 0..8 {
            self.square[1][i].set(Piece::Pawn, Colour::Black, 6, i as i8);
            self.square[6][i].set(Piece::Pawn, Colour::White, 1, i as i8);
        }

        //set the empty squares
        for i in 2..6 {
            for j in 0..8 {
                self.square[i][j].set(Piece::Empty, Colour::Empty, i as i8, j as i8);
            }
        }

        // set the non-pawn and empty pieces
        self.square[0][0].set(Piece::Rook  , Colour::Black, 0, 0);
        self.square[0][1].set(Piece::Knight, Colour::Black, 0, 1);
        self.square[0][2].set(Piece::Bishop, Colour::Black, 0, 2);
        self.square[0][3].set(Piece::Queen , Colour::Black, 0, 3);
        self.square[0][4].set(Piece::King  , Colour::Black, 0, 4);
        self.square[0][5].set(Piece::Bishop, Colour::Black, 0, 5);
        self.square[0][6].set(Piece::Knight, Colour::Black, 0, 6);
        self.square[0][7].set(Piece::Rook  , Colour::Black, 0, 7);

        self.square[7][0].set(Piece::Rook  , Colour::White, 7, 0);
        self.square[7][1].set(Piece::Knight, Colour::White, 7, 1);
        self.square[7][2].set(Piece::Bishop, Colour::White, 7, 2);
        self.square[7][3].set(Piece::Queen , Colour::White, 7, 3);
        self.square[7][4].set(Piece::King  , Colour::White, 7, 4);
        self.square[7][5].set(Piece::Bishop, Colour::White, 7, 5);
        self.square[7][6].set(Piece::Knight, Colour::White, 7, 6);
        self.square[7][7].set(Piece::Rook  , Colour::White, 7, 7);

        // self.check_moves();
    }

    pub fn check_moves(&mut self, check_king: bool) {
        // This is dumb, but it works
        //let mut count = 0;
        for i in 0..8 {
            for j in 0..8 {
                let tmp = self.move_colour;
                //count += 1;
                //println!("Count: {}", count);
                //println!("{:?}", tmp);
                if matches!(self.square[i][j].colour, tmp) {
                    //println!("{} {}", i,j);
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

    pub fn within_boundary(&self, x:i8, y:i8)-> bool{
        if x > 7 || x < 0 || y > 7 || y < 0  {
            return false
        };
        true

    }

    pub fn check_king_safe(
        & self,
        old_square: (usize, usize),
        new_square: (usize, usize),
    ) -> bool {
        // This or implementing an undo move.
        // Undo will probably be fast, but this seems easier
        let mut tmp_board = self.clone();
        tmp_board.make_move(old_square, new_square);
        // TODO: turn this back on when I only check depth one
        // when its false
        //tmp_board.check_moves(false);
        //return !tmp_board.check;
        return true
    }

    pub fn standard_conditions_to_stop(&self, old_square:(i8,i8), test_square: (i8,i8))->bool{
        let x = test_square.0;
        let y = test_square.1;
        if !self.within_boundary(x,y){
            return false
        }
        // This check can just be on self.move_colour once that rule is in place.
        if self.square[x as usize][y as usize].colour ==
            self.square[old_square.0 as usize][old_square.1 as usize].colour{
            return false
        }
        else{
            // currently not implemented
            // but this wil check to make sure this move makes the king safe
            true
            //return self.check_king_safe(
            //    (old_square.0 as usize, old_square.1 as usize),
            //    (test_square.0 as usize, test_square.1 as usize)
            //)
        }

    }


    pub fn diagonal_moves(&mut self, x: i8, y: i8)->Vec<[i8;2]> {
        // TODO: bring diag and straight together
        let mut out = vec![];
        for dir in [(1,1),(-1,1),(-1,-1),(1,-1)].iter(){
            let mut new_x: i8 = x.clone();
            let mut new_y: i8 = y.clone();
            let mut keep_moving = true;
            while keep_moving{
                new_x += dir.0;
                new_y += dir.1;
                keep_moving = self.standard_conditions_to_stop(
                    (x ,y), (new_x, new_y)
                    );
                if keep_moving{
                    out.push([new_x, new_y]);
                    if !matches!(self.square[new_x as usize][new_y as usize].colour, Colour::Empty){
                        keep_moving = false;
                        if matches!(self.square[new_x as usize][new_y as usize].piece, Piece::King){
                            self.check = true;
                        }
                    }
                }
            }
        }
        out
    }


    pub fn knight_moves(&mut self, x: i8, y: i8)->Vec<[i8;2]> {
        let mut out = vec![];
        for dir in [(1,2),(1,-2),(-1,2),(-1,-2), (2,1),(2,-1),(-2,1),(-2,-1)].iter(){
            let mut new_x: i8 = x.clone();
            let mut new_y: i8 = y.clone();
            new_x += dir.0;
            new_y += dir.1;
            let keep_moving = self.standard_conditions_to_stop(
                (x ,y), (new_x, new_y)
                );
            if keep_moving{
                out.push([new_x, new_y]);
                if matches!(self.square[new_x as usize][new_y as usize].piece, Piece::King){
                            self.check = true;
                }
            }
        }
        out
    }


    pub fn pawn_moves(&mut self, x:i8, y: i8)->Vec<[i8;2]>{
        let mut out = vec![];
        let col = self.square[x as usize][y as usize].colour;
        let first_row = match col{
            Colour::Black => 1,
            Colour::White => 6,
            Colour::Empty => -1,
        };
        let dir = match self.square[x as usize][y as usize].colour{
            Colour::Black => 1,
            Colour::White => -1,
            Colour::Empty => 0,
        };
        let mut test_x = x + dir;
        let test_y = y;
        if matches!(self.square[test_x as usize][test_y as usize].colour, Colour::Empty)
            &&  self.check_king_safe((x as usize, y as usize), (test_x as usize, test_y as usize) )
                && self.within_boundary(x,y)

        {
            out.push([test_x,y]);
            test_x += dir;
            if x == first_row
                && self.check_king_safe((x as usize, y as usize), (test_x as usize, test_y as usize) )
                    && matches!(self.square[test_x as usize][test_y as usize].colour, Colour::Empty)
                {
                    out.push([test_x,y]);
            }
        }
        println!("got this far {},{}", (x+dir),(y+1));
        if self.within_boundary(x+dir,y+1){
        println!("{:?}", self.square[(x+dir) as usize][(y+1) as usize].piece);
        }
        if self.within_boundary(x+dir, y+1)
            && !matches!(self.square[(x+dir) as usize][(y+1) as usize].colour, Colour::Empty)
                && self.square[(x+dir) as usize][(y+1) as usize].colour != col
                    && self.check_king_safe((x as usize, y as usize), ((x+dir) as usize, (y+1) as usize) )
        {
        println!("got this far the other side");
                out.push([x+dir, y+1])
        }
        if self.within_boundary(x+dir, y-1)
            && !matches!(self.square[(x+dir) as usize][(y-1) as usize].colour, Colour::Empty)
                && !matches!(self.square[(x+dir) as usize][(y-1) as usize].colour, col)
                    && self.check_king_safe((x as usize, y as usize), ((x+dir) as usize, (y-1) as usize) )
        {
                out.push([x+dir, y-1])
        }

        out
    }

    pub fn straight_moves(&mut self, x: i8, y: i8)->Vec<[i8;2]> {
        let mut out = vec![];
        for dir in [(1,0),(0,1),(-1,0),(0,-1)].iter(){
            let mut new_x: i8 = x.clone();
            let mut new_y: i8 = y.clone();
            let mut keep_moving = true;
            while keep_moving{
                new_x += dir.0;
                new_y += dir.1;
                keep_moving = self.standard_conditions_to_stop(
                    (x ,y), (new_x, new_y)
                    );
                if keep_moving{
                    out.push([new_x, new_y]);
                    if !matches!(self.square[new_x as usize][new_y as usize].colour, Colour::Empty){
                        keep_moving = false;
                        if matches!(self.square[new_x as usize][new_y as usize].piece, Piece::King){
                            self.check = true;
                        }
                    }
                }
            }
        }
        out
    }

    pub fn valid_moves(&mut self, x: usize, y: usize, check_king: bool) {
        let pe: Piece = self.square[x][y].piece;
        let col: Colour = self.square[x][y].colour;
        let mut out: Vec<[i8;2]> = vec![];
        // For now I'm just adding the possible moves to the square
        // let mut out: Vec<[i8;2]> = if matches!(col,Colour::White) {self.white_all_moves} else {self.black_all_moves};
        // let mut out_take: Vec<[i8;2]> = if matches!(col,Colour::White) {self.white_take_moves} else {self.black_take_moves};
        // Simply calculate all moves
        // We'll deal with checks later
        match pe {
            Piece::King => {}
            Piece::Queen => {
                out.append(&mut self.straight_moves(x as i8,y as i8));
                out.append(&mut self.diagonal_moves(x as i8, y as i8));
            }
            Piece::Rook => {out.append(&mut self.straight_moves(x as i8,y as i8))}
            Piece::Bishop => {out.append(&mut self.diagonal_moves(x as i8, y as i8))}
            Piece::Knight => {out.append(&mut self.knight_moves(x as i8, y as i8))}
            Piece::Pawn => {out.append(&mut self.pawn_moves(x as i8, y as i8))}
            Piece::Empty => (),
        }
        self.square[x][y].possible_moves = out;
    }

    pub fn print_board(&self, possible_board: Option<[usize;2]>) {
        // just dump out the board to terminal
        let possible = possible_board.unwrap_or([10,10]);
        for i in 0..8 {
            if i == 0 {
                println!("---------------------------------")
            }
            for j in 0..8 {
                if j == 0 { print!("|")}//else{print!("   |")};
                if possible_board.is_some()
                    && self.square[possible[0]][possible[1]]
                        .possible_moves.contains(&[i as i8,j as i8])
                {
                    match self.square[i][j].piece {
                        // make a print function so I can pass the possible_board
                        // and make some nice colours!
                        Piece::King => print!("*K*|"),
                        Piece::Queen => print!("*Q*|"),
                        Piece::Rook => print!("*R*|"),
                        Piece::Bishop => print!("*B*|"),
                        Piece::Knight => print!("*N*|"),
                        Piece::Pawn => print!("*p*|"),
                        Piece::Empty => print!(" * |"),
                    }
                }else{
                    match self.square[i][j].piece {
                        // make a print function so I can pass the possible_board
                        // and make some nice colours!
                        Piece::King => print!(" K |"),
                        Piece::Queen => print!(" Q |"),
                        Piece::Rook => print!(" R |"),
                        Piece::Bishop => print!(" B |"),
                        Piece::Knight => print!(" N |"),
                        Piece::Pawn => print!(" p |"),
                        Piece::Empty => print!("   |"),
                    }

                }
            }
            println!("");

            println!("---------------------------------")
        }
    }

    pub fn make_move(&mut self, square_1_pos: (usize, usize), square_2_pos: (usize, usize)) {
        let square_1: Square = self.square[square_1_pos.0][square_1_pos.1].clone();

        self.square[square_2_pos.0][square_2_pos.1].piece = square_1.piece;
        self.square[square_2_pos.0][square_2_pos.1].colour = square_1.colour;
        self.square[square_2_pos.0][square_2_pos.1].possible_moves = vec![];
        self.square[square_2_pos.0][square_2_pos.1].has_moved = true;

        self.square[square_1_pos.0][square_1_pos.1].set_empty();

        self.move_num += 1;
    }

    pub fn notation_to_coord(str_move: String) -> [u8; 2] {
        if str_move.chars().count() == 2 {
            let x1: u8 = 97 - str_move.as_bytes()[0];
            let y1: u8 = 49 - str_move.as_bytes()[1];
            return [x1, y1];
        }
        return [9, 9];
    }

    pub fn coord_to_notation(&self, coord: [i8; 2]) -> String {
        let comp = String::from("abcdefgh");
        let letter = comp.as_bytes()[coord[1] as usize] as char;
        let letter = letter.to_string();
        let number = (8 - coord[0]).to_string();
        let mut out = String::new();
        out += &letter;
        out += &number;
        out
    }

    pub fn print_pos(&self, square_pos: (usize, usize)) {
        let moves =  &self.square[square_pos.0][square_pos.1].possible_moves;
        println!(
            "Possible moves for {:?} on {:?}:",
            self.square[square_pos.0][square_pos.1].piece,
            self.coord_to_notation([square_pos.0 as i8, square_pos.1 as i8])
        );
        println!("{:?}", moves);
        for temp in moves.iter() {
            println!("{}", self.coord_to_notation(*temp))
        }
    }

}
