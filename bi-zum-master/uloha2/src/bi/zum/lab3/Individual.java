package bi.zum.lab3;

import cz.cvut.fit.zum.api.ga.AbstractEvolution;
import cz.cvut.fit.zum.api.ga.AbstractIndividual;
import cz.cvut.fit.zum.data.Edge;
import cz.cvut.fit.zum.data.StateSpace;
import cz.cvut.fit.zum.util.Pair;
import java.util.Random;


/**
 * @author schejond
 */
public class Individual extends AbstractIndividual {

    private double fitness = Double.NaN;
    private AbstractEvolution evolution;
    
    private boolean[] individual;

    /**
     * Creates a new individual
     * 
     * @param evolution The evolution object
     * @param randomInit <code>true</code> if the individual should be
     * initialized randomly (we do wish to initialize if we copy the individual)
     */
    public Individual(AbstractEvolution evolution, boolean randomInit) {
        this.evolution = evolution;
        final int length = evolution.getNodesCount();
        this.individual = new boolean[length];
        if(randomInit) {
            Random r = new Random();
            for (int i = 0 ; i < length ; i++) {
                this.individual[i] = r.nextBoolean();
            }
            this.repair();
        }
    }

    @Override
    public boolean isNodeSelected(int j) {
        return individual[j];
    }

    /**
     * Evaluate the value of the fitness function for the individual. After
     * the fitness is computed, the <code>getFitness</code> may be called
     * repeatedly, saving computation time.
     */
    @Override
    public void computeFitness() {
        int length = evolution.getNodesCount();
        double fitnessValue = StateSpace.nodesCount();
        
        for (int i = 0 ; i < this.individual.length ; i++) {
            if (this.individual[i]) {
                fitnessValue -= 1;
            }
        }
        
        this.fitness = fitnessValue;
    }

    /**
     * Only return the computed fitness value
     *
     * @return value of fitness function
     */
    @Override
    public double getFitness() {
        return this.fitness;
    }

    /**
     * Does random changes in the individual's genotype, taking mutation
     * probability into account.
     * 
     * @param mutationRate Probability of a bit being inverted, i.e. a node
     * being added to/removed from the vertex cover.
     */
    @Override
    public void mutate(double mutationRate) {
        Random r = new Random();
        for (int i = 0 ; i < this.individual.length ; i++) {
            if (r.nextDouble() < mutationRate) {
                final boolean currentValue = this.individual[i];
                this.individual[i] = !currentValue;
            }
        }
        this.repair();
    }
    
    /**
     * Crosses the current individual over with other individual given as a
     * parameter, yielding a pair of offsprings.
     * 
     * @param other The other individual to be crossed over with
     * @return A couple of offspring individuals
     */
    @Override
    public Pair crossover(AbstractIndividual other) {

        Pair<Individual,Individual> result = new Pair();
        Individual otherIndividual = (Individual)other;
        
        Random r = new Random();
        //najdi dve mista zlomu
        int prvniZlom;
        int druhyZlom;
        do {
            prvniZlom = r.nextInt(this.individual.length);
            druhyZlom = r.nextInt(this.individual.length);
        } while (prvniZlom >= druhyZlom);
        Individual firstOffspring = new Individual(evolution, false);
        Individual secondOffspring = new Individual(evolution, false);
        
        //jeden zlom
        for (int i = 0 ; i < this.individual.length ; i++) {
            if (i < prvniZlom) {
                firstOffspring.individual[i] = this.individual[i];
            } else {
                firstOffspring.individual[i] = otherIndividual.isNodeSelected(i);
            } 
        }
        //dva zlomy
        for (int i = 0 ; i < this.evolution.getNodesCount() ; i++) {
            if (i < prvniZlom) {
                secondOffspring.individual[i] = this.individual[i];
            } else if (i < druhyZlom) {
                secondOffspring.individual[i] = otherIndividual.isNodeSelected(i);
            } else {
                secondOffspring.individual[i] = this.individual[i];
            }
        }
        
        result.a = firstOffspring;
        result.b = secondOffspring;
        
        return result;
    }

    
    /**
     * When you are changing an individual (eg. at crossover) you probably don't
     * want to affect the old one (you don't want to destruct it). So you have
     * to implement "deep copy" of this object.
     *
     * @return identical individual
     */
    @Override
    public Individual deepCopy() {
        Individual newOne = new Individual(evolution, false);
        newOne.individual = new boolean[this.individual.length];
        for (int i = 0 ; i < this.individual.length ; i++) {
            newOne.individual[i] = this.individual[i];
        }

        newOne.fitness = this.fitness;
        return newOne;
    }

    /**
     * Return a string representation of the individual.
     *
     * @return The string representing this object.
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
            for (int i = 0 ; i < this.individual.length ; i++) {
                sb.append(i + " = " + this.individual[i]);
                if (i != this.individual.length - 1) {
                    sb.append(",");
                }
            }
        
//        sb.append(super.toString());

        return sb.toString();
    }
    
    /**
     * Repairs the genotype to make it valid, i.e. ensures all the edges are in
     * the vertex cover.
     */
    private void repair() {
        Random r = new Random();
        /* We iterate over all the edges */
        for (Edge e : StateSpace.getEdges()) {
            // pokud hrana nevychazi/nevchazi ani do jednoho prvku
            if (!this.individual[e.getFromId()] && !this.individual[e.getToId()]) {
                //randomly add from or to vrchol
                if (r.nextBoolean()) {
                    this.individual[e.getFromId()] = true;
                } else {
                    this.individual[e.getToId()] = true;
                }
            }
        }
    }
}
