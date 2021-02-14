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
				out.push_back("king not yet programmed");

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
		//basically here you want to clear all the moves whichever colour just moved and repopulate them with the update
		switch(col){

			case wh:
			white_take_moves.clear();
			white_all_moves.clear();
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
			black_take_moves.clear();
			black_all_moves.clear();
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
