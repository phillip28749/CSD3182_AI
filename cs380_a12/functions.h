/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 12
\date 07-23-2023
\brief

    The file includes necessary function declaration definition for Genetic Algorithm

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
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
    /*!******************************************************************************/
    /*
        @brief
            operator() overload

        @param genes
            vector genes

        @return
            total genes
    */
    /********************************************************************************/
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
    /*!******************************************************************************/
    /*
        @brief
            operator() overload

        @param genes
            vector genes

        @return
            percentage indicate the fitness chromosome
    */
    /********************************************************************************/
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
    /*!******************************************************************************/
    /*
        @brief
            operator() overload

        @param genes
            vector genes

        @return
            percentage indicate the fitness of individual chromosome
    */
    /********************************************************************************/
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
        /*!******************************************************************************/
        /*
            @brief
                operator() overload

            @param p
                default seed val

            @return
                seed val
        */
        /********************************************************************************/
        int operator()(int p = 0) const
        {
            return p;
        }
    };

    // Gene seeding class/function with a fixed value
    template<int Val = 0>
    struct Seed_Value
    {
        /*!******************************************************************************/
        /*
            @brief
                operator() overload

            @param p
                default seed val

            @return
                seed val
        */
        /********************************************************************************/
        int operator()(int /* p */ = 0) const
        {
            return Val;
        }
    };

    // Gene random seeding class/function
    template<int Max>
    struct Seed_Random
    {
        /*!******************************************************************************/
        /*
            @brief
                operator() overload

            @param p
                default seed val

            @return
                random generate number
        */
        /********************************************************************************/
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
        /*!******************************************************************************/
        /*
            @brief
                constructor

            @param p
                seed val
        */
        /********************************************************************************/
        Gene(int p = 0)
            : value{ S()(p) }
        {
        }

        /*!******************************************************************************/
        /*
            @brief
                getter function

            @return
                value
        */
        /********************************************************************************/
        T getValue() const
        {
            return value;
        }

        /*!******************************************************************************/
        /*
            @brief
                setter function

            @param v
                set value
        */
        /********************************************************************************/
        void setValue(T v)
        {
            value = v;
        }

        /*!******************************************************************************/
        /*
            @brief
                os function

            @param os
                os stream

            @param rhs
                rhs

            @return
                osstream
        */
        /********************************************************************************/
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

        /*!******************************************************************************/
        /*
            @brief
                constructor
        */
        /********************************************************************************/
        Chromosome()
            : genes(Size), fitness{ Fitness()(genes) }
        {
        }

        /*!******************************************************************************/
        /*
            @brief
                getter function

            @return
                genes
        */
        /********************************************************************************/
        std::vector<Gene>& getGenes()
        {
            return genes;
        }

        /*!******************************************************************************/
        /*
            @brief
                setter function

            @param v
                set value
        */
        /********************************************************************************/
        void setGenes(const std::vector<Gene>& v)
        {
            genes = v;
            fitness = Fitness()(genes);
        }

        /*!******************************************************************************/
        /*
            @brief
                getter function for individual gene

            @return
                genes
        */
        /********************************************************************************/
        Gene getGene(size_t i) const
        {
            return genes[i];
        }

        /*!******************************************************************************/
        /*
            @brief
                setter function for individual gene

            @param i
                index

            @param v
                set value
        */
        /********************************************************************************/
        void setGene(size_t i, const Gene& v)
        {
            genes[i] = v;
            fitness = Fitness()(genes);
        }

        /*!******************************************************************************/
        /*
            @brief
                getter function

            @return
                fitness
        */
        /********************************************************************************/
        int getFitness() const
        {
            return fitness;
        }


        /********************************************************************************/
        /*!
            @brief Perform a random mutation on the chromosome.

            Selects a random gene in the chromosome and changes its value to a new random value.
        */
        /********************************************************************************/
        void randomMutation()
        {
            setGene(std::rand() % Chromosome::size, Gene());
        }


        /********************************************************************************/
        /*!
            @brief Copy the genes from another chromosome.

            @param src The source chromosome whose genes will be copied into this chromosome.
        */
        /********************************************************************************/
        void copyGenesFrom(Chromosome& src)
        {
            std::copy(src.genes.begin(), src.genes.end(), genes.begin());
            fitness = Fitness()(genes);
        }

        /********************************************************************************/
        /*!
            @brief Overloaded ostream operator for outputting the chromosome.

            @param os The output stream.
            @param rhs The chromosome to be outputted.
            @return The output stream containing the representation of the chromosome.
        */
        /********************************************************************************/
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

        /********************************************************************************/
        /*!
            @brief Default constructor.

            Creates an individual with a default-initialized chromosome.
        */
        /********************************************************************************/
        Individual()
            : chromosome{ }
        {
        }

        /********************************************************************************/
        /*!
            @brief Get the chromosome of the individual.

            @return A reference to the chromosome representing the individual's potential solution.
        */
        /********************************************************************************/
        Chromosome& getChromosome()
        {
            return chromosome;
        }

        /********************************************************************************/
        /*!
            @brief Get the vector of genes in the chromosome.

            @return A reference to the vector of genes in the individual's chromosome.
        */
        /********************************************************************************/
        std::vector<gene_type>& getGenes()
        {
            return chromosome.getGenes();
        }

        /********************************************************************************/
        /*!
            @brief Set the genes in the chromosome.

            @param v The vector of genes to be set in the individual's chromosome.
        */
        /********************************************************************************/
        void setGenes(const std::vector<gene_type>& v)
        {
            chromosome.setGenes(v);
        }

        /********************************************************************************/
        /*!
            @brief Get an individual gene from the chromosome.

            @param i The index of the gene to retrieve.
            @return The gene at the specified index in the individual's chromosome.
        */
        /********************************************************************************/
        gene_type getGene(size_t i) const
        {
            return chromosome.getGene(i);
        }

        /********************************************************************************/
        /*!
            @brief Copy the genes from another individual.

            @param individual The source individual whose chromosome's genes will be copied into this individual's chromosome.
        */
        /********************************************************************************/
        void copyGenesFrom(Individual& individual)
        {
            chromosome.copyGenesFrom(individual.chromosome);
        }


        /********************************************************************************/
        /*!
            @brief Set an individual gene in the chromosome.

            @param i The index of the gene to set.
            @param gene The value of the gene to be set in the individual's chromosome.
        */
        /********************************************************************************/
        void setGene(size_t i, gene_type gene)
        {
            chromosome.setGene(i, gene);
        }

        /********************************************************************************/
        /*!
            @brief Get the fitness value of the individual.

            @return The fitness value of the individual's chromosome, indicating how good it is as a potential solution.
        */
        /********************************************************************************/
        int getFitness() const
        {
            return chromosome.getFitness();
        }

        /********************************************************************************/
        /*!
            @brief Overloaded ostream operator for outputting the individual.

            @param os The output stream.
            @param rhs The individual to be outputted.
            @return The output stream containing the representation of the individual.
        */
        /********************************************************************************/
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

        /********************************************************************************/
        /*!
            @brief Constructor.

            Creates an empty population with no individuals and sets the fittest pointer to nullptr.
        */
        /********************************************************************************/
        Population(size_t size = 0)
            : individuals{ }, fittest{ nullptr }
        {
            if (size)
            {
                individuals.resize(size);
                updateFittest();
            }
        }

        /********************************************************************************/
        /*!
            @brief Get the size of the population.

            @return The number of individuals in the population.
        */
        /********************************************************************************/
        size_t getSize() const
        {
            return individuals.size();
        }

        /********************************************************************************/
        /*!
            @brief Get an individual from the population.

            @param i The index of the individual to retrieve.
            @return A reference to the individual at the specified index in the population.
        */
        /********************************************************************************/
        Individual& getIndividual(size_t i)
        {
            return individuals[i];
        }

        /********************************************************************************/
        /*!
            @brief Get the fittest individual in the population.

            @return A pointer to the fittest individual in the population, or nullptr if the population is empty.
        */
        /********************************************************************************/
        Individual* getFittest() const
        {
            return fittest;
        }

        /********************************************************************************/
        /*!
            @brief Update the pointer to the fittest individual in the population.

            This function finds the fittest individual in the population and updates the fittest pointer accordingly.
        */
        /********************************************************************************/
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

        /********************************************************************************/
        /*!
            @brief Overloaded ostream operator for outputting the population.

            @param os The output stream.
            @param rhs The population to be outputted.
            @return The output stream containing the representation of the population.
        */
        /********************************************************************************/
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
        /********************************************************************************/
        /*!
            @brief Default constructor.

            Creates a genetic algorithm with an initial population set to nullptr and generation count set to 0.
        */
        /********************************************************************************/
        GeneticAlgorithm()
            : population{ nullptr }, generation{ 0 }
        {
        }

        /********************************************************************************/
        /*!
            @brief Destructor.

            Frees the memory occupied by the current population (if any).
        */
        /********************************************************************************/
        ~GeneticAlgorithm()
        {
            delete population;
        }

        /********************************************************************************/
        /*!
            @brief Get the fittest individual in the current population.

            @return A pointer to the fittest individual in the population, or nullptr if the population is empty.
        */
        /********************************************************************************/
        Individual* getFittest() const
        {
            return population->getFittest();
        }

        /********************************************************************************/
        /*!
            @brief Perform the selection step of the genetic algorithm.

            The selection process uses the Roulette Wheel Selection method to select individuals for mating and creating the new generation.

            @param sizeOfPopulation The size of the population to be created in the next generation.
            @return A pointer to the new population created after selection.
        */
        /********************************************************************************/
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

        /********************************************************************************/
        /*!
            @brief Perform the crossover step of the genetic algorithm.

            The crossover process combines the genes of selected parents to create new individuals for the new generation.

            @param newGeneration A pointer to the new population created after selection, where crossover will be applied.
            @param crossoverMethod The method used for crossover (e.g., Middle or Random).
        */
        /********************************************************************************/
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

        /********************************************************************************/
        /*!
            @brief Perform the mutation step of the genetic algorithm.

            The mutation process introduces random changes in the genes of individuals with a certain probability.

            @param newGeneration A pointer to the new population created after selection and crossover, where mutation will be applied.
            @param mutationProbability The probability of mutation for each individual in the population (in percentage).
        */
        /********************************************************************************/
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


        /********************************************************************************/
        /*!
            @brief Set the current population with a new generation.

            This function replaces the existing population with a new generation.

            @param newGeneration A pointer to the new population (generation).
        */
        /********************************************************************************/
        void setPopulation(Population<Individual>* newGeneration)
        {
            this->population = newGeneration;
            this->population->updateFittest();
        }

        /********************************************************************************/
        /*!
            @brief Start the genetic algorithm.

            This function initiates the search process using the genetic algorithm.

            @param sizeOfPopulation The size of the initial population to be created.
            @param mutationProbability The probability of mutation for each individual in the population (in percentage).
            @param crossoverMethod The method used for crossover (e.g., Middle or Random).
            @param os A pointer to an output stringstream (optional) to record the progress of the genetic algorithm.
        */
        /********************************************************************************/
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

        /********************************************************************************/
        /*!
            @brief Continue the genetic algorithm to the next generation.

            This function advances the genetic algorithm to the next generation and performs the selection, crossover, and mutation steps.

            @param mutationProbability The probability of mutation for each individual in the population (in percentage).
            @param crossoverMethod The method used for crossover (e.g., Middle or Random).
            @param os A pointer to an output stringstream (optional) to record the progress of the genetic algorithm.
            @return A boolean value indicating if the search process should continue to the next generation. Returns false when either the max fitness of a solution or the max limit for generations is achieved.
        */
        /********************************************************************************/
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
            delete population;
            
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