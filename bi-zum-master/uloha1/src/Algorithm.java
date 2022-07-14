import java.util.*;
import java.util.function.Function;

class Algorithm {
    private int mazeHeight;
    private int mazeWidth;
    private int endCol;
    private int endRow;
    private int startCol;
    private int startRow;

    int foundPathLength = 0;

    Algorithm(final int mazeWidth, final int mazeHeight, final int endCol, final int endRow, final int startCol, final int startRow) {
        this.mazeHeight = mazeHeight;
        this.mazeWidth = mazeWidth;
        this.endCol = endCol;
        this.endRow = endRow;
        this.startCol = startCol;
        this.startRow = startRow;
    }

    void bfs(char[][] maze) {
        Queue<PositionInMaze> queue = new LinkedList<>();
        queue.add(new PositionInMaze(startCol, startRow));

        int col = startCol;
        int row = startRow;
        PositionInMaze currentPosition;
        PositionInMaze[][] visitedPlacesInMaze = new PositionInMaze[mazeWidth][mazeHeight];
        while (!queue.isEmpty()) {
            currentPosition = queue.remove();
            col = currentPosition.getX();
            row = currentPosition.getY();
            visitedPlacesInMaze[col][row] = currentPosition;
            if (isPositionEmptyOrEnd(maze, col + 1, row)) {
                if (maze[col + 1][row] == 'E') {
                    break;
                }
                PositionInMaze newPosition = new PositionInMaze(col + 1, row);
                newPosition.setAncestorsCoordinates(currentPosition.getX(), currentPosition.getY());
                queue.add(newPosition);
                maze[col + 1][row] = 'O';
                Main.printMaze(maze);
            }
            if (isPositionEmptyOrEnd(maze, col - 1, row)) {
                if (maze[col - 1][row] == 'E') {
                    break;
                }
                PositionInMaze newPosition = new PositionInMaze(col - 1, row);
                newPosition.setAncestorsCoordinates(currentPosition.getX(), currentPosition.getY());
                queue.add(newPosition);
                maze[col - 1][row] = 'O';
                Main.printMaze(maze);
            }
            if (isPositionEmptyOrEnd(maze, col, row + 1)) {
                if (maze[col][row + 1] == 'E') {
                    break;
                }
                PositionInMaze newPosition = new PositionInMaze(col, row + 1);
                newPosition.setAncestorsCoordinates(currentPosition.getX(), currentPosition.getY());
                queue.add(newPosition);
                maze[col][row + 1] = 'O';
                Main.printMaze(maze);
            }
            if (isPositionEmptyOrEnd(maze, col, row - 1)) {
                if (maze[col][row - 1] == 'E') {
                    break;
                }
                PositionInMaze newPosition = new PositionInMaze(col, row - 1);
                newPosition.setAncestorsCoordinates(currentPosition.getX(), currentPosition.getY());
                queue.add(newPosition);
                maze[col][row - 1] = 'O';
                Main.printMaze(maze);
            }
            if (maze[col][row] != 'S') {
                maze[col][row] = 'C';
                Main.printMaze(maze);
            }
        }
        markPath(maze, visitedPlacesInMaze, col, row);
    }

    boolean dfsRecursive(char[][] maze, final int col, final int row) {
        if (maze[col][row] == 'E') {
            foundPathLength++;
            return true;
        }
        if (maze[col][row] != 'S') {
            maze[col][row] = 'O';
        }
        Main.printMaze(maze);
        //jsou mozne jen 4 smery tak je zavolam
        if (isPositionEmptyOrEnd(maze,col, row + 1)) {
            if (dfsRecursive(maze, col, row + 1)) {
                foundPathLength++;
                if (maze[col][row] == 'S') {
                    return true;
                }
                maze[col][row] = 'T';
                return true;
            }
        }
        if (isPositionEmptyOrEnd(maze,col, row - 1)) {
            if (dfsRecursive(maze, col, row - 1)) {
                foundPathLength++;
                if (maze[col][row] == 'S') {
                    return true;
                }
                maze[col][row] = 'T';
                return true;
            }
        }
        if (isPositionEmptyOrEnd(maze,col + 1, row)) {
            if (dfsRecursive(maze, col + 1, row)) {
                foundPathLength++;
                if (maze[col][row] == 'S') {
                    return true;
                }
                maze[col][row] = 'T';
                return true;
            }
        }
        if (isPositionEmptyOrEnd(maze,col - 1, row)) {
            if (dfsRecursive(maze, col - 1, row)) {
                foundPathLength++;
                if (maze[col][row] == 'S') {
                    return true;
                }
                maze[col][row] = 'T';
                return true;
            }
        }
        if (maze[col][row] != 'S') {
            maze[col][row] = 'C';
        }
        Main.printMaze(maze);
        return false;
    }

    void randomSearch(char[][] maze) {

        int curCol = startCol;
        int curRow = startRow;
        PositionInMaze[][] visitedPlacesInMaze = new PositionInMaze[mazeWidth][mazeHeight];
        List<PositionInMaze> openPlaces = new ArrayList<>();
        PositionInMaze currentPlace = new PositionInMaze(startCol, startRow);
        while(maze[curCol][curRow] != 'E') {
            visitedPlacesInMaze[curCol][curRow] = currentPlace;
            //put neighbours to openPositions
            if (isPositionEmptyOrEnd(maze, curCol + 1, curRow)) {
                if (maze[curCol + 1][curRow] == 'E') {
                    break;
                }
                maze[curCol + 1][curRow] = 'O';
                PositionInMaze openPlace = new PositionInMaze(curCol + 1, curRow);
                openPlace.setAncestorsCoordinates(curCol, curRow);
                openPlaces.add(openPlace);

                Main.printMaze(maze);
            }
            if (isPositionEmptyOrEnd(maze, curCol - 1, curRow)) {
                if (maze[curCol - 1][curRow] == 'E') {
                    break;
                }
                maze[curCol - 1][curRow] = 'O';
                PositionInMaze openPlace = new PositionInMaze(curCol - 1, curRow);
                openPlace.setAncestorsCoordinates(curCol, curRow);
                openPlaces.add(openPlace);

                Main.printMaze(maze);
            }
            if (isPositionEmptyOrEnd(maze, curCol, curRow + 1)) {
                if (maze[curCol][curRow + 1] == 'E') {
                    break;
                }
                maze[curCol][curRow + 1] = 'O';
                PositionInMaze openPlace = new PositionInMaze(curCol, curRow + 1);
                openPlace.setAncestorsCoordinates(curCol, curRow);
                openPlaces.add(openPlace);

                Main.printMaze(maze);
            }
            if (isPositionEmptyOrEnd(maze, curCol, curRow - 1)) {
                if (maze[curCol][curRow - 1] == 'E') {
                    break;
                }
                maze[curCol][curRow - 1] = 'O';
                PositionInMaze openPlace = new PositionInMaze(curCol, curRow - 1);
                openPlace.setAncestorsCoordinates(curCol, curRow);
                openPlaces.add(openPlace);

                Main.printMaze(maze);
            }
            //closeCurrent position
            if (maze[curCol][curRow] != 'S') {
                maze[curCol][curRow] = 'C';
                removeFromPlaces(curCol, curRow, openPlaces);
            }

            //select random open position
            PositionInMaze nextPlaceToVisit = getRandomNextPlaceToVisit(openPlaces);
            curCol = nextPlaceToVisit.getX();
            curRow = nextPlaceToVisit.getY();
            currentPlace = nextPlaceToVisit;
        }
        markPath(maze, visitedPlacesInMaze, curCol, curRow);
    }

    void greedySearch(char[][] maze) {
        PositionInMaze[][] visitedPositionsMaze = new PositionInMaze[mazeWidth][mazeHeight];
        PriorityQueue<PositionInMaze> openPositions = new PriorityQueue<>(new HeuristicValueComparator());

        PositionInMaze startingPosition = new PositionInMaze(startCol, startRow);
        visitedPositionsMaze[startCol][startRow] = startingPosition;
        openPositions.add(startingPosition);

        int col = startCol;
        int row = startRow;
        PositionInMaze currentPosition;
        while (!openPositions.isEmpty()) {
            currentPosition = openPositions.poll();

            col = currentPosition.getX();
            row = currentPosition.getY();

            if (isPositionEmptyOrEnd(maze, col + 1, row)) {
                if (maze[col + 1][row] == 'E') {
                    break;
                }
                PositionInMaze nextPosition = new PositionInMaze(col + 1, row);
                nextPosition.setAncestorsCoordinates(col, row);
                visitedPositionsMaze[col + 1][row] = nextPosition;
                openPositions.add(nextPosition);
                maze[col + 1][row] = 'O';
                Main.printMaze(maze);
            }

            if (isPositionEmptyOrEnd(maze, col - 1, row)) {
                if (maze[col - 1][row] == 'E') {
                    break;
                }
                PositionInMaze nextPosition = new PositionInMaze(col - 1, row);
                nextPosition.setAncestorsCoordinates(col, row);
                visitedPositionsMaze[col - 1][row] = nextPosition;
                openPositions.add(nextPosition);
                maze[col - 1][row] = 'O';
                Main.printMaze(maze);
            }

            if (isPositionEmptyOrEnd(maze, col, row + 1)) {
                if (maze[col][row + 1] == 'E') {
                    break;
                }
                PositionInMaze nextPosition = new PositionInMaze(col, row + 1);
                nextPosition.setAncestorsCoordinates(col, row);
                visitedPositionsMaze[col][row + 1] = nextPosition;
                openPositions.add(nextPosition);
                maze[col][row + 1] = 'O';
                Main.printMaze(maze);
            }

            if (isPositionEmptyOrEnd(maze, col, row - 1)) {
                if (maze[col][row - 1] == 'E') {
                    break;
                }
                PositionInMaze nextPosition = new PositionInMaze(col, row - 1);
                nextPosition.setAncestorsCoordinates(col, row);
                visitedPositionsMaze[col][row - 1] = nextPosition;
                openPositions.add(nextPosition);
                maze[col][row - 1] = 'O';
                Main.printMaze(maze);
            }

            if (maze[col][row] != 'S') {
                maze[col][row] = 'C';
                Main.printMaze(maze);
            }
        }
        markPath(maze, visitedPositionsMaze, col, row);
    }

    void dijsktra(char[][] maze) {
        PositionInMaze[][] visitedPositionsMaze = new PositionInMaze[mazeWidth][mazeHeight];
        PriorityQueue<PositionInMaze> openPositions = new PriorityQueue<>(new DistanceComparator());

        PositionInMaze startingPosition = new PositionInMaze(startCol, startRow);
        visitedPositionsMaze[startCol][startRow] = startingPosition;
        openPositions.add(startingPosition);

        int col = startCol;
        int row = startRow;
        PositionInMaze currentPosition;
        while (!openPositions.isEmpty()) {
            currentPosition = openPositions.poll();

            col = currentPosition.getX();
            row = currentPosition.getY();

            if (maze[col][row] == 'E') {
                break;
            }

            if (isPositionEmptyEndOrOpen(maze, col + 1, row)) {
                if (maze[col + 1][row] != 'O') {
                    PositionInMaze nextPosition = new PositionInMaze(col + 1, row);
                    nextPosition.setAncestorsCoordinates(col, row);
                    nextPosition.setDistanceFromStart(currentPosition.getDistanceFromStart() + 1);
                    visitedPositionsMaze[col + 1][row] = nextPosition;
                    openPositions.add(nextPosition);
                }
                if (maze[col + 1][row] != 'E' && maze[col + 1][row] != 'O') {
                    maze[col + 1][row] = 'O';
                    Main.printMaze(maze);
                }
                if (visitedPositionsMaze[col + 1][row].adjustDistanceFromStart(currentPosition.getDistanceFromStart() + 1)) {
                    visitedPositionsMaze[col + 1][row].setAncestorsCoordinates(col, row);
                }
            }

            if (isPositionEmptyEndOrOpen(maze, col - 1, row)) {
                if (maze[col - 1][row] != 'O') {
                    PositionInMaze nextPosition = new PositionInMaze(col - 1, row);
                    nextPosition.setAncestorsCoordinates(col, row);
                    nextPosition.setDistanceFromStart(currentPosition.getDistanceFromStart() + 1);
                    visitedPositionsMaze[col - 1][row] = nextPosition;
                    openPositions.add(nextPosition);
                }
                if (maze[col - 1][row] != 'E' && maze[col - 1][row] != 'O') {
                    maze[col - 1][row] = 'O';
                    Main.printMaze(maze);
                }
                if (visitedPositionsMaze[col - 1][row].adjustDistanceFromStart(currentPosition.getDistanceFromStart() + 1)) {
                    visitedPositionsMaze[col - 1][row].setAncestorsCoordinates(col, row);
                }
            }

            if (isPositionEmptyEndOrOpen(maze, col, row + 1)) {
                if (maze[col][row + 1] != 'O') {
                    PositionInMaze nextPosition = new PositionInMaze(col, row + 1);
                    nextPosition.setAncestorsCoordinates(col, row);
                    nextPosition.setDistanceFromStart(currentPosition.getDistanceFromStart() + 1);
                    visitedPositionsMaze[col][row + 1] = nextPosition;
                    openPositions.add(nextPosition);
                }
                if (maze[col][row + 1] != 'E' && maze[col][row + 1] != 'O') {
                    maze[col][row + 1] = 'O';
                    Main.printMaze(maze);
                }
                if (visitedPositionsMaze[col][row + 1].adjustDistanceFromStart(currentPosition.getDistanceFromStart() + 1)) {
                    visitedPositionsMaze[col][row + 1].setAncestorsCoordinates(col, row);
                }
            }

            if (isPositionEmptyEndOrOpen(maze, col, row - 1)) {
                if (maze[col][row - 1] != 'O') {
                    PositionInMaze nextPosition = new PositionInMaze(col, row - 1);
                    nextPosition.setAncestorsCoordinates(col, row);
                    nextPosition.setDistanceFromStart(currentPosition.getDistanceFromStart() + 1);
                    visitedPositionsMaze[col][row - 1] = nextPosition;
                    openPositions.add(nextPosition);
                }
                if (maze[col][row - 1] != 'E' && maze[col][row - 1] != 'O') {
                    maze[col][row - 1] = 'O';
                    Main.printMaze(maze);
                }
                if (visitedPositionsMaze[col][row - 1].adjustDistanceFromStart(currentPosition.getDistanceFromStart() + 1)) {
                    visitedPositionsMaze[col][row - 1].setAncestorsCoordinates(col, row);
                }
            }

            if (maze[col][row] != 'S') {
                maze[col][row] = 'C';
                Main.printMaze(maze);
            }
        }
        foundPathLength++;
        markPath(maze, visitedPositionsMaze, col, row);
    }

    void aStar(char[][] maze) {
        PositionInMaze[][] visitedPositionsMaze = new PositionInMaze[mazeWidth][mazeHeight];
        PriorityQueue<PositionInMaze> openPositions = new PriorityQueue<>(new AStarComparator());

        PositionInMaze startingPosition = new PositionInMaze(startCol, startRow);
        visitedPositionsMaze[startCol][startRow] = startingPosition;
        openPositions.add(startingPosition);

        int col = startCol;
        int row = startRow;
        PositionInMaze currentPosition;
        while (!openPositions.isEmpty()) {
            currentPosition = openPositions.poll();

            col = currentPosition.getX();
            row = currentPosition.getY();

            if (isPositionEmptyOrEnd(maze, col + 1, row)) {
                if (maze[col + 1][row] == 'E') {
                    break;
                }
                PositionInMaze nextPosition = new PositionInMaze(col + 1, row);
                nextPosition.setAncestorsCoordinates(col, row);
                nextPosition.setDistanceFromStart(currentPosition.getDistanceFromStart() + 1);
                visitedPositionsMaze[col + 1][row] = nextPosition;
                openPositions.add(nextPosition);
                maze[col + 1][row] = 'O';
                Main.printMaze(maze);
            } else if (maze[col + 1][row] == 'O') {
                if (visitedPositionsMaze[col + 1][row].getDistanceFromStart() + visitedPositionsMaze[col + 1][row].getHeuristicValue() > (currentPosition.getDistanceFromStart() + 1 + currentPosition.getHeuristicValue())) {
                    visitedPositionsMaze[col + 1][row].setAncestorsCoordinates(col, row);
                }
//                if (visitedPositionsMaze[col + 1][row].adjustDistanceFromStart(currentPosition.getDistanceFromStart() + 1)) {
//                    visitedPositionsMaze[col + 1][row].setAncestorsCoordinates(col, row);
//                }
            }

            if (isPositionEmptyOrEnd(maze, col - 1, row)) {
                if (maze[col - 1][row] == 'E') {
                    break;
                }
                PositionInMaze nextPosition = new PositionInMaze(col - 1, row);
                nextPosition.setAncestorsCoordinates(col, row);
                nextPosition.setDistanceFromStart(currentPosition.getDistanceFromStart() + 1);
                visitedPositionsMaze[col - 1][row] = nextPosition;
                openPositions.add(nextPosition);
                maze[col - 1][row] = 'O';
                Main.printMaze(maze);
            } else if (maze[col - 1][row] == 'O') {
                if (visitedPositionsMaze[col - 1][row].getDistanceFromStart() + visitedPositionsMaze[col - 1][row].getHeuristicValue() > (currentPosition.getDistanceFromStart() + 1 + currentPosition.getHeuristicValue())) {
                    visitedPositionsMaze[col - 1][row].setAncestorsCoordinates(col, row);
                }
//                if (visitedPositionsMaze[col - 1][row].adjustDistanceFromStart(currentPosition.getDistanceFromStart() + 1)) {
//                    visitedPositionsMaze[col - 1][row].setAncestorsCoordinates(col, row);
//                }
            }

            if (isPositionEmptyOrEnd(maze, col, row + 1)) {
                if (maze[col][row + 1] == 'E') {
                    break;
                }
                PositionInMaze nextPosition = new PositionInMaze(col, row + 1);
                nextPosition.setAncestorsCoordinates(col, row);
                nextPosition.setDistanceFromStart(currentPosition.getDistanceFromStart() + 1);
                visitedPositionsMaze[col][row + 1] = nextPosition;
                openPositions.add(nextPosition);
                maze[col][row + 1] = 'O';
                Main.printMaze(maze);
            } else if (maze[col][row + 1] == 'O') {
                if (visitedPositionsMaze[col][row + 1].getDistanceFromStart() + visitedPositionsMaze[col][row + 1].getHeuristicValue() > (currentPosition.getDistanceFromStart() + 1 + currentPosition.getHeuristicValue())) {
                    visitedPositionsMaze[col][row + 1].setAncestorsCoordinates(col, row);
                }
//                if (visitedPositionsMaze[col][row + 1].adjustDistanceFromStart(currentPosition.getDistanceFromStart() + 1)) {
//                    visitedPositionsMaze[col][row + 1].setAncestorsCoordinates(col, row);
//                }
            }

            if (isPositionEmptyOrEnd(maze, col, row - 1)) {
                if (maze[col][row - 1] == 'E') {
                    break;
                }
                PositionInMaze nextPosition = new PositionInMaze(col, row - 1);
                nextPosition.setAncestorsCoordinates(col, row);
                nextPosition.setDistanceFromStart(currentPosition.getDistanceFromStart() + 1);
                visitedPositionsMaze[col][row - 1] = nextPosition;
                openPositions.add(nextPosition);
                maze[col][row - 1] = 'O';
                Main.printMaze(maze);
            } else if (maze[col][row - 1] == 'O') {
                if (visitedPositionsMaze[col][row - 1].getDistanceFromStart() + visitedPositionsMaze[col][row - 1].getHeuristicValue() > (currentPosition.getDistanceFromStart() + 1 + currentPosition.getHeuristicValue())) {
                    visitedPositionsMaze[col][row - 1].setAncestorsCoordinates(col, row);
                }
//                if (visitedPositionsMaze[col][row - 1].adjustDistanceFromStart(currentPosition.getDistanceFromStart() + 1)) {
//                    visitedPositionsMaze[col][row - 1].setAncestorsCoordinates(col, row);
//                }
            }

            if (maze[col][row] != 'S') {
                maze[col][row] = 'C';
                Main.printMaze(maze);
            }
        }
        markPath(maze, visitedPositionsMaze, col, row);
    }

    //also checks if the coordinates are valid
    private boolean isPositionEmptyOrEnd(char[][] maze, final int col, final int row) {
        return col <= mazeWidth && col >= 0 && row <= mazeHeight && row >= 0
                && maze[col][row] != 'C' && maze[col][row] != 'X' && maze[col][row] != 'S' && maze[col][row] != 'O';
    }

    //also checks if the coordinates are valid
    private boolean isPositionEmptyEndOrOpen(char[][] maze, final int col, final int row) {
        return col <= mazeWidth && col >= 0 && row <= mazeHeight && row >= 0
                && maze[col][row] != 'C' && maze[col][row] != 'X' && maze[col][row] != 'S';
    }

    private void markPath(char[][] maze, final PositionInMaze[][] positionMaze, final int col, final int row) {
        if (maze[col][row] == 'S') {
            return;
        }
        if (maze[col][row] != 'E') {
            foundPathLength++;
            maze[col][row] = 'T';
        }
        if (maze[col][row] == 'E') {
            foundPathLength++;
        }
        final PositionInMaze currentPlace = positionMaze[col][row];
        markPath(maze, positionMaze, currentPlace.getAncestorCol(), currentPlace.getAncestorRow());
    }

    private void removeFromPlaces(final int col, final int row, final List<PositionInMaze> places) {
        for (int i = 0 ; i < places.size() ; i++) {
            if (places.get(i).getX() == col && places.get(i).getY() == row) {
                places.remove(i);
                return;
            }
        }
    }

    private PositionInMaze getRandomNextPlaceToVisit(List<PositionInMaze> openPlaces) {
        Random rand = new Random();
        return openPlaces.get(rand.nextInt(openPlaces.size()));
    }

    private class HeuristicValueComparator implements Comparator<PositionInMaze> {
        @Override
        public int compare(PositionInMaze o1, PositionInMaze o2) {
            return Integer.compare(o1.getHeuristicValue(), o2.getHeuristicValue());
        }
    }

    private class DistanceComparator implements Comparator<PositionInMaze> {
        @Override
        public int compare(PositionInMaze o1, PositionInMaze o2) {
            return Integer.compare(o1.getDistanceFromStart(), o2.getDistanceFromStart());
        }
    }

    private class AStarComparator implements Comparator<PositionInMaze> {
        @Override
        public int compare(PositionInMaze o1, PositionInMaze o2) {
            final int value1 = o1.getHeuristicValue() + o1.getDistanceFromStart();
            final int value2 = o2.getHeuristicValue() + o2.getDistanceFromStart();
            return Integer.compare(value1, value2);
        }
    }

    private class PositionInMaze {
        private int x;
        private int y;
        private int ancestorCol;
        private int ancestorRow;
        private int distanceFromStart;
        private int heuristicValue;

        PositionInMaze(final int x, final int y) {
            this.x = x;
            this.y = y;
            this.distanceFromStart = 0;
            this.heuristicValue = Math.abs(endCol - x) + Math.abs(endRow - y);
        }

        int getHeuristicValue() {
            return heuristicValue;
        }

        int getDistanceFromStart() {
            return distanceFromStart;
        }

        void setDistanceFromStart(int distanceFromStart) {
            this.distanceFromStart = distanceFromStart;
        }

        //returns true if the distance has been changed
        boolean adjustDistanceFromStart(final int distanceFromStart) {
            if (this.distanceFromStart > distanceFromStart) {
                this.distanceFromStart = distanceFromStart;
                return true;
            }
            return false;
        }

        void setAncestorsCoordinates(final int col, final int row) {
            this.ancestorCol = col;
            this.ancestorRow = row;
        }

        int getX() {
            return x;
        }

        int getY() {
            return y;
        }

        int getAncestorCol() {
            return ancestorCol;
        }

        int getAncestorRow() {
            return ancestorRow;
        }
    }
}
