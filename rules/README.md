So currently I am just re-implementing my crappy
half finished cpp implementation.
The point of this is to just get moving with this project
that has been a bit slow/stale. 

Things to implement:

- Piece enum, done
- Colour enum, done
- Square struct, done
- Board struct, done
  - play game
    - 50 move rule
    - checkmate
    - stale mate
  - new board func, done
  - set up board, done
  - print board, done
  - find possible moves
    - including:
      - checks
      - en-passant
      - castling
      - pawn promotion
  - print possible moves
  - make a move, done
    - only make a legal move
  - notation to coord
  - coord to notation

- Set up tests