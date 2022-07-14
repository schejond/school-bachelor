package cz.fit.cvut;

import java.util.*;
import java.util.stream.Collectors;

class Logic {
    //catastrophe, which eliminates 3/4 of the population
    // and for the best individuals from original population it starts climb the hill method
    static List<Individual> catastropheThenClimbTheHill(List<Individual> population, List<Letter> letters) {
        System.out.println("Population doesn't evolve fast enough. Doing catastrophe + climb the hill");
        //shuffle original population
        Random r = new Random();
        Collections.shuffle(population, r);
        List<Individual> newPopulation = new ArrayList<>(population.subList(0, population.size() / 4 + 1));
        ///tournament selection and application of climbTheHill method on it's results
        List<Individual> selectedIndividuals = tournamentSelection(population, population.size()/20);
        List<Individual> inTheHillIndividuals = new ArrayList<>();
        for (Individual selectedIndividual : selectedIndividuals) {
            inTheHillIndividuals.add(climbTheHill(selectedIndividual, letters));
        }
        newPopulation.addAll(inTheHillIndividuals);
        //fill the the rest with random individuals
        while (newPopulation.size() != population.size()) {
            Individual individual = new Individual();
            Logic.computeFitness(individual, letters);
            newPopulation.add(individual);
        }

        return newPopulation;
    }

    //climbs the hill for selected individual until it finds better position for him
    static Individual climbTheHill(Individual individual, final List<Letter> letters) {
        final int MAX_NUMBER_OF_STEPS = 8;
        final int IMAGE_DIMENSION = 16;
        final int originalFitness = individual.getFitness();
        int steps = 1;
        //try to go right
        while (individual.getFitness() <= originalFitness && steps < MAX_NUMBER_OF_STEPS) {
            //tries x steps to the right
            for (Position position : individual.getIndividual()) {
                if (position.getPositionX() + steps < IMAGE_DIMENSION) {
                    position.setPositionX(position.getPositionX() + steps);
                    computeFitness(individual, letters);
                    if (individual.getFitness() > originalFitness) {
                        return climbTheHill(individual, letters);
                    } else {
                        position.setPositionX(position.getPositionX() - steps);
                    }
                }
            }

            //tries x steps to the left
            for (Position position : individual.getIndividual()) {
                if (position.getPositionX() - steps >= 0) {
                    position.setPositionX(position.getPositionX() - steps);
                    computeFitness(individual, letters);
                    if (individual.getFitness() > originalFitness) {
                        return climbTheHill(individual, letters);
                    } else {
                        position.setPositionX(position.getPositionX() + steps);
                    }
                }
            }

            //tries x steps up
            for (Position position : individual.getIndividual()) {
                if (position.getPositionY() + steps < IMAGE_DIMENSION) {
                    position.setPositionY(position.getPositionY() + steps);
                    computeFitness(individual, letters);
                    if (individual.getFitness() > originalFitness) {
                        return climbTheHill(individual, letters);
                    } else {
                        position.setPositionY(position.getPositionY() - steps);
                    }
                }
            }

            //tries x steps down
            for (Position position : individual.getIndividual()) {
                if (position.getPositionY() - steps >= 0) {
                    position.setPositionY(position.getPositionY() - steps);
                    computeFitness(individual, letters);
                    if (individual.getFitness() > originalFitness) {
                        return climbTheHill(individual, letters);
                    } else {
                        position.setPositionY(position.getPositionY() + steps);
                    }
                }
            }
            steps++;
        }
        computeFitness(individual, letters);
        return individual;
    }

    public static List<Individual> getTopKIndividuals(List<Individual> population, final int k) {
        return population.stream().sorted(new FitnessComparator()).collect(Collectors.toList()).subList(0, k);
    }

    //fitness = number of letters we can distinguish thanks to this individual
    static void computeFitness(Individual individual, final List<Letter> letters) {
        int fitness = 0;
        Map<Integer, Boolean> indexOfLetterToIfItHasNoDuplicate = new HashMap<>();
        for (int i = 0; i < letters.size(); i++) {
            indexOfLetterToIfItHasNoDuplicate.put(i, true);
        }
        for (int i = 0; i < letters.size(); i++) {
            if (i < letters.size() - 1 && indexOfLetterToIfItHasNoDuplicate.get(i)) {
                boolean[] currentLetterTrace = getBWTrace(individual, letters.get(i));
                for (int j = i + 1; j < letters.size(); j++) {
                    boolean[] otherLetterTrace = getBWTrace(individual, letters.get(j));
                    //if currentLetterTrace is same as otherLetterTrace
                    if (compareBWTraces(currentLetterTrace, otherLetterTrace)) {
                        indexOfLetterToIfItHasNoDuplicate.put(i, false);
                        indexOfLetterToIfItHasNoDuplicate.put(j, false);
                    }
                }
            }
        }
        for (boolean bool : indexOfLetterToIfItHasNoDuplicate.values()) {
            if (bool) {
                fitness++;
            }
        }
        individual.setFitness(fitness);
    }

    private static boolean[] getBWTrace(final Individual individual, final Letter letter) {
        boolean[] trace = new boolean[5];
        for (int i = 0; i < individual.getIndividual().size(); i++) {
            trace[i] = letter.getCoordinateValue(individual.getIndividual().get(i).getPositionX(), individual.getIndividual().get(i).getPositionY());
        }
        return trace;
    }

    //select n individuals
    static List<Individual> tournamentSelection(final List<Individual> individuals, final int n) {
        Random r = new Random();
        List<Individual> selected = new ArrayList<>();
        final int k = individuals.size() / 6;

        for (int j = 0; j < n; j++) {
            int highestFitnessValue = Integer.MIN_VALUE;
            Individual individualWithHighestFitness = null;
            for (int i = 0; i < k; i++) {
                Individual fighter = individuals.get(r.nextInt(individuals.size()));
                if (highestFitnessValue < fighter.getFitness()) {
                    highestFitnessValue = fighter.getFitness();
                    individualWithHighestFitness = fighter;
                }
            }
            selected.add(individualWithHighestFitness);
        }

        return selected;
    }

    private static boolean compareBWTraces(final boolean[] trace1, final boolean[] trace2) {
        for (int i = 0; i < trace1.length; i++) {
            if (trace1[i] != trace2[i]) {
                return false;
            }
        }
        return true;
    }

    private static class FitnessComparator implements Comparator<Individual> {
        @Override
        public int compare(Individual i1, Individual i2) {
            return Float.compare(i2.getFitness(), i1.getFitness());
        }
    }
}
