/*!*****************************************************************************
\file functions.cpp
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 3
\date 05-21-2023
\brie

Conbinaion of assignment 1 and 2, define function declared in functions.h

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#include "functions.h"

namespace AI 
{

    /*!*****************************************************************************
    \brief
    Converts the node and its children into a string representation.

    \return
    Returns a string representation of the node and its children.
    *******************************************************************************/
    template<typename T>
    std::string Node<T>::getAsString() {

        std::string str;
        str += value + " {";
        str += std::to_string(children.size()) + " ";
        for (const auto& child : children)
            str += child->getAsString();
        str += "} ";
        return str;
    }
    template std::string AI::Node<int>::getAsString();
    template std::string AI::Node<std::string>::getAsString();

    /*!*****************************************************************************
    \brief
    Checks whether a given string represents a number.

    \param str
    The string to be checked.

    \return
    Returns true if the string represents a number; otherwise, returns false.
    *******************************************************************************/
    template<typename T>
    bool Node<T>::isnumber(const std::string& str) {
        for (auto x : str) {
            if (!std::isdigit(x))
                return false;
        }
        return true;
    }
    template bool Node<int>::isnumber(const std::string&);
    template bool Node<std::string>::isnumber(const std::string&);

    /*!*****************************************************************************
    \brief
        // Push to the list all children of pNode excluding
        // those with value not equal "x"

    \param TreeNode* pNode
        node to be pushed
    \return
        A vector of TreeNode that contain the pushed node
    *******************************************************************************/
    std::vector<TreeNode*> GetTreeAdjacents::operator()(TreeNode* pNode)
    {
        std::vector<AI::TreeNode*> list = {};

        for (auto node : pNode->children) {

            if (node->value == "x")
                list.emplace_back(node);
        }

        return list;
    }

    /*!*****************************************************************************
     * \brief
        Returns a vector of shuffled adjacent nodes in a map.
     *
     * \param pNode
        The TreeNode of the node whose adjacent nodes are to be retrieved.
     *
     * \return
        A vector of pointers to shuffled adjacent nodes in the map.
    *******************************************************************************/
    std::vector<TreeNode*> GetTreeStochasticAdjacents::operator()(TreeNode* pNode)
    {
            std::vector<TreeNode*> adjacents;
            adjacents = GetTreeAdjacents::operator()(pNode);

            // Use the base class operator() and then shuffle the result
            std::random_shuffle(adjacents.begin(), adjacents.end());

            return adjacents;
    }

    /*!*****************************************************************************
     * \brief
        Runs the flood fill algorithm recursively.
     *
     * \param pNode
        The pNode of the starting node for flood fill.

     * \param value
        The value to fill the connected nodes with.
    *******************************************************************************/
    void Flood_Fill_Recursive::run(TreeNode* pNode, std::string value)
    {
        // Implement the flood fill
        std::vector<TreeNode*> adjacents = pGetAdjacents->operator()(pNode);

        for (TreeNode* adj : adjacents)
        {
            adj->value = value;
            run(adj, value);
            //delete adj;
        }
    }

    /*!*****************************************************************************
     * \brief
        Runs the iterative flood fill algorithm.
     *
     * \param pNode
        The TreeNode of the starting node for flood fill.

     * \param value
        The string to fill the connected nodes with.
    *******************************************************************************/
    template<typename T>
    inline void Flood_Fill_Iterative<T>::run(TreeNode* pNode, std::string value)
    {
        // Implement the flood fill
        openlist.clear();
        openlist.push(pNode);

        while (!openlist.isEmpty())
        {
            TreeNode* current = openlist.pop();
            std::vector<TreeNode*> adjacents = pGetAdjacents->operator()(current);

            for (TreeNode* adj : adjacents)
            {
                adj->value = value;
                openlist.push(adj);
            }
            //delete current;
        }
    }
    template void Flood_Fill_Iterative<Queue>::run(TreeNode* pNode, std::string value);
    template void Flood_Fill_Iterative<Stack>::run(TreeNode* pNode, std::string value);
}