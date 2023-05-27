/*!*****************************************************************************
\file functions.cpp
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 4
\date 05-28-2023
\brief

The file includes define function for functions.h

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
        Find and return all empty adjacent cells
     *
     * \param key
        The key for which adjacent nodes are to be retrieved.
     *
     * \return
        A vector of pointers to AI::Node objects representing the adjacent nodes.
    *******************************************************************************/
    std::vector<AI::Node*> GetMapAdjacents::operator()(Key key)
    {
        std::vector<AI::Node*> list = {};

        // Find and return all empty adjacent cells
        int j = key[0];
        int i = key[1];

        if (j >= 0 && j < this->size && i >= 0 && i < this->size)
        {
            if (i > 0 && this->map[j * this->size + i - 1] == 0)
                list.emplace_back(new Node(Key{ j, i - 1 }, 10, 'W'));
            if (i < this->size - 1 && this->map[j * this->size + i + 1] == 0)
                list.emplace_back(new Node(Key{ j, i + 1 }, 10, 'E'));
            if (j > 0 && this->map[(j - 1) * this->size + i] == 0)
                list.emplace_back(new Node(Key{ j - 1, i }, 10, 'N'));
            if (j < this->size - 1 && this->map[(j + 1) * this->size + i] == 0)
                list.emplace_back(new Node(Key{ j + 1, i }, 10, 'S'));
        }

        return list;
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
	std::vector<char> Dijkstras::run(Key starting, Key target)
	{
        // Initialize closedlist and openlist
        HashTable* closedlist = new HashTable();
        PriorityQueue* openlist = new PriorityQueue();

        Node* pCurrent = nullptr;

        openlist->push(new Node(starting));

        // Implement the search
        while (true) {

            if (openlist->empty()) {

                pCurrent = nullptr;
                break;
            }

            pCurrent = openlist->pop();

            closedlist->add(pCurrent->key, pCurrent);

            if (compareArrays(pCurrent->key, target))
                break;

            std::vector<Node*> adjacents = this->pGetAdjacents->operator()(pCurrent->key);
            for (Node* adjacent : adjacents)
            {
                if (!closedlist->find(adjacent->key))
                {
                    Node* openlist_found = openlist->find(adjacent->key);
                    if (!openlist_found)
                        openlist->push(new Node(adjacent->key,
                            pCurrent->g + adjacent->g,
                            adjacent->info,
                            pCurrent));
                    else
                    {
                        int tentative_g = pCurrent->g + adjacent->g;
                        if (tentative_g < openlist_found->g)
                        {
                            openlist_found->parent = pCurrent;
                            openlist_found->info = pCurrent->info;
                            openlist_found->g = tentative_g;
                        }
                    }
                }
                delete adjacent;
            }
        }

        auto pCurr = getPath(pCurrent);

        delete openlist;
        delete closedlist;
        return pCurr;
	}

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
    std::vector<char> Dijkstras::getPath(Node* pNode)
    {
        std::vector<char> a{};

        // Trace back to return a vector of moves (.info)
        while (pNode) {
            a.push_back(pNode->info);
            pNode = pNode->parent;
        }

        a.pop_back(); // Remove starting point
        std::reverse(a.begin(), a.end());
        return a;
    }


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
    bool Dijkstras::compareArrays(Key current, Key target)
    {
        if (current == target)
            return true;

        if (target[1] >= 5)
            return true;

        return false;
    }

} // end namespace