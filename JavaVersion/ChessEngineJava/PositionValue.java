package ChessEngineJava;

public class PositionValue {
    public double valueCurrentPlayer;
    public double valueOtherPlayer;


    public double getValueCurrentPlayer() {
        return valueCurrentPlayer;
    }

    public double getValueOtherPlayer() {
        return valueOtherPlayer;
    }

    public double getDifference() {
        return valueCurrentPlayer - valueOtherPlayer;
    }

    public PositionValue(double valueCurrentPlayer, double valueOtherPlayer) {
        this.valueCurrentPlayer = valueCurrentPlayer;
        this.valueOtherPlayer = valueOtherPlayer;
    }
}
