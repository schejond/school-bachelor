import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    private static int mazeHeight;
    private static int mazeWidth;
    private static int delay = 0;

    private final static String ANSI_RESET = "\u001B[0m";
    private final static String ANSI_RED = "\u001B[31m";
    private final static String ANSI_GREEN = "\u001B[32m";
    private final static String ANSI_YELLOW = "\u001B[33m";
    private final static String ANSI_PURPLE = "\u001B[35m";

    static void printMaze(final char[][] maze) {
        StringBuilder mazeString = new StringBuilder();
        mazeString.append("\u001Bc");
        for (int row = 0 ; row < mazeHeight ; row++) {
            for (int col = 0 ; col < mazeWidth ; col++) {
                final char charAt = maze[col][row];
                if (charAt == 'S' || charAt == 'E') {
                    mazeString.append(ANSI_RED).append(maze[col][row]).append(ANSI_RESET);
                    continue;
                }
                if (charAt == 'C') {
                    mazeString.append(ANSI_GREEN).append(maze[col][row]).append(ANSI_RESET);
                    continue;
                }
                if (charAt == 'O') {
                    mazeString.append(ANSI_YELLOW).append(maze[col][row]).append(ANSI_RESET);
                    continue;
                }
                if (charAt == 'T') {
                    mazeString.append(ANSI_PURPLE).append(maze[col][row]).append(ANSI_RESET);
                    continue;
                }
                mazeString.append(maze[col][row]);
            }
            mazeString.append("\n");
        }

        try {
            BufferedWriter sb = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(FileDescriptor.out), StandardCharsets.US_ASCII), 1204);
            sb.write(mazeString.toString());
            sb.flush();
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            TimeUnit.MILLISECONDS.sleep(delay);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void printLegend() {
        System.out.println("------------------------------------");
        System.out.println("LEGEND:");
        System.out.println(ANSI_RED + "S" + ANSI_RESET + " = Start");
        System.out.println(ANSI_RED + "E" + ANSI_RESET + " = End/Target");
        System.out.println(ANSI_YELLOW + "O" + ANSI_RESET + " = Open Node");
        System.out.println(ANSI_GREEN + "C" + ANSI_RESET + " = Closed Node");
        System.out.println(ANSI_PURPLE + "T" + ANSI_RESET + " = Found path from start to target");
        System.out.println("------------------------------------");
    }

    public static void main (String[] args) {
        if (args.length >= 2) {
            delay = Integer.valueOf(args[1]);
        }
        List<String> inputLines = new ArrayList<>();
        Scanner input = new Scanner(System.in);
        String line = "";
        while (input.hasNext()) {
            line = input.nextLine();
            if (line.startsWith("start")) {
                break;
            }
            inputLines.add(line);
        }

        String positions = line;
        positions += input.nextLine();
        Pattern p = Pattern.compile("\\d+");
        Matcher m = p.matcher(positions);
        List<Integer> coordinates = new ArrayList<>();
        while(m.find()) {
            coordinates.add(Integer.valueOf(m.group()));
        }
        final int startingCol = coordinates.get(0);
        final int startingRow = coordinates.get(1);
        final int endingCol = coordinates.get(2);
        final int endingRow= coordinates.get(3);

        char[][] maze = new char[inputLines.get(0).length()][inputLines.size()];
        for (int lineNmb = 0 ; lineNmb < inputLines.size() ; lineNmb++ ) {
            for (int colNmb = 0 ; colNmb < inputLines.get(lineNmb).length() ; colNmb++) {
                maze[colNmb][lineNmb] = inputLines.get(lineNmb).charAt(colNmb);
            }
        }

        maze[startingCol][startingRow] = 'S';
        maze[endingCol][endingRow] = 'E';

        if (args.length < 1) {
            System.out.println("AS FIRST ARGUMENT, PROVIDE THE NUMBER OF ALGORITHM WHICH YOU WANT TO EXECUTE");
            System.out.println("------------------------------");
            System.out.println("WHICH ALGO YOU WANT TO USE");
            System.out.println("1 FOR BFS");
            System.out.println("2 FOR DFS");
            System.out.println("3 FOR Random Search");
            System.out.println("4 FOR Greedy Search");
            System.out.println("5 FOR Dijkstra");
            System.out.println("6 FOR A*");
            System.out.println("------------------------------");
            return;
        }
        final int decision = Integer.valueOf(args[0]);

        mazeHeight = inputLines.size();
        mazeWidth = inputLines.get(0).length();

        Algorithm algo = new Algorithm(mazeWidth, mazeHeight, endingCol, endingRow, startingCol, startingRow);
        printMaze(maze);
        if (startingCol == endingCol && startingRow == endingRow) {
            System.out.println("TARGET IS " + algo.foundPathLength + " MOVES AWAY IF YOU FOLLOW ROUTE OF T'S");
            printLegend();
            return;
        }

        switch (decision) {
            //bfs
            case 1 : {
                algo.bfs(maze);
                break;
            }
            //dfs
            case 2 : {
                algo.dfsRecursive(maze, startingCol, startingRow);
                break;
            }
            //random
            case 3 : {
                algo.randomSearch(maze);
                break;
            }
            //greedy
            case 4 : {
                algo.greedySearch(maze);
                break;
            }
            //dijkstra
            case 5 : {
                algo.dijsktra(maze);
                break;
            }
            case 6 : {
                algo.aStar(maze);
                break;
            }
            default : {
                System.out.println("ACTION NOT DEFINED");
                return;
            }
        }
        printMaze(maze);
        System.out.println("TARGET IS " + (algo.foundPathLength+1) + " MOVES AWAY IF YOU FOLLOW ROUTE OF T'S");
        printLegend();
    }
}
