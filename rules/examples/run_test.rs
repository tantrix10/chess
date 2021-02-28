extern crate rules;

fn main(){
    let mut board = rules::Board::new();
    // Set up the board to the standard starting position
    board.set();
    // Print the board to terminal
    board.print_board();
    // Play the move 1.e4
    board.make_move((6,4), (4,4));
    // Print the board again to show new move
    board.print_board();
    // Silly move of Rh8xe4 to show piece transfer working
    // This will break once legality is enforced
    board.make_move((0,7), (4,4));
    board.print_board();
}