/*!*****************************************************************************
\file functions.cpp
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 2
\date 05-14-2023
\brief
The file includes necessary function definition and defines a namespace "AI"
which contains various functions and templates.It provides functionalities
for flood fill algorithms and includes domain-specific functors and wrappers.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#include "functions.h"

namespace AI 
{

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
    std::vector<AI::Node*> GetMapAdjacents::operator()(Key key)
    {
        std::vector<Node*> list = {};

        int j = key.j;
        int i = key.i;

        if (j >= 0 && j < this->size && i >= 0 && i < this->size)
        {
            if (i > 0 && this->map[j * this->size + i - 1] == 0)
                list.emplace_back(new Node(Key(j, i - 1), &this->map[j * this->size + i - 1]));
            if (i < this->size - 1 && this->map[j * this->size + i + 1] == 0)
                list.emplace_back(new Node(Key(j, i + 1), &this->map[j * this->size + i + 1]));
            if (j > 0 && this->map[(j - 1) * this->size + i] == 0)
                list.emplace_back(new Node(Key(j - 1, i), &this->map[(j - 1) * this->size + i]));
            if (j < this->size - 1 && this->map[(j + 1) * this->size + i] == 0)
                list.emplace_back(new Node(Key(j + 1, i), &this->map[(j + 1) * this->size + i]));
        }

        return list;
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
    std::vector<AI::Node*> GetMapStochasticAdjacents::operator()(Key key)
    {
        std::vector<AI::Node*> list = {};
        list = GetMapAdjacents::operator()(key);

        // Find and return all empty adjacent cells
        // Use the base class operator() and then shuffle the result
        std::random_shuffle(list.begin(), list.end());

        return list;
    }

    /*!*****************************************************************************
    // Define function for for struct Queue
    *******************************************************************************/
    void Queue::clear() { p_Node.clear(); }
    void Queue::push(Node* pNode){ p_Node.emplace_back(pNode); }
    Node* Queue::pop(){

        Node* pNode = p_Node.front();
        p_Node.pop_front();

        return pNode;
    }
    bool Queue::isEmpty() { return p_Node.size() == 0; }


    /*!*****************************************************************************
    // Define function for for struct Stack
    *******************************************************************************/
    void Stack::clear() { p_Node.clear(); }
    void Stack::push(Node* pNode) { p_Node.emplace_back(pNode); }
    Node* Stack::pop() {

        Node* pNode = p_Node.back();
        p_Node.pop_back();

        return pNode;
    }
    bool Stack::isEmpty() { return p_Node.size() == 0; }


    /*!*****************************************************************************
     * \brief
        Runs the flood fill algorithm recursively.
     *
     * \param key
        The key of the starting node for flood fill.

     * \param color
        The color to fill the connected nodes with.
    *******************************************************************************/
    void Flood_Fill_Recursive::run(Key key, int color)
    {
        // Implement the flood fill
        std::vector<Node*> adjacents = pGetAdjacents->operator()(key);

        for (Node* adj : adjacents)
        {
            *(adj->pValue) = color;
            run(adj->key, color);
            delete adj;
        }
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
    template<typename T>
    void Flood_Fill_Iterative<T>::run(Key key, int color)
    {
        // Implement the flood fill
        openlist.clear();
        openlist.push(new Node(key));

        while (!openlist.isEmpty())
        {
            Node* current = openlist.pop();
            std::vector<Node*> adjacents = pGetAdjacents->operator()(current->key);

            for (Node* adj : adjacents)
            {
                *(adj->pValue) = color;
                openlist.push(adj);
            }
            delete current;
        }
    }

    template void Flood_Fill_Iterative<Queue>::run(Key key, int color);
    template void Flood_Fill_Iterative<Stack>::run(Key key, int color);
} 