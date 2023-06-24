/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 8
\date 06-24-2023
\brief

    The file includes necessary function declaration and definition for Backtracking 

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/

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

    /*!******************************************************************************/
    /*
        @brief
            Parameter Constructor for class NextLocation_Sudoku1D

        @param map
            void pointer
    */
     /********************************************************************************/
    NextLocation_Sudoku1D(void* map)
        : map{ static_cast<AI::MapInt1D*>(map) }
    {
    }

    /*!******************************************************************************/
    /*
        @brief
            overload operator()

        @return 
            return AI::Location
    */
    /********************************************************************************/
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

    /*!******************************************************************************/
    /*
        @brief
            Parameter Constructor for class NextLocation_Sudoku2D

        @param map
            void pointer
    */
    /********************************************************************************/
    NextLocation_Sudoku2D(void* map)
        : map{ static_cast<AI::MapInt2D*>(map) }
    {
    }

    /*!******************************************************************************/
    /*
        @brief
            overload operator()

        @return
            return AI::Location
    */
    /********************************************************************************/
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

    /*!******************************************************************************/
    /*
        @brief
            Parameter Constructor for class NextCandidate_Sudoku1D

        @param map
            void pointer
    */
    /********************************************************************************/
    NextCandidate_Sudoku1D(void* map)
        : map{ static_cast<AI::MapInt1D*>(map) }
    {
    }

    /*!******************************************************************************/
    /*
        @brief
            Function that check for index value within the given map location

        @param value
            int value to check against
    */
    /********************************************************************************/
    int indexOf(int value) {

        for (int i = 0; i < map->size; ++i) {
            if (map->base[i] == value) {
                return i;
            }
        }

        return -1;
    }

    /*!******************************************************************************/
    /*
        @brief
            overload operator()

        @return
            return AI::Location
    */
    /********************************************************************************/
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

    /*!******************************************************************************/
    /*
        @brief
            Parameter Constructor for class NextCandidate_Sudoku2D

        @param map
            void pointer
    */
    /********************************************************************************/
    NextCandidate_Sudoku2D(void* map)
        : map{ static_cast<AI::MapInt2D*>(map) }
    {
    }

    /*!******************************************************************************/
    /*
        @brief
            Function that check for index value within the given map location

        @param Map[]
            array of SudoKu map

        @param value
            int value to check against

        @param index
            index of the map
    */
    /********************************************************************************/
    int indexOf(int Map[], int value, int index) {

        int row = index % map->width;
        int col = index / map->height;
        int startrow = row - row % 3;
        int startcol = col - col % 3;

        for (int i = 0; i < map->width; ++i) {

            if (Map[map->width * i + row] == value)
                return value;
        }

        for (int i = 0; i < map->height; ++i) {

            if (Map[map->height * col + i] == value)
                return value;
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                if (Map[(i + startcol) * map->width + (j + startrow)] == value)
                    return value;
            }
        }

        return -1;
    }

    /*!******************************************************************************/
    /*
        @brief
            overload operator()

        @return
            return v or 0
    */
    /********************************************************************************/
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

        /*!******************************************************************************/
        /*
            @brief
                Parameter Constructor for template class Backtracking

            @param map
                void pointer
        */
        /********************************************************************************/
        Backtracking(void* map = nullptr)
            : next_location{ map }, next_candidate{ map }
        {
        }

        /*!******************************************************************************/
        /*
            @brief
                Find solution in a blocking mode
        */
        /********************************************************************************/
        void run()
        {
            while (solve() == false) {
            }
        }


        /*!******************************************************************************/
        /*
            @brief
                // One iteration of the search. Used by run() in a blocking running mode
                // or can be called by timer in an non-blocking run

            @return
                bool
        */
        /********************************************************************************/
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