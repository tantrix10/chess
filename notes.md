In case you ever get confused about importing 

https://stackoverflow.com/questions/45519176/how-do-i-use-or-import-a-local-rust-file#45520092


# Notes phase 1

## Todos in order:
### rules engine
1. Get the correct legal moves generated
	- Checks
		- Look ahead one move to see if King can be caputured, if so, move not legal
	- en passent 
	- promotion
	- castling
	- checkmate
	- stalemate
	- test against chess wiki test position

## Play engine
1. Get an 'engine' playing random moves
2. Add in a score for number of peices and choose the highest from ply 1
3. Add ply - n
4. Improve tree search by pruning, continue checking takes
5. Add selective ordering
6. Add piece location preference to score

## Gui
1. have terminal input
2. generate board from terminal input
3. Move pieces on board, showing legal moves
4. Allow custom board set up etc
5. Play against engine with custom strength, ply, depth search etc etc 

# Notes phase 2

## play engine
1. Include low energy NN estimator for position strength
2. Opening improvement
3. piece 'potential field' modified by surrounding pieces

# Notes phase 3
## play engine
1. Completely different egnines
	1. NLP-inspired engine to pick out important features in the game (what is the 'grammar' of pawn structure)
	2. quantum computer engine
	3. Lowest possible enery engine, maybe NN