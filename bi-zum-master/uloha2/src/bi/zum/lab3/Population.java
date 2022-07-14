package bi.zum.lab3;

import cz.cvut.fit.zum.api.ga.AbstractEvolution;
import cz.cvut.fit.zum.api.ga.AbstractIndividual;
import cz.cvut.fit.zum.api.ga.AbstractPopulation;
import cz.cvut.fit.zum.data.StateSpace;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

/**
 * @author schejond
 */
public class Population extends AbstractPopulation {
    
    public Population(AbstractEvolution evolution, int size) {
        individuals = new Individual[size];
        for (int i = 0; i < individuals.length; i++) {
            individuals[i] = new Individual(evolution, true);
            individuals[i].computeFitness();
        }
    }

    /**
     * Method to select individuals from population
     *
     * @param count The number of individuals to be selected
     * @return List of selected individuals
     */
    public List<AbstractIndividual> selectIndividuals(int count) {
        Random r = new Random();
        ArrayList<AbstractIndividual> selected = new ArrayList<AbstractIndividual>();
        final int k = 40;//this.individuals.length/4;

        for (int j = 0; j < count; j++) {
            double highestFitnessValue = Double.NEGATIVE_INFINITY;
            AbstractIndividual individualWithHighestFitness = null;
            for (int i = 0; i < k; i++) {
                AbstractIndividual fighter = this.individuals[r.nextInt(this.individuals.length)];
                if (highestFitnessValue < fighter.getFitness()) {
                    highestFitnessValue = fighter.getFitness();
                    individualWithHighestFitness = fighter;
                }
            }
            selected.add(individualWithHighestFitness);
        }
        
        return selected;
    }
}
