extern crate rules;

fn main(){
    let mut board = rules::Board::new();
    board.set();
    board.print_board();
}