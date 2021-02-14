#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <algorithm>

//enum declare the pieces and colours
enum Piece {k, q, r, n, b, p, e_p};
enum Colour {bl, wh, e_c};


class Square
{

public:
	//a square has a peice, a colour and coords. 
	Piece piece;
	Colour colour;
	int x, y;
	std::vector<std::string> possible_moves;


	void set(Piece type, Colour col, int x1, int y1){
		//using a set function just to make moving easier
		piece = type; 
		colour = col;
		x = x1;
		y = y1;

	};

	Square(){
		//instaniate each square as empty and on 0,0 square
		piece = e_p;
		colour = e_c;
		x = 0;
		y = 0;
	};


};






class board
{ 
	Square square[8][8]; //instantiate 8x8 array of squares
	int move_num           = 0; //track move number to track whose turn it is
	int fifty_move_counter = 0;
	std::string pgn        = "game pgn, will extend this to generate proper pgn  standard";
	bool game_state        = true; //game state false when finished, will add termination condition too, white win/black win/draw
	int result; //0, draw, 1, win for white, -1 win for black

	bool white_check       = false;
	bool black_check       = false;
	bool en_passent        = false;


	bool white_king_move       = false;
	bool black_king_move       = false;
	bool white_king_rook_move  = false;
	bool white_queen_rook_move = false;
	bool black_king_rook_move  = false;
	bool blac_queen_rook_move  = false;


	std::string white_king = "e1";
	std::string black_king = "e8";
	//vector of all possible moves to compare against the king for checks as well as enforcing only legal moves played
	//remember that pawns moving forward do not capture
	std::vector<std::string> white_take_moves;
	std::vector<std::string> black_take_moves;  
	std::vector<std::string> white_all_moves; //a list of all moves to check for stalemate condition, if empty it's a draw
	std::vector<std::string> black_all_moves;
	std::vector<std::string> temp_string; //this is bad practice, but sometimes I just need a temp string I can use 
	


public:
	void play(){
		std::string mv; //input move
		//enter player names

		std::string colmove; //whose move is it

		std::string player1;
		std::string player2;
		std::cout<<"Enter player1 and player2" << std::endl; 
		std::cout<<"player 1:";
		std::cin >> player1;
		std::cout<<"player 2:";
		std::cin >> player2;
		int move_test;
		print_board();

		while(game_state){
			//very simple play system
			//currently allows any move to be made or checking legal moves of a square
			//tracks whose move it is


			


			if(move_num % 2 ==0){colmove = "white";}
			else{colmove = "black";}

			std::cout<< "It is " << colmove << " to move. Enter move: ";
			// format shoudl be "square1-square2" for exmaple "e1-e5"
			std::cin >> mv;
			std::vector<int> v = notation_to_coord(mv);

			if(mv.length() == 2){//if you querry a single square it will list the possible moves from that square
				//std::cout<< mv << " " << v[0] << v[1] <<std::endl;
				print_pos(v);
				//std::cout << std::endl;
			}
			if(mv.length() == 5){
			move_test = move(v);
			if (move_test == -1){
				std::cout<<"illegal move, try again"<<std::endl;
				continue;
			}
			print_board();
			if(move_num == 15){game_state=false;} //terminate the game after three moves, just for testing
			}
		}

	  std::ofstream myfile; //open and print the move into an output pgn
	  myfile.open (player1 + "_vs_" + player2 + "_pgn.txt");
	  myfile << pgn << std::endl;
	  myfile.close();
	}

	void set_board(){
		//set the pawns
		for (int i = 0; i < 8; i+=1){
	
			square[6][i].set(p, wh, 6, i);
			square[1][i].set(p, bl, 1, i);

		}
		//set the empty squares
		for (int i = 2; i < 6; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				square[i][j].set(e_p, e_c, i, j);
			}
		}

		// set the non-pawn and empty peices 
		square[0][0].set(r,bl,0,0);
		square[0][1].set(n,bl,0,1);
		square[0][2].set(b,bl,0,2);
		square[0][3].set(q,bl,0,3);
		square[0][4].set(k,bl,0,4);
		square[0][5].set(b,bl,0,5);
		square[0][6].set(n,bl,0,6);
		square[0][7].set(r,bl,0,7);

		square[7][7].set(r,wh,7,7);
		square[7][0].set(r,wh,7,0);
		square[7][1].set(n,wh,7,1);
		square[7][2].set(b,wh,7,2);
		square[7][3].set(q,wh,7,3);
		square[7][4].set(k,wh,7,4);
		square[7][5].set(b,wh,7,5);
		square[7][6].set(n,wh,7,6);
		square[7][7].set(r,wh,7,7);

		check_moves();
		

	};

	//this is a function to make sure when you move a piece is doesn't put your own king in check
	//it takes a move, checks all moves your oponent can make then checks to see if any could take your king
	bool check_pin(std::vector<int> v, Colour t_col){

		Piece temp = square[v[1]][v[0]].piece;
		Colour col = square[v[1]][v[0]].colour;


		/*
		the idea of this function is to check all oposite coloured pieces, then check if it's rook, queen or bishop
		those are the only pieces that can pin a piece to a king

		then define the direction that the pin is in and if the piece moves in the same direction it's okay
		
		need to be careful of cases where 

		*/

		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				Piece temp_opp = square[v[3]][v[2]].piece;
				if(square[i][j].colour != col and (temp_opp == q or temp_opp == r or temp_opp == b)){


				}
			}
		}


	}

	void check_moves(){
		// there shoudl be a more efficient way to do this. 
		/*
		I can manually set possible moves when setting the board
		after that, I would only need to check straight and diag lines from that piece
		plus the night moves.

		Also moves eminating from where the new piece went. 

		Implement this after the rest is actually working, it's okay for now. 


		*/
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				valid_moves(i, j);
			}
		}
	};

	void print_board(){
		//simple print, just itterate over all squares and print its value
		for (int i = 0; i < 8; ++i){
			if (i == 0)
			{
				std::cout << "---------------------------------------------------------"<<std::endl ;
				
			}
			for (int j = 0; j < 8; ++j ){
			if (j == 0)
			{
				std::cout << "|" ;
				
			}
			Piece v = square[i][j].piece;
			switch(v){
				case k:
				std::cout << " King |";
				break;
				case q:
				std::cout << "Queen |" ;
				break;
				case r:
				std::cout << " Rook |";
				break;
				case n:
				std::cout << "Knight|" ;
				break;
				case b:
				std::cout << "Bishop|";
				break;
				case p:
				std::cout << " Pawn |" ;
				break;
				case e_p:
				std::cout << "  +   |" ;
				break;
				}
			}
			std::cout<<std::endl<<"---------------------------------------------------------" <<std::endl;
		}
		std::cout<<"" <<std::endl;
	};

	void print_pos(std::vector<int> v){
		//print out all possible moves for a given square
		std::vector<std::string> moves = square[v[1]][v[0]].possible_moves;
		std::cout<<"Possible moves: "; 
		for (auto i = moves.begin(); i != moves.end(); ++i){
    		std::cout <<*i << ' ';
		}
		std::cout<<std::endl<<std::endl;
	}


	int move(std::vector<int> v){
		// so far this just moves a piece from one square to another
		// need to add checks for legalaity, checks etc 

		//takes in vector of length four of form y1, x1, y2, x2. 
		//where those are the coordinates moving from and to in the actual array, not notation. 
		Piece temp = square[v[1]][v[0]].piece;
		Colour col = square[v[1]][v[0]].colour;
		std::vector<std::string> poss_moves = square[v[1]][v[0]].possible_moves;

		std::vector<std::string>::iterator it;
		Colour old_colour = square[v[3]][v[2]].colour;

 		//first check if moves is allowed!
		std::string test = coord_to_notation(v[3], v[2]);

		//if the move is not found in the list of possible moves for that square return -1, not a legal move
		it = std::find(poss_moves.begin(), poss_moves.end(), test); 

		if(it == poss_moves.end() ){
			return -1;
		}


		//don't clear here, because you might need it to compare if a king can move somewhere
		//take_moves.clear(); //empty the set of all moves

		square[v[1]][v[0]].set(e_p, e_c, v[1], v[0]);
		square[v[3]][v[2]].set(temp, col, v[3], v[2]);
		
		if (temp == k){
			switch(col){

				case wh:
					white_king = coord_to_notation(v[3], v[2]);
				break;

				case bl:
					black_king = coord_to_notation(v[3], v[2]);
					break;

			}
		}
		//here we simply increment a counter if there are no checks or pawn moves
		//50 moves without a check or pawn move and the game is a draw
		//reset the counter if we get a check or pawn move
		if(temp == p or (old_colour != col and old_colour != e_c) ){

			fifty_move_counter = 0;
		}
		else{

			++fifty_move_counter;
		}
		if(fifty_move_counter == 50){
			std::cout<<"Draw by 50 move rule!"<<std::endl;
			result = 0;
			game_state = false;
		}


		/*to-do:

		-clear and re-populate the moves of one colour
		-then check if the other colour is in check
		-then re-populate the possible moves of the next colour to move

		*/
		//clear out old moves and redfine possible moves.
		white_take_moves.clear();
		white_all_moves.clear();


		black_take_moves.clear();
		black_all_moves.clear();

		check_moves();


		//if white has just made a move and black now has none remaining, the game ends in some way.
		//and visa versa
		if(move_num%2 == 0){
		//once we have a list of all possible captures then we have to check for checks
			if(black_all_moves.size() == 0 ){
				game_state = false;
				std::cout<<"Black has no more moves"<<std::endl;
				if (black_check == true){
					std::cout<<"white wins!"<<std::endl;
					game_state = false;
					result = 1; //if black has no moves and is in check, they loose
				}

				else{
				result = 0;
				}
			}
			//if white has just moved and can take the black king, it's check
			for(int i = 0; i <= white_take_moves.size(); ++i ){
				if( white_take_moves[i] == black_king){
					black_check = true;
					std::cout<<"Check!"<<std::endl;
					break;
				}
				else{
					black_check = false;
				}
				
			 }	


		}


		if(move_num%2 == 1){
			//once we have a list of all possible captures then we have to check for checks
			if(white_all_moves.size() == 0 ){
				game_state = false;
				std::cout<<"white has no more moves"<<std::endl;
				if (white_check == true){
						std::cout<<"Black Wins!"<<std::endl;
						result = -1;
						game_state = false;
						 //if white is in check and has no moves, black wins
					}
				}
				result = 0;
			
				for(int i = 0; i <= black_take_moves.size(); ++i ){
				if( black_take_moves[i] == white_king){
					white_check = true;
					std::cout<<"Check!"<<std::endl;
					break;
				}
				else{
					white_check = false;
				}
				
			}
				
			

		}



		++move_num; 
		return 1;
	}

	std::vector<int> notation_to_coord(std::string move){
		//this is so I can type in regular notation for moves
		/* chess notation is such that you actually only need the square you're arriving on,
		unless there's an ambiguity. I should implement this for easier pgn implementation
		*/
		if(move.length()==2){

		std::vector<int> out;
		int x1 = move[0];
		int y1 = move[1]- '0';//bit of a hacky way to convert char to int perhaps but works for now

		
		out.push_back( -97 + x1  ); //string to int char going from a-h goes from 97-101
		out.push_back(  8  - y1  ); //just have to flip the coords as we count a1 on a chess board bottom left

		return out;

		}
		
		pgn += std::to_string(move_num) + ". " + move + " " ;
		std::vector<int> out;
		int x1 = move[0];
		int y1 = move[1]- '0';
		int x2 = move[3];
		int y2 = move[4]- '0';
		
		out.push_back( -97 + x1  ); //string to int char going from a-h goes from 97-101
		out.push_back(  8  - y1  ); //just have to flip the coords as we count a1 on a chess board bottom left
		out.push_back( -97 + x2  );
		out.push_back(  8  - y2  );
		return out;
	}




	std::string coord_to_notation(int x, int y){
		//this will take in a coordinate from the array and generate the notation
		std::string out;
		std::string comp = "abcdefgh";
		out += comp[y];
		out += std::to_string(8-x );
		return out;

	}

	void valid_moves(int x, int y){
		//just for testing I will do this with the coords, but I will make this better by using pointers to square objects later
		//it is important for me to generate a list of all possible moves for the game engine I want to make later
		//also this will be used to enforce legal moves are being made. 

		//to-do: just make a little check valid move function, and loop over it, much cleaner and easier to make changes to. 
		//to-do: add check condition, check mate condition and enpassent condition.
		//to-do: add castling, needs a king moved statement 
		Piece pe = square[x][y].piece;
		Colour col = square[x][y].colour;
		std::vector<std::string> out;
		std::vector<std::string> pawn_out;
		switch(pe){

				case e_p:
				//not sure why this is being pushed for pawn moves, need to check that
				out.push_back("that's an empty square, nothing to move here!");
				break;


				case k:
				//need to add itterating over other possible moves to look for moving into checks
				//also need to add a castling move (which means tracking if the king has moved)
				switch(col){
					//first check if the king and rook haven't moved and the space inbetween is empty, then we can castle
					//still need to add other moves
					case bl:
						if (black_king_move  == false and black_king_rook_move == false
							and square[0][6].piece == e_p and square[0][5].piece == e_p
							and std::count(white_take_moves.begin(), white_take_moves.end(), "f8") == 0 
							and std::count(white_take_moves.begin(), white_take_moves.end(), "g8") == 0
							){
							out.push_back("g8")
						}
						if (black_king_move  == false and black_queen_rook_move == false
							and square[0][1].piece == e_p and square[0][2].piece == e_p and square[0][3].piece == e_p
							and std::count(white_take_moves.begin(), white_take_moves.end(), "d8") == 0 
							and std::count(white_take_moves.begin(), white_take_moves.end(), "c8") == 0
							and std::count(white_take_moves.begin(), white_take_moves.end(), "b8") == 0 
							){
							out.push_back("c8")
						}

					break;
					case wh:
						if (white_king_move  == false and white_king_rook_move == false
							and square[7][6].piece == e_p and square[7][5].piece == e_p
							and std::count(white_take_moves.begin(), white_take_moves.end(), "f1") == 0 
							and std::count(white_take_moves.begin(), white_take_moves.end(), "g1") == 0
							){
							out.push_back("g1")
						}
						if (white_king_move  == false and white_queen_rook_move == false
							and square[7][1].piece == e_p and square[7][2].piece == e_p and square[7][3].piece == e_p
							and std::count(white_take_moves.begin(), white_take_moves.end(), "d1") == 0 
							and std::count(white_take_moves.begin(), white_take_moves.end(), "c1") == 0
							and std::count(white_take_moves.begin(), white_take_moves.end(), "b1") == 0 
							){
							out.push_back("c1")
						}

					break;
				}
				out.push_back("not all king moves programmed yet");

				break;

				case q:
				
				//this is simply a combination of rook and bishop movements 
				{
					//move in each four diagonal directions until you encounter the edge of the board, or a piece.
					//if that piece is of oposite colour you can take it
					int i = 1;
					while(x-i>= 0 and y-i >= 0){
						
						if(square[x-i][y-i].piece == e_p){
							out.push_back(coord_to_notation(x-i,y-i));
						}
						else if(square[x-i][y-i].colour == square[x][y].colour){
							break;
						}
						else if(square[x-i][y-i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-i,y-i));
							break;
						}

						i++;
					}

					i = 1;
					while(x+i < 8 and y+i < 8){
						
						if(square[x+i][y+i].piece == e_p){
							out.push_back(coord_to_notation(x+i,y+i));
						}
						else if(square[x-i][y+i].colour == square[x][y].colour){
							break;
						}
						else if(square[x+i][y+i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+i,y+i));
							break;
						}

						i++;
					}
					i = 1;
					while(y-i >= 0 and x+i < 8){
						
						if(square[x+i][y-i].piece == e_p){
							out.push_back(coord_to_notation(x+i,y-i));
						}
						else if(square[x+i][y-i].colour == square[x][y].colour){
							break;
						}
						else if(square[x+i][y-i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+i,y-i));
							break;
						}

						i++;
					}

					i = 1;
					while(y+i < 8 and x-i >= 0){
						
						if(square[x-i][y+i].piece == e_p){
							out.push_back(coord_to_notation(x-i,y+i));
						}
						else if(square[x-i][y+i].colour == square[x][y].colour){
							break;
						}
						else if(square[x-i][y+i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-i,y+i));
							break;
						}

						i++;
					}
					i = 1;
					while(x-i>=0){
						
						if(square[x-i][y].piece == e_p){
							out.push_back(coord_to_notation(x-i,y));
						}
						else if(square[x-i][y].colour == square[x][y].colour){
							break;
						}
						else if(square[x-i][y].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-i,y));
							break;
						}

						i++;
					}

					i = 1;
					while(x+i < 8){
						
						if(square[x+i][y].piece == e_p){
							out.push_back(coord_to_notation(x+i,y));
						}
						else if(square[x+i][y].colour == square[x][y].colour){
							break;
						}
						else if(square[x+i][y].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+i,y));
							break;
						}

						i++;
					}
					i = 1;
					while(y-i>=0){
						
						if(square[x][y-i].piece == e_p){
							out.push_back(coord_to_notation(x,y-i));
						}
						else if(square[x][y-i].colour == square[x][y].colour){
							break;
						}
						else if(square[x][y-i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x,y-i));
							break;
						}

						i++;
					}

					i = 1;
					while(y+i < 8){
						
						if(square[x][y+i].piece == e_p){
							out.push_back(coord_to_notation(x,y+i));
						}
						else if(square[x][y+i].colour == square[x][y].colour){
							break;
						}
						else if(square[x][y+i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x,y+i));
							break;
						}

						i++;
					}

				};

				break;

				case r:
				{
					//move in each four straight directions until you encounter the edge of the board, or a piece.
					//if that piece is of oposite colour you can take it
					int i = 1;
					while(x-i>=0){
						
						if(square[x-i][y].piece == e_p){
							out.push_back(coord_to_notation(x-i,y));
						}
						else if(square[x-i][y].colour == square[x][y].colour){
							break;
						}
						else if(square[x-i][y].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-i,y));
							break;
						}

						i++;
					}

					i = 1;
					while(x+i < 8){
						
						if(square[x+i][y].piece == e_p){
							out.push_back(coord_to_notation(x+i,y));
						}
						else if(square[x+i][y].colour == square[x][y].colour){
							break;
						}
						else if(square[x+i][y].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+i,y));
							break;
						}

						i++;
					}
					i = 1;
					while(y-i>=0){
						
						if(square[x][y-i].piece == e_p){
							out.push_back(coord_to_notation(x,y-i));
						}
						else if(square[x][y-i].colour == square[x][y].colour){
							break;
						}
						else if(square[x][y-i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x,y-i));
							break;
						}

						i++;
					}

					i = 1;
					while(y+i < 8){
						
						if(square[x][y+i].piece == e_p){
							out.push_back(coord_to_notation(x,y+i));
						}
						else if(square[x][y+i].colour == square[x][y].colour){
							break;
						}
						else if(square[x][y+i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x,y+i));
							break;
						}

						i++;
					}

				};
				break;

				case n:
				
				//first check case of knight going forward (again there's a better way to do this, but will do that later (I really hate this one haha))
				//yeah, when I tidy this up it's very simple to check the combinations of x +- (2,1), y +- (2,1)
				if(x+2 < 8){
					if(y+1 < 8){

						if(square[x+2][y+1].piece == e_p){
							out.push_back(coord_to_notation(x+2,y+1));
						}
						else if(square[x+2][y+1].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+2,y+1));
							
						}
					}
					if(y-1 > 0){

						if(square[x+2][y-1].piece == e_p){
							out.push_back(coord_to_notation(x+2,y-1));
						}
						else if(square[x+2][y-1].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+2,y-1));
							
						}
					}
				}

				if(x-2 > 0){
					if(y+1 < 8){

						if(square[x-2][y+1].piece == e_p){
							out.push_back(coord_to_notation(x-2,y+1));
						}
						else if(square[x-2][y+1].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-2,y+1));
							
						}
					}
					if(y-1 >= 0){

						if(square[x-2][y-1].piece == e_p){
							out.push_back(coord_to_notation(x-2,y-1));
						}
						else if(square[x-2][y-1].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-2,y-1));
							
						}
					}
				}
				// this deals with the knight moving forwards or backwards now, now to deal with moving sideways first


				if(y+2 < 8){
					if(x+1 < 8){

						if(square[x+1][y+2].piece == e_p){
							out.push_back(coord_to_notation(x+1,y+2));
						}
						else if(square[x+1][y+2].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+1,y+2));
							
						}
					}
					if(x-1 > 0){

						if(square[x-1][y+2].piece == e_p){
							out.push_back(coord_to_notation(x-1,y+2));
						}
						else if(square[x-1][y+2].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-1,y+2));
							
						}
					}
				}

				if(y-2 > 0){
					if(x+1 < 8){

						if(square[x+1][y-2].piece == e_p){
							out.push_back(coord_to_notation(x+1,y-2));
						}
						else if(square[x+1][y-2].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+1,y-2));
							
						}
					}
					if(x-1 >= 0){

						if(square[x-1][y-2].piece == e_p){
							out.push_back(coord_to_notation(x-1,y-2));
						}
						else if(square[x-1][y-2].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-1,y-2));
							
						}
					}
				}


				break;

				case b:
				
				{
					//move in each four diagonal directions until you encounter the edge of the board, or a piece.
					//if that piece is of oposite colour you can take it
					int i = 1;
					while(x-i>= 0 and y-i >= 0){
						
						if(square[x-i][y-i].piece == e_p){
							out.push_back(coord_to_notation(x-i,y-i));
						}
						else if(square[x-i][y-i].colour == square[x][y].colour){
							break;
						}
						else if(square[x-i][y-i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-i,y-i));
							break;
						}

						i++;
					}

					i = 1;
					while(x+i < 8 and y+i < 8){
						
						if(square[x+i][y+i].piece == e_p){
							out.push_back(coord_to_notation(x+i,y+i));
						}
						else if(square[x+i][y+i].colour == square[x][y].colour){
							break;
						}
						else if(square[x+i][y+i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+i,y+i));
							break;
						}

						i++;
					}
					i = 1;
					while(y-i >= 0 and x+i < 8){
						
						if(square[x+i][y-i].piece == e_p){
							out.push_back(coord_to_notation(x+i,y-i));
						}
						else if(square[x+i][y-i].colour == square[x][y].colour){
							break;
						}
						else if(square[x+i][y-i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x+i,y-i));
							break;
						}

						i++;
					}

					i = 1;
					while(y+i < 8 and x-i >= 0){
						
						if(square[x-i][y+i].piece == e_p){
							out.push_back(coord_to_notation(x-i,y+i));
						}
						else if(square[x-i][y+i].colour == square[x][y].colour){
							break;
						}
						else if(square[x-i][y+i].colour != square[x][y].colour){
							out.push_back(coord_to_notation(x-i,y+i));
							break;
						}

						i++;
					}

				};

				break;

				case p:
				//n.b. here I still need to add the en-passent rule, but I need to add that state to the pawn peice and actually have a proper square swtich function
				//pawns are the only peices that move differently depending on colour so we need another switch case. 
				//check if the pawn is on the 2nd/6th rank to see if it can move two squares, if not standard rules.
				switch(col){
					case wh:
						if (x==6 and square[x-1][y].piece == e_p and square[x-2][y].piece == e_p){
							out.push_back(coord_to_notation(x-2,y));
							
						}
						if (square[x-1][y].piece == e_p and x-1 >= 0 ){
							out.push_back(coord_to_notation(x-1,y));
						}
						if (square[x-1][y-1].colour == bl and x-1 >=0 and y-1 >=0){
							out.push_back(coord_to_notation(x-1,y-1));
							pawn_out.push_back(coord_to_notation(x-1,y-1));
						}
						if (square[x-1][y+1].colour == bl and x-1 >=0 and y+1 < 8){
						out.push_back(coord_to_notation(x-1,y+1));
						pawn_out.push_back(coord_to_notation(x-1,y+1));
						}
					break;
					case bl:
						if (x==1 and square[x+1][y].piece == e_p and square[x+2][y].piece == e_p){
							out.push_back(coord_to_notation(x+2,y));
							
						}
						if (square[x+1][y].piece == e_p and x+1 < 8 ){
							out.push_back(coord_to_notation(x+1,y));
						}
						if (square[x+1][y-1].colour == wh and x+1 < 8 and y-1 >= 0){
							out.push_back(coord_to_notation(x+1,y-1));
							pawn_out.push_back(coord_to_notation(x+1,y-1));
						}
						if (square[x+1][y+1].colour == wh and x-1 >=0 and y+1 < 8){
						out.push_back(coord_to_notation(x+1,y+1));
						pawn_out.push_back(coord_to_notation(x+1,y+1));
						}
					break;
				}
		};


		square[x][y].possible_moves = out;
		//the only piece that makes a capture move that is different to its regular movement is the pawn
		//so strictly speaking the take moves are not only the take moves but also moves to empty squares
		//it might be worth removing that incase I want to play with weighting those moves higher
		switch(col){

			case wh:

			white_all_moves.insert(white_all_moves.end(), out.begin(), out.end()  );
			//clear the moves here because we need to check where they king can go
			if(pe != p){
			white_take_moves.insert(white_take_moves.end(), out.begin(), out.end());
			}
			else{
				white_take_moves.insert(white_take_moves.end(), pawn_out.begin(),pawn_out.end());
			}
			break;

			case bl:

			black_all_moves.insert(black_all_moves.end(), out.begin(), out.end()  );
			//clear the moves here because we need to check where they king can go
			if(pe != p){
			black_take_moves.insert(black_take_moves.end(), out.begin(), out.end());
			}
			else{
				black_take_moves.insert(black_take_moves.end(), pawn_out.begin(),pawn_out.end());
			}
			break;

		}
	};


};


int main(){
	board b;
	b.set_board();
	b.play();
	return 0;
}