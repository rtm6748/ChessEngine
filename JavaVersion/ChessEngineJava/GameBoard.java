package ChessEngineJava;

import java.util.ArrayList;
import java.util.HashMap;

public class GameBoard{
    private final Piece[][] board;
    public static final int boardSize = 8;

/*    private static boolean blackKingMove = false;
    private static boolean blackARookMove = false;
    private static boolean blackHRookMove = false;
    private static boolean whiteKingMove = false;
    private static boolean whiteARookMove = false;
    private static boolean whiteHRookMove = false;*/


    public GameBoard() {
        board = new Piece[boardSize][boardSize];
        standardSetup();
    }
    public GameBoard(Piece[][] board) {
        this.board = new Piece[board.length][board[0].length];
        for (int i = 0; i < board.length; ++i) {
            for (int j = 0; j < board[i].length; ++j) {
                this.board[i][j] = board[i][j].getCopy();
            }
        }
    }

    public GameBoard getCopy() {
        return new GameBoard(this.board);
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
        board[0][7] = new Rook(new Square(0, 7), Color.BLACK);
        board[1][7] = new Knight(new Square(1, 7), Color.BLACK);
        board[2][7] = new Bishop(new Square(2,7), Color.BLACK);
        board[3][7] = new Queen(new Square(3, 7), Color.BLACK);
        board[4][7] = new King(new Square(4, 7), Color.BLACK);
        board[5][7] = new Bishop(new Square(5,7), Color.BLACK);
        board[6][7] = new Knight(new Square(6,7), Color.BLACK);
        board[7][7] = new Rook(new Square(7, 7), Color.BLACK);
    }

    public Piece getPiece(Square square) {
        return board[square.getX()][square.getY()];
    }

    public Piece move(Piece piece, Square square) {
        Square currSquare = piece.getCurrSquare();
        Piece replaced = board[square.getX()][square.getY()];
        board[square.getX()][square.getY()] = piece;
        board[currSquare.getX()][currSquare.getY()] = new Empty(new Square(currSquare.getX(), currSquare.getY()), (currSquare.getX() % 2 + currSquare.getY() % 2) % 2 == 0 ? Color.BLACK : Color.WHITE);
        piece.setSquare(square);
        return replaced;
    }

    public void set(Piece piece, Square square) {
        board[square.getX()][square.getY()] = piece;
        piece.setSquare(square);
    }

    private ArrayList<Piece> getPieces(Color color) {
        ArrayList<Piece> pieces = new ArrayList<>();
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j].getColor() == color) {
                    pieces.add(board[i][j]);
                }
            }
        }
        return pieces;
    }

    private HashMap<Piece, ArrayList<Square>> getPossibleMoves(Color color) {
        HashMap<Piece, ArrayList<Square>> piecesMoves = new HashMap<>();
        ArrayList<Piece> currPieces = this.getPieces(color);
        for (Piece currPiece : currPieces) {
            piecesMoves.put(currPiece, currPiece.getMoves(this, currPiece.getCurrSquare()));
        }
        return piecesMoves;
    }

    private Piece getKing(Color color) {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j].getClass() == King.class && board[i][j].getColor() == color) {
                    return board[i][j];
                }
            }
        }
        return new Empty(new Square(-1, -1), Color.WHITE);
    }

    public HashMap<Piece, ArrayList<Square>> getValidMoves(Color color) {
        HashMap<Piece, ArrayList<Square>> piecesMoves = getPossibleMoves(color);

        for (Piece piece : piecesMoves.keySet()) {
            int size = piecesMoves.get(piece).size();
            for (int intSquare = 0; intSquare < size; ++intSquare) {
                Square originalPieceSquare = piece.getCurrSquare();

                Square square = piecesMoves.get(piece).get(intSquare);
                Piece piece1 = move(piece, square);

                HashMap<Piece, ArrayList<Square>> otherPiecesMoves = this.getPossibleMoves(color == Color.WHITE ? Color.BLACK : Color.WHITE);

                Piece currKing = this.getKing(color);

                boolean invalidMove = false;
                for (Piece otherPiece : otherPiecesMoves.keySet()) {
                    if (!otherPiecesMoves.get(otherPiece).isEmpty()) {
                        for (Square otherSquare : otherPiecesMoves.get(otherPiece)) {
                            if (currKing.getCurrSquare().equals(otherSquare)) {
                                ArrayList<Square> updatedMoves = piecesMoves.get(piece);

                                if (!piecesMoves.containsKey(piece)) {
                                    System.out.println("Key not found for: " + piece);
                                    System.out.println("All keys: " + piecesMoves.keySet());
                                }

                                updatedMoves.remove(intSquare);
                                piecesMoves.replace(piece, piecesMoves.get(piece), updatedMoves);
                                --intSquare;
                                --size;
                                invalidMove = true;
                                break;
                            }
                        }
                        if (invalidMove) {
                            break;
                        }
                    }
                }

                move(piece, originalPieceSquare);
                set(piece1, square);
            }
        }
        piecesMoves.entrySet().removeIf(entry -> entry.getValue().isEmpty());
        //Add castles
        //Add en passant

        return piecesMoves;
    }


    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        for (int i = boardSize - 1; i >= 0; --i) {
            for (int j = 0; j < boardSize; ++j) {
                stringBuilder.append(board[j][i].toString());
                stringBuilder.append(' ');
            }
            stringBuilder.append('\n');
        }
        return stringBuilder.toString();
    }
}
