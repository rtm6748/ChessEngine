public class Move {
    Piece piece;
    Square square;

    public Move(Piece piece, Square square) {
        this.piece = piece;
        this.square = square;
    }

    public Piece getPiece() {
        return piece;
    }

    public Square getSquare() {
        return square;
    }

    @Override
    public String toString() {
        return piece.toString() + " to " + square.toString();
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Move move) {
            return this.piece.equals(move.getPiece()) && this.square.equals(move.getSquare());
        }
        return false;
    }
}
