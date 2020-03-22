#include <iostream>


enum Piece {k, q, r, n, b, p, e};
enum Colour {bl, wh, em};


class Square
{
	Piece piece;
	Colour colour;
	int x, y;

public:
	Piece get_type(){return piece;}
	Colour get_colour(){return colour;}
	void set(Piece type, Colour col, int x1, int y1){
		piece = type; 
		colour = col;
		x = x1;
		y = y1;}

	Square(){
		piece = e;
		colour = em;
		x = -1;
		y = -1;
	};
};






class board
{ 
	Square square[8][8];
	int move_num = 0;
	Colour turn = wh;
	std::string png = "game jamie vs jamie, will extend this to generate pgn "

public:
	void set_board(){
		for (int i = 0; i < 8; i+=1){
	
			square[6][i].set(p, bl, 6, i);
			square[1][i].set(p, wh, 1, i);

		}
		for (int i = 2; i < 6; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				square[i][j].set(e, em, i, j);
			}
		}
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
		for (int i = 0; i < 8; ++i){
			for (int j = 0; j < 8; ++j ){

			Piece v = square[i][j].get_type();
			switch(v){
				case k:
				std::cout << "  King  ";
				break;
				case q:
				std::cout << " Queen  " ;
				break;
				case r:
				std::cout << "  Rook  ";
				break;
				case n:
				std::cout << " Knight " ;
				break;
				case b:
				std::cout << " Bishop ";
				break;
				case p:
				std::cout << "  Pawn  " ;
				break;
				case e:
				std::cout << "   -    " ;
				break;
				}
			}
			std::cout<<"" <<std::endl;
		}
		std::cout<<"" <<std::endl;
	};

	void move(int x1, int x2, int y1, int y2){
		Piece temp = square[x1][y1].get_type();
		Colour col = square[x1][y1].get_colour();
		square[x1][y1].set(e, em, x1, y1);
		square[x2][y2].set(temp, col, x2, y2);
	}
//board();

};


int main(){
	board b;
	b.set_board();
	b.print_board();
	b.move(6,4,4,4);
	b.print_board();
	return 0;
}