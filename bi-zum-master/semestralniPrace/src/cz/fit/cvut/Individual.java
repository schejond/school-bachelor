package cz.fit.cvut;

import javafx.util.Pair;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

class Individual {
    private static final int IMAGE_DIMENSIONS = 16;
    private List<Position> individual = new ArrayList<>(5);
    private int fitness;
    private int[][] selectedPositions = new int[IMAGE_DIMENSIONS][IMAGE_DIMENSIONS];

    //create new individual with unique positions
    Individual() {
        for (int i = 0; i < 5; i++) {
            individual.add(new Position());
        }
        //initialize selectedPositions
        for (int i = 0 ; i < 16 ; i++) {
            for (int j = 0 ; j < 16 ; j++) {
                selectedPositions[i][j] = -1;
            }
        }
        this.repair(0);
        this.fitness = 0;
    }

    void repair(final int population) {
        for (Position pos : this.individual) {
            //if this position is already marked
            while (this.selectedPositions[pos.getPositionX()][pos.getPositionY()] == population) {
                pos.generateNewPositions();
            }
            this.selectedPositions[pos.getPositionX()][pos.getPositionY()] = population;
        }
    }

    //1 point crossover
    Pair<Individual, Individual> crossover(Individual otherIndividual, final List<Letter> letters, final int population) {
        Random r = new Random();
        final int point = r.nextInt(5);
        Individual offspring1 = new Individual();
        Individual offspring2 = new Individual();

        for (int i = 0 ; i < this.getIndividual().size() ; i++) {
            if (i < point) {
                offspring1.getIndividual().get(i).setPositionX(this.getIndividual().get(i).getPositionX());
                offspring1.getIndividual().get(i).setPositionY(this.getIndividual().get(i).getPositionY());
                offspring2.getIndividual().get(i).setPositionX(otherIndividual.getIndividual().get(i).getPositionX());
                offspring2.getIndividual().get(i).setPositionX(otherIndividual.getIndividual().get(i).getPositionX());
            } else {
                offspring1.getIndividual().get(i).setPositionX(otherIndividual.getIndividual().get(i).getPositionX());
                offspring1.getIndividual().get(i).setPositionY(otherIndividual.getIndividual().get(i).getPositionY());
                offspring2.getIndividual().get(i).setPositionX(this.getIndividual().get(i).getPositionX());
                offspring2.getIndividual().get(i).setPositionX(this.getIndividual().get(i).getPositionX());
            }
        }
        offspring1.repair(population);
        offspring2.repair(population);
        Logic.computeFitness(offspring1, letters);
        Logic.computeFitness(offspring2, letters);

//-----------deterministic crowding-----------------------------
        Individual betterFirstIndividual;
        Individual betterSecondIndividual;
        if (point > 2) {
            if (this.getFitness() > offspring1.getFitness()) {
                betterFirstIndividual = this;
            } else {
                betterFirstIndividual = offspring1;
            }
            if (otherIndividual.getFitness() > offspring2.getFitness()) {
                betterSecondIndividual = otherIndividual;
            } else {
                betterSecondIndividual = offspring2;
            }
        } else {
            if (this.getFitness() > offspring2.getFitness()) {
                betterFirstIndividual = this;
            } else {
                betterFirstIndividual = offspring2;
            }
            if (otherIndividual.getFitness() > offspring1.getFitness()) {
                betterSecondIndividual = otherIndividual;
            } else {
                betterSecondIndividual = offspring1;
            }
        }
        return new Pair<>(betterFirstIndividual, betterSecondIndividual);
    }

    void mutate() {
        Random r = new Random();
        for (Position pos : this.getIndividual()) {
            if (r.nextBoolean()) {
                final Position newPosition = new Position();
                pos.setPositionX(newPosition.getPositionX());
                pos.setPositionY(newPosition.getPositionY());
            }
        }
    }

    List<Position> getIndividual() {
        return individual;
    }

    int getFitness() {
        return fitness;
    }

    void setFitness(int fitness) {
        this.fitness = fitness;
    }
}
