package cz.fit.cvut;

import java.util.Objects;
import java.util.Random;

public class Position {
    private static final int IMAGE_DIMENSIONS = 16;
    private int positionX;
    private int positionY;

    //generates random position
    Position() {
        Random r = new Random();
        positionX = r.nextInt(IMAGE_DIMENSIONS);
        positionY = r.nextInt(IMAGE_DIMENSIONS);
    }

    void generateNewPositions() {
        Random r = new Random();
        positionX = r.nextInt(IMAGE_DIMENSIONS);
        positionY = r.nextInt(IMAGE_DIMENSIONS);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Position position = (Position) o;
        return positionX == position.positionX &&
                positionY == position.positionY;
    }

    @Override
    public int hashCode() {
        return Objects.hash(positionX, positionY);
    }

    int getPositionX() {
        return positionX;
    }

    void setPositionX(int positionX) {
        this.positionX = positionX;
    }

    int getPositionY() {
        return positionY;
    }

    void setPositionY(int positionY) {
        this.positionY = positionY;
    }
}
