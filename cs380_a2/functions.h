/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 2
\date 05-14-2023
\brief

The file includes necessary headers and defines a namespace "AI"
which contains various functions and templates.It provides functionalities 
for flood fill algorithms and includes domain-specific functors and wrappers.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stack>
#include <algorithm>
#include <list>

#include "data.h"

#define UNUSED(expr) (void)expr;

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
         * \brief Destroys the `GetMapAdjacents` object.
        *******************************************************************************/
        virtual ~GetMapAdjacents()
        {
        }

        /*!*****************************************************************************
         * \brief 
            Functor operator that returns the adjacent nodes of a given key.
         *
         * \param key 
            The key for which adjacent nodes are to be retrieved.
         *
         * \return 
            A vector of pointers to AI::Node objects representing the adjacent nodes.
        *******************************************************************************/
        std::vector<AI::Node*> operator()(Key key);
    };

    /*!*****************************************************************************
     * \brief Domain-specific functor that returns shuffled adjacent nodes in a map.
    *******************************************************************************/
    class GetMapStochasticAdjacents : public GetMapAdjacents
    {
    public:
        /*!*****************************************************************************
         * \brief 
            Constructs a `GetMapStochasticAdjacents` object.
         *
         * \param map 
            A pointer to an array of integers representing the map.

         * \param size 
            The width and height of the map in terms of the number of elements.
        *******************************************************************************/
        GetMapStochasticAdjacents(int* map, int size)
            : GetMapAdjacents{ map, size }
        {
        }

        /*!*****************************************************************************
         * \brief 
            Returns a vector of shuffled adjacent nodes in a map.
         *
         * \param key 
            The key of the node whose adjacent nodes are to be retrieved.
         *
         * \return 
            A vector of pointers to shuffled adjacent nodes in the map.
        *******************************************************************************/
        std::vector<AI::Node*> operator()(Key key);
    };

    /*!*****************************************************************************
    // Wrappers that provide same interface for queue and stack
    *******************************************************************************/
    struct Interface
    {
        virtual void clear() = 0;

        virtual void push(Node* pNode) = 0;

        virtual Node* pop() = 0;
    };

    /*!*****************************************************************************
    // Declare function for for struct Queue
    *******************************************************************************/
    struct Queue : Interface
    {
        std::list<Node*> p_Node;

        void clear();

        void push(Node* pNode);

        Node* pop();

        bool isEmpty();
    };

    /*!*****************************************************************************
    // Declare function for for struct Stack
    *******************************************************************************/
    struct Stack : Interface //...
    {

        std::list<Node*> p_Node;

        void clear();

        void push(Node* pNode);

        Node* pop();

        bool isEmpty();
    };

    /*!*****************************************************************************
     * \brief Class for performing flood fill using a recursive algorithm.
    *******************************************************************************/
    class Flood_Fill_Recursive
    {
        GetAdjacents* pGetAdjacents;

    public:
        /*!*****************************************************************************
         * \brief 
            Constructs a `Flood_Fill_Recursive` object.
         *
         * \param pGetAdjacents 
            Pointer to a GetAdjacents object used for retrieving adjacent nodes.
        *******************************************************************************/
        Flood_Fill_Recursive(GetAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }
        {
        }
        /*!*****************************************************************************
         * \brief 
            Runs the flood fill algorithm recursively.
         *
         * \param key 
            The key of the starting node for flood fill.

         * \param color 
            The color to fill the connected nodes with.
        *******************************************************************************/
        void run(Key key, int color);
    };

    /*!*****************************************************************************
     * \brief 
        Class for performing iterative flood fill.
     *
     * \tparam T 
        Type specifying whether it is depth-first or breadth-first.
    *******************************************************************************/
    template<typename T>
    class Flood_Fill_Iterative
    {
        GetAdjacents* pGetAdjacents;
        T openlist;

    public:
        /*!*****************************************************************************
         * \brief 
            Constructs a `Flood_Fill_Iterative` object.
         *
         * \param pGetAdjacents 
            Pointer to a GetAdjacents object used for retrieving adjacent nodes.
        *******************************************************************************/
        Flood_Fill_Iterative(GetAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }, openlist{}
        {
        }
        /*!*****************************************************************************
         * \brief 
            Runs the iterative flood fill algorithm.
         *
         * \param key 
            The key of the starting node for flood fill.

         * \param color 
            The color to fill the connected nodes with.
        *******************************************************************************/
        void run(Key key, int color);
    };

} // end namespace

#endif