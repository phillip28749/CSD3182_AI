#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stack>
#include <algorithm>

#include "data.h"

#define UNUSED(x) (void)x;

// This is a domain specific functor class that is used to find a solution 
// for a one-dimensional Sudoku by using Backtracking algorithm.
// Functor returns next location which is not occupied yet on the map.
// If no location found, returns Location<>{ nullptr, 0 }.
class NextLocation_Sudoku1D
{
    AI::MapInt1D* map;

public:

    NextLocation_Sudoku1D(void* map)
        : map{ static_cast<AI::MapInt1D*>(map) }
    {
    }

    AI::Location<> operator()() const
    {
        // Your code ...
        for (int i = 0; i < map->size; ++i)
            if (map->base[i] == 0)
                return AI::Location<int>(map->base, i);

        return AI::Location<int>{ nullptr, 0 };
    }
};

// This is a domain specific functor class that is used to find a solution 
// for a two-dimensional Sudoku by using Backtracking algorithm.
// Functor returns next location which is not occupied yet on the map.
// If no location found, returns Location<>{ nullptr, 0 }.
class NextLocation_Sudoku2D
{
    AI::MapInt2D* map;

public:

    NextLocation_Sudoku2D(void* map)
        : map{ static_cast<AI::MapInt2D*>(map) }
    {
    }

    AI::Location<> operator()() const
    {
        for (int j = 0; j < map->height; ++j)
            for (int i = 0; i < map->width; ++i)
                if (map->base[j * map->width + i] == 0)
                    return AI::Location<>{map->base, j* map->width + i};

        return AI::Location<>{ nullptr, 0 };
    }
};

// This is a domain specific functor class that is used to find a solution 
// for a one-dimensional Sudoku by using Backtracking algorithm.
// Functor returns next candidate for a specified location on the map.
// If no candidate found, returns 0.
class NextCandidate_Sudoku1D
{
    AI::MapInt1D* map;

public:

    NextCandidate_Sudoku1D(void* map)
        : map{ static_cast<AI::MapInt1D*>(map) }
    {
    }

    int indexOf(int value) {

        for (int i = 0; i < map->size; ++i) {
            if (map->base[i] == value) {
                return i;
            }
        }

        return -1;
    }

    int operator()(AI::Location<> location)
    {
        int v = map->base[location.getIndex()];
        while (v < map->size)
        {
            v++;
            if (indexOf(v) == -1)
                return v;
        }

        return 0;
    }
};

// This is a domain specific functor class that is used to find a solution 
// for a two-dimensional Sudoku by using Backtracking algorithm.
// Functor returns next candidate for a specified location on the map.
// If no candidate found, returns 0.
class NextCandidate_Sudoku2D
{
    AI::MapInt2D* map;

public:

    NextCandidate_Sudoku2D(void* map)
        : map{ static_cast<AI::MapInt2D*>(map) }
    {
    }

    int indexOf(int Map[], int value, int index) {

        int row = index % map->width;
        int col = index / map->height;
        int startrow = row - row % 3;
        int startcol = col - col % 3;
        int pos = -1;

        for (int i = 0; i < map->width; ++i) {

            if (Map[map->width * i + row] == value) {
                
                pos = value;
            }
        }

        for (int i = 0; i < map->height; ++i) {

            if (Map[map->height * i + col] == value) {

                pos = value;
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (Map[(i + startcol) * map->width + (j + startrow)] == value) {
                    pos = value;
                }
            }
        }

        return pos;
    }

    int operator()(AI::Location<> location)
    {
        int v = map->base[location.getIndex()];
        while (v < map->height)
        {
            v++;
            if (indexOf(map->base, v, location.getIndex()) == -1)
                return v;
        }

        return 0;
    }
};


namespace AI
{
    // Template class of the Backtracking algorithm.
    // Parameter NL defines domain-specific NextLocation functor. 
    // Parameter NC defines domain-specific NextCandidate functor. 
    template<typename NL, typename NC>
    class Backtracking
    {
        std::stack<Location<>> stack;
        NL next_location;
        NC next_candidate;
 
    public:
        Backtracking(void* map = nullptr)
            : next_location{ map }, next_candidate{ map }
        {
        }

        // Find solution in a blocking mode
        void run()
        {
            while (solve() == false) {
            }
        }

        // One iteration of the search. Used by run() in a blocking running mode
        // or can be called by timer in an non-blocking run
        bool solve()
        {
            if (this->stack.empty())
            {
                // Set the stack to start the search
                auto location = this->next_location.operator()();
                if (location.notFound())
                    return true;
                stack.push(location);
            }

            auto location = this->stack.top();
            auto candidate = this->next_candidate.operator()(location);
            if (candidate)
            {
                // If there is a new candidate for the current location, 
                // set it and find next location to check in the next call 
                // of this function      
                location.setValue(candidate);
                auto next = this->next_location.operator()();
                if (next.notFound()) // No more locations, a solution is found
                    return true;
                this->stack.push(next);
            }
            else
            {
                // If a new candidate not found, backtrack by cleaning 
                // location and popping it from the stack    
                location.clearValue();
                this->stack.pop();
            }
            // Solution not found yet, so return false to repeat the process
            return false;
        }
    };

} // end namespace

#endif