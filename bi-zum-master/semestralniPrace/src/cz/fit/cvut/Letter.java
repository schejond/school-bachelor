package cz.fit.cvut;

class Letter {
    private String name;
    private boolean[][] pictureColors; //white = true, //black = false;

    Letter(String name) {
        this.name = name;
        int IMAGE_DIMENSIONS = 16;
        this.pictureColors = new boolean[IMAGE_DIMENSIONS][IMAGE_DIMENSIONS];
        //inicialize the picture scheme
        for (int col = 0; col < IMAGE_DIMENSIONS; col++) {
            for (int line = 0; line < IMAGE_DIMENSIONS; line++) {
                pictureColors[col][line] = true;
            }
        }
    }

    String getColorTrace(final Individual individual) {
        StringBuilder trace = new StringBuilder();
        for (Position position : individual.getIndividual()) {
            if (pictureColors[position.getPositionX()][position.getPositionY()]) {
                trace.append("W");
            } else {
                trace.append("B");
            }
        }
        return trace.toString();
    }

    void setCoordinateToBlackColor(final int col, final int line) {
        pictureColors[col][line] = false;
    }

    boolean getCoordinateValue(final int col, final int line) {
        return pictureColors[col][line];
    }

    String getName() {
        return name;
    }
}
