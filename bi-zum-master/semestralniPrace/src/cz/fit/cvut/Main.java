package cz.fit.cvut;

import javafx.util.Pair;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.io.File;

public class Main {
    private final static int IMAGE_DIMENSIONS = 16;
    private final static float PROBABILITY_OF_CROSSOVER = 0.33f;
    private final static float PROBABILITY_OF_CROSSOVER_SELECTED = 0.44f;
    private final static float PROBABILITY_OF_MUTATION = 0.04f;
    private final static float PROBABILITY_OF_CATASTROPHE = 0.005f;
    private final static int INDIVIDUALS_FOR_HILL_CLIMBING = 25;
    private final static int MAXIMUM_NUMBER_OF_SAME_GENERATIONS = 15;
    private final static int STARTING_NUMBER_OF_INDIVIDUALS_IN_POPULATION = 1500;
    private final static int MAXIMUM_NUMBER_OF_ITERATIONS = 20000;

    public static void main(String[] args) {
        //args check
        if (args.length != 1) {
            System.out.println("Wrong usage of this program. Use with 1 argument (location of the dataset with the alphabet)");
            return;
        }

        String datasetPath = args[0];

        List<Letter> letters = new ArrayList<>();
        //load each letter in given dataset
        File dir = new File(datasetPath);
        if (!dir.isDirectory()) {
            System.out.println("Given path is not a directory!");
            return;
        }
        File[] directoryListing = dir.listFiles();
        if (directoryListing != null) {
            for (File child : directoryListing) {
                if (!child.canRead()) {
                    System.out.println("Can't read file '" + child.getName() + "' from the dataset");
                    return;
                }
                Letter letter = new Letter(String.valueOf(child.getName().charAt(0)));
                try {
                    BufferedImage bufferedImage = ImageIO.read(child);
                    int width = bufferedImage.getWidth();
                    int height = bufferedImage.getHeight();
                    if (width != IMAGE_DIMENSIONS || height != 16) {
                        System.out.println("Input image has wrong dimensions.");
                        return;
                    }
                    //set letter values
                    for (int col = 0; col < width; col++) {
                        for (int line = 0; line < height; line++) {
                            Color c = new Color(bufferedImage.getRGB(line, col));
                            //if the pixel is not white or black
                            if ((c.getRed() != 255 || c.getGreen() != 255 || c.getBlue() != 255)
                                    && (c.getRed() != 0 || c.getGreen() != 0 || c.getBlue() != 0)) {
                                System.out.println("Picture contains pixels of other color than BLACK or WHITE");
                                return;
                            }
                            if (c.getRed() == 0 && c.getBlue() == 0 && c.getGreen() == 0) {
                                letter.setCoordinateToBlackColor(col, line);
                            }
                        }
                    }
                    letters.add(letter);
//                    System.out.println("pismeno: " + letter.getName());
//                    for (int col = 0 ; col < width ; col++) {
//                        for (int line = 0 ; line < height ; line++) {
//                            if (letter.getCoordinateValue(col, line)) {
//                                System.out.print("W");
//                            }
//                            else {
//                                System.out.print("B");
//                            }
//                        }
//                        System.out.println();
//                    }
                } catch (Exception e) {
                    System.out.println("Exception occured while getting pixels from the image. " + e.getMessage());
                    return;
                }
            }
        } else {
            System.out.println("Given directory is empty!");
            return;
        }

        //initialization of the population populace
        List<Individual> population = new ArrayList<>();
        Random r = new Random();
        for (int i = 0; i < STARTING_NUMBER_OF_INDIVIDUALS_IN_POPULATION; i++) {
            Individual individual = new Individual();
            //compute fitness for first generation
            Logic.computeFitness(individual, letters);
            population.add(individual);
        }
        int iteration = 0;
        int generationsWithSameFitnessCount = 0;
        int lastGenerationFitness = 0;
        while (true) {
            if (iteration >= MAXIMUM_NUMBER_OF_ITERATIONS) {
                System.out.println("Genetic algorithm reached maximum number of populations!");
                System.out.println("Currently best individuals fitness value is " + lastGenerationFitness);
                return;
            }
            if (iteration % 10 == 0) {
                System.out.println("Started iteration number: " + iteration);
            }
            //find individual with best fitness value
            int highestFitness = 0;
            Individual highestFitnessIndividual = null;
            for (Individual individual : population) {
                if (individual.getFitness() > highestFitness) {
                    highestFitness = individual.getFitness();
                    highestFitnessIndividual = individual;
                }
            }

            //solution found
            if (highestFitness == letters.size()) {
                if (highestFitnessIndividual == null) {
                    throw new RuntimeException("Found solution but the highestFitnessIndividual object was not correctly associated!");
                }
                System.out.println("-------------------------------------------------------");
                System.out.println("Found solution in the " + iteration + "th iteration.");
                System.out.println("Individual has positions: ");
                for (Position position : highestFitnessIndividual.getIndividual()) {
                    System.out.print("[" + position.getPositionX() + " ; " + position.getPositionY() + "] ");
                }
                System.out.println();
                System.out.println("-------------------------------------------------------");
                System.out.println("Traces for individual letters:");
                for (Letter letter : letters) {
                    System.out.println(letter.getName() + " -> " + letter.getColorTrace(highestFitnessIndividual));
                }
                System.out.println("-------------------------------------------------------");
                return;
            }

            if (highestFitness == lastGenerationFitness) {
                generationsWithSameFitnessCount++;
            } else {
                lastGenerationFitness = highestFitness;
                generationsWithSameFitnessCount = 0;
            }
            if (generationsWithSameFitnessCount == MAXIMUM_NUMBER_OF_SAME_GENERATIONS) {
                generationsWithSameFitnessCount = 0;
                population = new ArrayList<>(Logic.catastropheThenClimbTheHill(population, letters));
                iteration++;
                continue;
            }
            //select best individuals
            List<Individual> selectedIndividuals = Logic.tournamentSelection(population, population.size() / 20);
//            List<Individual> topkIndividuals = Logic.getTopKIndividuals(population, 10);
            List<Individual> newPopulation = new ArrayList<>();
            for (int i = 0; i < selectedIndividuals.size(); i++) {
                Individual individual = selectedIndividuals.get(i);
                if (i < selectedIndividuals.size() - 1 && r.nextFloat() <= PROBABILITY_OF_CROSSOVER_SELECTED) {
                    Individual individual2 = selectedIndividuals.get(i + 1);
                    Pair<Individual, Individual> childs = individual.crossover(individual2, letters, iteration);
                    newPopulation.add(childs.getKey());
                    newPopulation.add(childs.getValue());
                    continue;
                }
                newPopulation.add(individual);
            }
            int numberOfIndividualsForHillclimbing = INDIVIDUALS_FOR_HILL_CLIMBING;
            while (newPopulation.size() < population.size()) {
                //choose random individual
                Individual randomIndividual = population.get(r.nextInt(population.size()));
                if (r.nextFloat() <= PROBABILITY_OF_CROSSOVER) {
                    Individual randomIndividual2 = population.get(r.nextInt(population.size()));
                    Pair<Individual, Individual> childs = randomIndividual.crossover(randomIndividual2, letters, iteration);
                    newPopulation.add(childs.getKey());
                    newPopulation.add(childs.getValue());
                    continue;
                }
                if (r.nextFloat() <= PROBABILITY_OF_MUTATION) {
                    randomIndividual.mutate();
                    randomIndividual.repair(iteration);
                    Logic.computeFitness(randomIndividual, letters);
                    newPopulation.add(randomIndividual);
                    continue;
                }
                if (numberOfIndividualsForHillclimbing > 0) {
                    numberOfIndividualsForHillclimbing--;
                    newPopulation.add(Logic.climbTheHill(randomIndividual, letters));
                    continue;
                }
                newPopulation.add(randomIndividual);
            }
            //srovnani poctu generaci
            while (newPopulation.size() > population.size()) {
                newPopulation.remove(population.size());
            }
            //-----------Catastrophe-------------------------------------------------
            if (r.nextFloat() <= PROBABILITY_OF_CATASTROPHE) {
                System.out.println("Catastrophe!");
                Collections.shuffle(newPopulation, r);
                newPopulation = newPopulation.subList(0, population.size() / 2 + 1);
                newPopulation.addAll(selectedIndividuals);
                while (newPopulation.size() != population.size()) {
                    Individual individual = new Individual();
                    Logic.computeFitness(individual, letters);
                    newPopulation.add(individual);
                }
            }

            // replace population with the new one
            population = newPopulation;
            iteration++;
        }
    }
}
