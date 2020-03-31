#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>


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
	Square square[8][8];
	int move_num = 0;
	std::string pgn = "game jamie vs jamie, will extend this to generate pgn ";
	Piece total; //I want to keep track of all pieces on the board so I can generate all availible moves
	bool game_state = true;

public:
	void play(){
		std::string mv;
		
		std::string player1;
		std::string player2;
		std::cout<<"Enter player1 and player2" << std::endl; 
		std::cout<<"player 1:";
		std::cin >> player1;
		std::cout<<"player 2:";
		std::cin >> player2;
		print_board();
		while(game_state){
			//very simple play system


			
			std::string colmove;
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
			move(v);
			print_board();
			if(move_num == 3){game_state=false;}
			}
		}

	  std::ofstream myfile; //really I want the moves to go onto new lines but I'll do that later
	  myfile.open (player1 + "_vs_" + player2 + "_pgn.txt");
	  myfile << pgn << std::endl;
	  myfile.close();
	}

	void set_board(){
		//set the pawns
		for (int i = 0; i < 8; i+=1){
	
			square[6][i].set(p, bl, 6, i);
			square[1][i].set(p, wh, 1, i);

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
		square[0][0].set(r,wh,0,0);
		square[0][1].set(n,wh,0,1);
		square[0][2].set(b,wh,0,2);
		square[0][3].set(q,wh,0,3);
		square[0][4].set(k,wh,0,4);
		square[0][5].set(b,wh,0,5);
		square[0][6].set(n,wh,0,6);
		square[0][7].set(r,wh,0,7);

		square[7][7].set(r,wh,7,7);
		square[7][0].set(r,wh,7,0);
		square[7][1].set(n,wh,7,1);
		square[7][2].set(b,wh,7,2);
		square[7][3].set(q,wh,7,3);
		square[7][4].set(k,wh,7,4);
		square[7][5].set(b,wh,7,5);
		square[7][6].set(n,wh,7,6);
		square[7][7].set(r,wh,7,7);

		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				valid_moves(i, j);
			}
		}

	};

	void print_board(){
		//simple print, just itterate over square and print its value
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
		std::vector<std::string> moves = square[v[1]][v[0]].possible_moves;

		for (auto i = moves.begin(); i != moves.end(); ++i){
    		std::cout << *i << ' ';
		}
		std::cout<<std::endl;
	}


	void move(std::vector<int> v){
		// so far this just moves a piece from one square to another
		// need to add checks for legalaity, checks etc 
		Piece temp = square[v[1]][v[0]].piece;
		Colour col = square[v[1]][v[0]].colour;

		square[v[1]][v[0]].set(e_p, e_c, v[1], v[0]);
		square[v[3]][v[2]].set(temp, col, v[3], v[2]);
		move_num++;
		valid_moves(v[3], v[2]);
	}

	std::vector<int> notation_to_coord(std::string move){
		//this is so I can type in regular notation for moves
		/* chess notation is such that you actually only need the square you're arriving on,
		unless there's an ambiguity. I should implement this for easier pgn implementation
		*/
		if(move.length()==2){

		std::vector<int> out;
		int x1 = move[0];
		int y1 = move[1]- '0';

		
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
		//this will take in a coord and generate the notation, for printing possible moves
		std::string out;
		std::string comp = "abcdefgh";
		out += comp[y];
		out += std::to_string(8-x + 1);
		return out;

	}

	void valid_moves(int x, int y){
		//just for testing I will do this with the coords, but I will make this better by using pointers to square objects later
		//it is important for me to generate a list of all possible moves for the game engine I want to make later
		Piece pe = square[x][y].piece;
		Colour col = square[x][y].colour;
		std::vector<std::string> out;
		switch(pe){
				case k:
				//need to add itterating over other possible moves to look for moving into checks
				//also need to add a castling move (which means tracking if the king has moved)
				out.push_back("king not yet programmed");

				break;

				case q:
				//std::cout << "Queen " ;
				out.push_back("queen not yet programmed");

				break;

				case r:
				//std::cout << " Rook ";
				out.push_back("rook not yet programmed");

				break;

				case n:
				//std::cout << "Knight" ;
				out.push_back("knight not yet programmed");

				break;

				case b:
				//std::cout << "Bishop";
				out.push_back("bishop not yet programmed");

				break;

				case p:
				//n.b. here I still need to add the en-passent rule, but I need to add that state to the pawn peice and actually have a proper square swtich function
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
						}
						if (square[x-1][y+1].colour == bl and x-1 >=0 and y+1 < 8){
						out.push_back(coord_to_notation(x-1,y+1));
						}
					break;
					case bl:
						if (x==1 and square[x+1][y].piece == e_p and square[x+2][y].piece == e_p){
							out.push_back(coord_to_notation(x,y+2));
							
						}
						if (square[x+1][y].piece == e_p and x+1 < 8 ){
							out.push_back(coord_to_notation(x,y+1));
						}
						if (square[x+1][y-1].colour == wh and x+1 < 8 and y-1 >= 0){
							out.push_back(coord_to_notation(x+1,y-1));
						}
						if (square[x+1][y+1].colour == wh and x-1 >=0 and y+1 < 8){
						out.push_back(coord_to_notation(x+1,y+1));
						}
					break;
				}

				case e_p:
				out.push_back("that's an empty square, nothing to move here!");
				break;

		};


		square[x][y].possible_moves = out;
	};


};


int main(){
	board b;
	b.set_board();
	b.play();
	return 0;
}