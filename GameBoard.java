import java.util.ArrayList;
import java.util.HashMap;

public class GameBoard{
    private static Piece[][] board;
    public static final int boardSize = 8;

    private static boolean blackKingMove = false;
    private static boolean blackARookMove = false;
    private static boolean blackHRookMove = false;
    private static boolean whiteKingMove = false;
    private static boolean whiteARookMove = false;
    private static boolean whiteHRookMove = false;


    public GameBoard() {
        board = new Piece[boardSize][boardSize];
        standardSetup();
    }

    public void standardSetup() {
        board[0][0] = new Rook(new Square(0, 0), Color.WHITE);
        board[1][0] = new Knight(new Square(1, 0), Color.WHITE);
        board[2][0] = new Bishop(new Square(2,0), Color.WHITE);
        board[3][0] = new Queen(new Square(3, 0), Color.WHITE);
        board[4][0] = new King(new Square(4, 0), Color.WHITE);
        board[5][0] = new Bishop(new Square(5,0), Color.WHITE);
        board[6][0] = new Knight(new Square(6,0), Color.WHITE);
        board[7][0] = new Rook(new Square(7, 0), Color.WHITE);
        for (int i = 0; i < boardSize; ++i) {
            board[i][1] = new Pawn(new Square(i, 1), Color.WHITE);
        }
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 2; j < 6; ++j) {
                board[i][j]  = new Empty(new Square(i, j), (i % 2 + j % 2) % 2 == 0 ? Color.BLACK : Color.WHITE);
            }
        }
        for (int i = 0; i < boardSize; ++i) {
            board[i][6] = new Pawn(new Square(i, 6), Color.BLACK);
        }
        board[0][7] = new Rook(new Square(0, 7), Color.WHITE);
        board[1][7] = new Knight(new Square(1, 7), Color.WHITE);
        board[2][7] = new Bishop(new Square(2,7), Color.WHITE);
        board[3][7] = new Queen(new Square(3, 7), Color.WHITE);
        board[4][7] = new King(new Square(4, 7), Color.WHITE);
        board[5][7] = new Bishop(new Square(5,7), Color.WHITE);
        board[6][7] = new Knight(new Square(6,7), Color.WHITE);
        board[7][7] = new Rook(new Square(7, 7), Color.WHITE);
    }

    public Piece getPiece(Square square) {
        return board[square.getX()][square.getY()];
    }

    public Piece move(Piece piece, Square square) {
        Square currSquare = piece.getCurrSquare();
        Piece replaced = board[square.getX()][square.getY()];
        //MayCauseCopyProblems
        board[square.getX()][square.getY()] = piece;
        board[currSquare.getX()][currSquare.getY()] = new Empty(new Square(currSquare.getX(), currSquare.getY()), (currSquare.getX() % 2 + currSquare.getY() % 2) % 2 == 0 ? Color.BLACK : Color.WHITE);
        return replaced;
    }

    public void set(Piece piece, Square square) {
        board[square.getX()][square.getY()] = piece;
    }

    public HashMap<Piece, ArrayList<Square>> getValidMoves(Color color) {
        HashMap<Piece, ArrayList<Square>> piecesMoves = new HashMap<>();
        ArrayList<Piece> currPieces = new ArrayList<>();
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j].getColor() == color) {
                    currPieces.add(board[i][j]);
                }
            }
        }
        for (Piece currPiece : currPieces) {
            piecesMoves.put(currPiece, currPiece.getMoves(this, currPiece.getCurrSquare()));
        }

        for (Piece piece : piecesMoves.keySet()) {
            int size = piecesMoves.get(piece).size();
            for (int intSquare = 0; intSquare < size; ++intSquare) {
                Square square = piecesMoves.get(piece).get(intSquare);
                Piece piece1 = move(piece, square);

                HashMap<Piece, ArrayList<Square>> otherPiecesMoves = new HashMap<>();
                ArrayList<Piece> otherPieces = new ArrayList<>();
                for (int i = 0; i < boardSize; ++i) {
                    for (int j = 0; j < boardSize; ++j) {
                        Color otherColor = Color.WHITE == color ? Color.BLACK : Color.WHITE;
                        if (board[i][j].getColor() == otherColor) {
                            otherPieces.add(board[i][j]);
                        }
                    }
                }

                for (Piece otherPiece : otherPieces) {
                    otherPiecesMoves.put(otherPiece, otherPiece.getMoves(this, otherPiece.getCurrSquare()));
                }

                Piece currKing = new Empty(new Square(-1, -1), Color.WHITE);
                boolean found = false;
                for (int i = 0; !found; ++i) {
                    for (int j = 0; j < boardSize; ++j) {
                        if (board[i][j].getClass() == King.class && board[i][j].getColor() == color) {
                            currKing = board[i][j];
                            found = true;
                            break;
                        }
                    }
                }
                boolean invalidMove = false;
                for (Piece otherPiece : otherPiecesMoves.keySet()) {
                    for (Square otherSquare : piecesMoves.get(otherPiece)) {
                        if (currKing.getCurrSquare() == otherSquare) {
                            ArrayList<Square> updatedMoves = piecesMoves.get(piece);
                            piecesMoves.remove(piece, piecesMoves.get(piece));
                            updatedMoves.remove(intSquare);
                            piecesMoves.put(piece, updatedMoves);
                            --intSquare;
                            invalidMove = true;
                            break;
                        }
                    }
                    if (invalidMove) {
                        break;
                    }
                }

                move(piece, piece.getCurrSquare());
                set(piece1, square);
            }
        }
        return piecesMoves;
    }


    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                stringBuilder.append(board[i][j].toString());
                stringBuilder.append(' ');
            }
            stringBuilder.append('\n');
        }
        return stringBuilder.toString();
    }
}
