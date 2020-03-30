#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
enum Piece {k, q, r, n, b, p, e_p};
enum Colour {bl, wh, e_c};


class Square
{

public:
	Piece piece;
	Colour colour;
	int x, y;

	void set(Piece type, Colour col, int x1, int y1){
		piece = type; 
		colour = col;
		x = x1;
		y = y1;}

	Square(){
		piece = e_p;
		colour = e_c;
		x = -1;
		y = -1;
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
			move(v);
			print_board();
			if(move_num == 3){game_state=false;}

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

	void move(std::vector<int> v){
		// so far this just moves a piece from one square to another
		// need to add checks for legalaity, checks etc 
		Piece temp = square[v[1]][v[0]].piece;
		Colour col = square[v[1]][v[0]].colour;

		square[v[1]][v[0]].set(e_p, e_c, v[1], v[0]);
		square[v[3]][v[2]].set(temp, col, v[3], v[2]);
		move_num++;
	}

	std::vector<int> notation_to_coord(std::string move){
		//this is so I can type in regular notation for moves
		/* chess notation is such that you actually only need the square you're arriving on,
		unless there's an ambiguity. I should implement this for easier pgn implementation
		*/
		pgn += std::to_string(move_num) + ". " + move + " " ;
		std::vector<int> out;
		int x1 = move[0];
		int y1 = move[1]- '0';
		int x2 = move[3];
		int y2 = move[4]- '0';
		
		out.push_back( -97 + x1  );
		out.push_back(  8  - y1  );
		out.push_back( -97 + x2  );
		out.push_back(  8  - y2  );
		return out;
	}



};


int main(){
	board b;
	b.set_board();
	b.play();
	// b.print_board();
	// b.move(6,4,4,4);
	// b.print_board();
	// b.move(1,3,4,4);
	// b.move(7,5,6,5);
	// b.print_board();
	// b.move(0,2,6,5);
	// b.move(5,3,5,4);
	// b.print_board();
	return 0;
}