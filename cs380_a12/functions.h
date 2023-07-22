#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <sstream>
#include <vector>

#include "data.h"

#define UNUSED(x) (void)x;

// Returns result of addition of all genes in a chromosome
template<typename Gene>
struct Fitness_Accumulate
{
    int operator()(const std::vector<Gene>& genes) const
    {
        int sum = 0; 

        for (auto gene : genes) {
            sum += gene.getValue();
        }

        return sum;
    }
};

// Returns a measure in percentages that indicates the fitness of  
// a particular chromosome into a particular solution.
// Fittest chromosome has all genes equal 1.
template<typename Gene>
struct Fitness_Nbits
{
    int operator()(const std::vector<Gene>& genes) const
    {
        float percent = 0.0f;

        for (auto gene : genes) {
            if (gene.getValue()) {
                percent += gene.getValue();
            }
        }

        return static_cast<int>(percent/genes.size()*100);
    }
};

// Returns a measure in percentages that indicates the fitness of  
// a particular chromosome into a particular solution of 8 queens 
// problem
template<typename Gene>
struct Fitness_8queens
{
    int operator()(const std::vector<Gene>& genes) const
    {
        for (auto gene : genes) {
            if (gene.getValue() == 8) return 0;
        }

        return 100;
    }
};


namespace AI
{
    // Crossover methods for the genetic algorithm
    enum CrossoverMethod { Middle, Random  };
    
    // Simplest gene seeding class/function
    struct Seed
    {
        int operator()(int p = 0) const
        {
            return p;
        }
    };

    // Gene seeding class/function with a fixed value
    template<int Val = 0>
    struct Seed_Value
    {
        int operator()(int /* p */ = 0) const
        {
            return Val;
        }
    };

    // Gene random seeding class/function
    template<int Max>
    struct Seed_Random
    {
        int operator()(int /* p */ = 0) const
        {
            return std::rand() % Max;
        }
    };

    // Gene class
    template<typename T = int, typename S = Seed>
    class Gene
    {
        T value;

    public:
        Gene(int p = 0)
            : value{ S()(p) }
        {
        }

        T getValue() const
        {
            return value;
        }

        void setValue(T v)
        {
            value = v;
        }

        friend std::ostream& operator<<(std::ostream& os, const Gene& rhs)
        {
            os << rhs.value;
            return os;
        }
    };

    // Chromosome class
    template<typename Gene, typename Fitness, size_t Size>
    class Chromosome
    {
        std::vector<Gene> genes;
        int fitness;

    public:

        using gene_type = Gene;

        static const size_t size = Size;

        Chromosome()
            : genes(Size), fitness{ Fitness()(genes) }
        {
        }

        std::vector<Gene>& getGenes()
        {
            return genes;
        }

        void setGenes(const std::vector<Gene>& v)
        {
            genes = v;
            fitness = Fitness()(genes);
        }

        Gene getGene(size_t i) const
        {
            return genes[i];
        }

        void setGene(size_t i, const Gene& v)
        {
            genes[i] = v;
            fitness = Fitness()(genes);
        }

        int getFitness() const
        {
            return fitness;
        }

        // Select a random mutation point and change 
        // gene at the mutation point
        void randomMutation()
        {
            setGene(std::rand() % Chromosome::size, Gene());
        }

        // Copy genes from a source
        void copyGenesFrom(Chromosome& src)
        {
            std::copy(src.genes.begin(), src.genes.end(), genes.begin());
            fitness = Fitness()(genes);
        }

        friend std::ostream& operator<<(std::ostream& os, 
            const Chromosome& rhs)
        {
            os << '[';
            for (auto it = rhs.genes.begin(); it != rhs.genes.end(); ++it)
                os << *it << (it + 1 != rhs.genes.end() ? "," : "");
            os << "]=" << rhs.fitness;
            return os;
        }
    };

    // Individual class
    template<typename Chromosome>
    class Individual
    {
        Chromosome chromosome;
 
    public:

        using chromosome_type = Chromosome;
        using gene_type = typename Chromosome::gene_type;

        Individual()
            : chromosome{ }
        {
        }

        Chromosome& getChromosome()
        {
            return chromosome;
        }

        std::vector<gene_type>& getGenes()
        {
            return chromosome.getGenes();
        }

        void setGenes(const std::vector<gene_type>& v)
        {
            chromosome.setGenes(v);
        }

        gene_type getGene(size_t i) const
        {
            return chromosome.getGene(i);
        }

        void copyGenesFrom(Individual& individual)
        {
            chromosome.copyGenesFrom(individual.chromosome);
        }

        void setGene(size_t i, gene_type gene)
        {
            chromosome.setGene(i, gene);
        }

        int getFitness() const
        {
            return chromosome.getFitness();
        }

        friend std::ostream& operator<<(std::ostream& os, Individual& rhs)
        {
            os << rhs.chromosome;
            return os;
        }
    };

    // Population class
    template<typename Individual>
    class Population
    {
        std::vector<Individual> individuals;
        Individual* fittest;

    public:
        Population(size_t size = 0)
            : individuals{ }, fittest{ nullptr }
        {
            if (size)
            {
                individuals.resize(size);
                updateFittest();
            }
        }

        size_t getSize() const
        {
            return individuals.size();
        }

        Individual& getIndividual(size_t i)
        {
            return individuals[i];
        }

        Individual* getFittest() const
        {
            return fittest;
        }

        void updateFittest()
        {
            if (getSize())
            {
                fittest = &getIndividual(0);

                for (int i = 1; i < static_cast<int>(getSize()); ++i)
                {
                    if (individuals[i].getFitness() > fittest->getFitness())
                        fittest = &getIndividual(i);
                }
            }
            else
                fittest = nullptr;
        }

        friend std::ostream& operator<<(std::ostream& os, Population& rhs)
        {
            os << " = " << rhs.getFittest()->getFitness() << std::endl;
            for (size_t i = 0; i < rhs.getSize(); ++i)
                os << "  " << i << ':' << rhs.getIndividual(i) << std::endl;
            return os;
        }
    };
 
    // Genetic Algorithm class
    template<typename Individual>
    class GeneticAlgorithm
    {
        Population<Individual>* population;
        int generation;
 
    public:
        GeneticAlgorithm()
            : population{ nullptr }, generation{ 0 }
        {
        }

        ~GeneticAlgorithm()
        {
            delete population;
        }

        Individual* getFittest() const
        {
            return population->getFittest();
        }

        // Implementation of the Roulette Wheel Selection. The probability of an 
        // individual to be selected is directly proportional to its fitness.
        Population<Individual>* selection(size_t sizeOfPopulation)
        {
            if (!this->population)
                this->setPopulation(new Population<Individual>(sizeOfPopulation));

            auto newGeneration = new Population<Individual>(sizeOfPopulation);

            // Play roulette
            int sum_fitness = 0;
            for (unsigned int i = 0; i < sizeOfPopulation; ++i)
                sum_fitness += this->population->getIndividual(i).getFitness();

            for (unsigned int i = 0; i < sizeOfPopulation; ++i)
            {
                int rnd = std::rand() % sum_fitness;
                unsigned int I = 0;
                for (; I < sizeOfPopulation; ++I)
                {
                    rnd -= this->population->getIndividual(I).getFitness();
                    if (rnd <= 0)
                        break;
                }
                // Copy genes
                newGeneration->getIndividual(i).copyGenesFrom(
                    this->population->getIndividual(I));
            }
            return newGeneration;
        }

        // Crossover parents genes
        void crossover(Population<Individual>* newGeneration, 
            CrossoverMethod crossoverMethod)
        {
            int sizeOfChromosome = (int)newGeneration->getIndividual(0).getGenes().size();

            int crossOverPoint = 0;
            if (crossoverMethod == CrossoverMethod::Middle)
                crossOverPoint = sizeOfChromosome / 2;
            else if (crossoverMethod == CrossoverMethod::Random)
                crossOverPoint = (int)(std::rand() % sizeOfChromosome / 2);

            for (unsigned int j = 0; j < newGeneration->getSize() - 1; j += 2)
            {
                // Swap values among pairs
                for (int i = 0; i < crossOverPoint; ++i)
                {
                    // Swap genes
                    auto t = newGeneration->getIndividual(j + 1).getGene(i);
                    newGeneration->getIndividual(j + 1).setGene(i,
                        newGeneration->getIndividual(j).getGene(i));
                    newGeneration->getIndividual(j).setGene(i, t);
                }
            }
        }

        // Do mutation of genes under a random probability
        void mutation(Population<Individual>* newGeneration, 
            int mutationProbability)
        {
            int sizeOfPopulation = newGeneration ?
                static_cast<int>(newGeneration->getSize()) : 0;

            for (int j = 0; j < sizeOfPopulation; ++j)
                // Select a random mutation point and flip 
                // genes at the mutation point
                if (std::rand() % 100 < mutationProbability)
                    newGeneration->getIndividual(j)
                    .getChromosome().randomMutation();
        }

        // Replace existing population if any with a new generation
        void setPopulation(Population<Individual>* newGeneration)
        {
            this->population = newGeneration;
            this->population->updateFittest();
        }

        // Start the search
        void run(size_t sizeOfPopulation = 100, int mutationProbability = 70, 
                    CrossoverMethod crossoverMethod = CrossoverMethod::Middle, 
                    std::ostringstream* os = nullptr)
        {
            this->generation = 0;

            this->setPopulation(new Population<Individual>(sizeOfPopulation));

            // While loop until the solution is found
            while (this->next(mutationProbability, crossoverMethod, os))
            {
            }
        }

        // Continue the search
        bool next(int mutationProbability, CrossoverMethod crossoverMethod, 
                    std::ostringstream* os)
        {
            if (!population)
                this->setPopulation(new Population<Individual>(population->getSize()));

            if (os)
                *os << "\nGeneration: " + this->generation +
                os->str() + '\n';

            // Stop the search when either max fitness of solution or 
            // max limit for generations achived
            if (population->getFittest()->getFitness() == 100
                || generation > 10000)
                return false;

            // Select fittest parents to mate and produce a new generation
            auto newGeneration = selection(population->getSize());

            // Recombination: creates new individuals by taking the
            // chromosomes from the fittest members of the
            // population and modifing these chromosomes using
            // crossover and/or mutation.
            this->crossover(newGeneration, crossoverMethod);
            this->mutation(newGeneration, mutationProbability);

            // Set population with new generation
            this->setPopulation(newGeneration);

            this->generation++;

            return true;
        }
    };

} // end namespace

#endif