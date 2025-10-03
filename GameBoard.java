public class GameBoard{
    private static Piece[][] board;
    public static final int boardSize = 8;

    public GameBoard() {
        board = new Piece[boardSize][boardSize];
    }

    public Piece getPiece(Square square) {
        return board[square.getX()][square.getY()];
    }

    public Square[] getValidMoves(Square square) {
        board[square.getX()][square.getY()].getMoves(this, square);
        return null;
    }
}
