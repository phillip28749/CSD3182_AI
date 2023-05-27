/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 4
\date 05-28-2023
\brief

The file includes necessary headers and defines a namespace "AI"
which contains class GetMapAdjacents and class Dijkstras declaration

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data.h"
#include <algorithm>

#define UNUSED(x) (void)x;

namespace AI 
{

    // Domain specific functor that returns adjacent nodes
    class GetMapAdjacents : public GetAdjacents
    {
        int* map; // the map with integers where 0 means an empty cell
        int size; // width and hight of the map in elements

    public:

        /*!*****************************************************************************
         * \brief
            Constructs a `GetMapAdjacents` object.
         *
         * \param map
            A pointer to an array of integers representing the map.

         * \param size
            The width and height of the map in terms of the number of elements.
        *******************************************************************************/
        GetMapAdjacents(int* map = nullptr, int size = 0)
            : GetAdjacents(), map{ map }, size{ size }
        {
        }

        /*!*****************************************************************************
         * \brief
            Find and return all empty adjacent cells
         *
         * \param key
            The key for which adjacent nodes are to be retrieved.
         *
         * \return
            A vector of pointers to AI::Node objects representing the adjacent nodes.
        *******************************************************************************/
        std::vector<AI::Node*> operator()(Key key);
    };

    class Dijkstras
    {
        GetAdjacents* pGetAdjacents;

    public:

        /*!*****************************************************************************
         * @brief Constructs a Dijkstras object.
         *
         * @param pGetAdjacents
         *     Pointer to a GetAdjacents object that provides the function to retrieve adjacent nodes.
         *     The Dijkstras object takes ownership of the provided GetAdjacents object.
        *******************************************************************************/
        Dijkstras(GetAdjacents* pGetAdjacents)
            : pGetAdjacents(pGetAdjacents)
        {
        }

        /*!*****************************************************************************
         * @brief Runs Dijkstra's algorithm to find the path from the starting position to the target position.
         *
         * @param starting
         *     An array of 2 elements [j, i] representing the starting position on the map.
         *
         * @param target
         *     An array of 2 elements [j, i] representing the target position on the map.
         *
         * @return
         *     A vector of characters representing the path from the starting position to the target position.
         * 
        *******************************************************************************/
        std::vector<char> run(Key starting, Key target);

    private:
        /*!*****************************************************************************
         * @brief Retrieves the path from a given node to the starting point.
         *
         * @param pNode
         *     Pointer to the current node.
         *
         * @return
         *     A vector of characters representing the path from the current node to the starting point.
         *     Each character represents a movement direction
        *******************************************************************************/
        std::vector<char> getPath(Node* pNode);

        /*!*****************************************************************************
         * @brief Compares two arrays for equality.
         *
         * @param current
         *     The current array of 2 elements [j, i].
         *
         * @param target
         *     The target array of 2 elements [j, i].
         *
         * @return
         *     `true` if the arrays are equal, `false` otherwise.
        *******************************************************************************/
        bool compareArrays(Key current, Key target);
    };
} // end namespace

#endif