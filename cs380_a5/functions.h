/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 5
\date 06-04-2023
\brief

    The file includes necessary function for BellmanFord class

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <array>
#include <climits>
#include <algorithm>

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI 
{
    const int null = -1;
    const int inf = INT_MAX;

    // An implementation of the Bellman-Ford algorithm. 
    // The algorithm finds the shortest path between a
    // starting node and all other nodes in the graph. 
    // The algorithm also detects negative cycles.
    template<int SIZE = 0>
     class BellmanFord
    {
        int* matrix; // the cost adjacency matrix
        int* distance;
        int* predecessor;

    public:
        /*!*****************************************************************************
         * @brief Constructor for the BellmanFord class.
         *
         * @param matrix
         *     Pointer to the cost adjacency matrix.
         *******************************************************************************/
        BellmanFord(int* matrix = nullptr)
            : matrix{ matrix }, distance{ nullptr }, predecessor{ nullptr }
        {
            distance = new int[SIZE];
            predecessor = new int[SIZE];
         }

        /*!*****************************************************************************
         * @brief Destructor for the BellmanFord class.
         *******************************************************************************/
        ~BellmanFord()
        {
            delete[] distance;
            delete[] predecessor;
        }

        /*!*****************************************************************************
         * @brief Runs the Bellman-Ford algorithm to find the shortest path.
         *
         * @param starting
         *     Starting node index (default: 0).
         *
         * @return
         *     False if negative cycles are found, true otherwise.
        *******************************************************************************/
        bool run(int starting = 0)
        {
            if (!SIZE) return true;

            // Initialize the distance to all nodes to be infinity
            // except for the start node which is zero.
            // Initialize predecessor array which will be used in shortest path
            // reconstruction after the algorithm has terminated.
            for (int i = 0; i < SIZE; ++i) {
                distance[i] = inf;
                predecessor[i] = null;
            }

            distance[starting] = 0;

            // For each node, apply relaxation for all the edges
            for (int k = 0; k < SIZE; k++)
            {
                int counter = 0; // Number of relaxations
                for (int j = 0; j < SIZE; j++)
                    for (int i = 0; i < SIZE; i++)
                        if ((j != i) && (distance[j] != inf) &&
                            (matrix[j * SIZE + i] != inf))
                        {
                            int new_dist = distance[j] + matrix[j * SIZE + i];
                            if (new_dist < distance[i])
                            {
                                // Relaxation
                                distance[i] = new_dist;
                                predecessor[i] = j;
                                counter++;
                            }
                        }
                if (counter == 0) // Stop when no more relaxation
                    return true;  // There is no negative cycles 
            }

            return false; // There is a negative cycle
        }

        /*!*****************************************************************************
         * @brief Reconstructs the shortest path from the starting node to the target node.
         *
         * @param target
         *     Index of the target node.
         *
         * @return
         *     Vector of node indices representing the shortest path.
        *******************************************************************************/
        std::vector<int> getPath(int target)
        {
            std::vector<int> path{};
            for (int i = target; predecessor[i] != null; i = predecessor[i])
                path.push_back(i);

            reverse(std::begin(path),std::end(path));

            return path;
        }

        /*!*****************************************************************************
         * @brief Creates a step-by-step route description of the shortest path.
         *
         * @param target
         *     Index of the target node.
         *
         * @return
         *     Vector of arrays, each containing [source, destination, cost].
        *******************************************************************************/
        std::vector<std::array<int, 3>> getRoute(int target)
        {
            std::vector<std::array<int, 3>> route{};

            for (int i = target; predecessor[i] != null; i = predecessor[i])
                route.push_back(std::array<int, 3>{ predecessor[i], i, distance[i]});

            for (size_t i = 0; i < route.size() - 1; ++i)
                route[i][2] -= route[i + 1][2];

            reverse(std::begin(route), std::end(route));

            return route;
        }

        friend std::ostream& operator<<(std::ostream& os, const BellmanFord& rhs)
        {
            os << "[";

            for (int i = 0; i < SIZE; i++) {

                if (rhs.distance[i] == inf) {

                    os << "inf";
                }
                else {
                    
                    os << rhs.distance[i]; 
                }
                os << (i < SIZE - 1 ? "," : "");
            }

            os << "] [";

            for (int i = 0; i < SIZE; i++) {

                if (rhs.predecessor[i] == null){

                    os << "null";
                }
                else{

                    os << rhs.predecessor[i];
                }

                os << (i < SIZE - 1 ? "," : "");
            }

            os << ']';

            return os;
        }
    };

} // end namespace

#endif