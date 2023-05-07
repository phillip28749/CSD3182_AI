/*!*****************************************************************************
\file functions.cpp
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 1
\date 05-7-2023
\brief
The file implements various functions and templates within the "AI" namespace. 
It includes functions for searching and traversing nodes, 
such as BFS (Breadth-First Search) and DFS (Depth-First Search). 
Additionally, it provides template functions for checking whether a 
given string represents a number and converting a node's contents 
into a string representation.

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

    /*!*****************************************************************************
    \brief
    Performs Breadth-First Search (BFS) on a node to find a specific value.

    \param node
    The starting node for the search.

    \param lookingfor
    The value to search for.

    \return
    Returns a pointer to the node containing the searched value if found;
    otherwise, returns nullptr.
    *******************************************************************************/
    template<typename T>
    Node<T>* BFS(Node<T>& node, const T& lookingfor)
    {
        std::queue<Node<T>*> openlist;
        openlist.push(&node);
        while (!openlist.empty())
        {
            Node<T>* current = openlist.front();
            openlist.pop();
            if (current->value == lookingfor)
                return current;
            for (Node<T>* child : current->children)
                openlist.push(child);
        }
        return nullptr;
    }

    /*!*****************************************************************************
    \brief
    Performs Depth-First Search (DFS) on a node to find a specific value.

    \param node
    The starting node for the search.

    \param lookingfor
    The value to search for.

    \return
    Returns a pointer to the node containing the searched value if found;
    otherwise, returns nullptr.
    *******************************************************************************/
    template<typename T>
    Node<T>* DFS(Node<T>& node, const T& lookingfor)
    {
        std::stack<Node<T>*> openlist;
        openlist.push(&node);
        while (!openlist.empty())
        {
            Node<T>* current = openlist.top();
            openlist.pop();
            if (current->value == lookingfor)
                return current;
            for (Node<T>* child : current->children)
                openlist.push(child);
        }
        return nullptr;
    }

    // Explicit instantiation of the required types
    template bool Node<int>::isnumber(const std::string&);
    template bool Node<std::string>::isnumber(const std::string&);
    template std::string AI::Node<int>::getAsString();
    template std::string AI::Node<std::string>::getAsString();
    template Node<std::string>* BFS(Node<std::string>& node, const std::string& lookingfor);
    template Node<std::string>* DFS(Node<std::string>& node, const std::string& lookingfor);
} 

