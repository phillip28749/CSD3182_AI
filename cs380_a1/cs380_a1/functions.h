/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 1
\date 05-7-2023
\brief
The file includes necessary headers and defines a namespace "AI" 
which contains various functions and templates. It also includes a 
struct template "Node" representing a simple graph node, along with 
serialization and deserialization functions for the node. The file 
provides implementations of the Breadth-First Search (BFS) and 
Depth-First Search (DFS) algorithms.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#include "data.h"

namespace AI 
{
    #define UNUSED(expr) (void)expr;


    /*!*****************************************************************************
    \brief
    A simple graph node definition with serialization functions
    *******************************************************************************/
    template<typename T>
    struct Node
    {
        // Member data

        T value;
        Node* parent;
        std::list<Node*> children;

        /*!*****************************************************************************
        \brief
        Parameter constructor
        *******************************************************************************/
        Node(T value = {}, Node* parent = nullptr, 
                    const std::list<Node*>& children = {})
            : value{ value }, parent{ parent }, children{ children }
        {

        }

        /*!*****************************************************************************
        \brief
        Destructor
        *******************************************************************************/
        ~Node()
        {
            for (auto child : children)
                delete child;
        }

        /*!*****************************************************************************
        \brief
        Serialization operator for the `Node` struct template.

        \param os
        The output stream to write the serialized node to.

        \param rhs
        The node to be serialized.

        \return
        The reference to the output stream after serialization.
        *******************************************************************************/
        friend std::ostream& operator<<(std::ostream& os, Node const& rhs)
        {
            os << rhs.value << " {";
            os << rhs.children.size() << ' ';
            for (auto const& child : rhs.children) {
                os << child->getAsString();
            }
            os << "} ";

            return os;
        }

        /*!*****************************************************************************
        \brief
        Deserialization operator for the `Node` struct template.

        \param is
        The input stream to read the serialized node from.

        \param rhs
        The node to be deserialized.

        \return
        The reference to the input stream after deserialization.
        *******************************************************************************/
        friend std::istream& operator>>(std::istream& is, Node& rhs)
        {
            std::string input;
            std::getline(is, input);
            rhs.setFromString(input);
            return is;
        }

        /*!*****************************************************************************
        \brief
        Returns values from root to this node as an array

        \return
        Returns a vector<T>
        *******************************************************************************/
        std::vector<T> getPath() const
        {
            std::vector<T> r;
            r.push_back(this->value);
            Node* node = this->parent;
            while (node)
            {
                r.insert(r.begin(), node->value);
                node = node->parent;
            }
            return r;
        }

        /*!*****************************************************************************
        \brief
        Deserialization function that turns a stream of bytes
        into an object in memory.

        \param str
        The string

        \return
        Returns a string
        *******************************************************************************/
        std::string setFromString(std::string str)
        {

            size_t n = str.find(' ');

            if (n == std::string::npos)
                return str;

            this->value = str.substr(0, n);
            str = str.substr(n + 1);

            if (str[0] != '{')
                return str;

            str = str.substr(1);

            n = str.find(' ');
            if (n == std::string::npos)
                return str;

            int length;
            bool check = isnumber(str.substr(0, n));

            if (!check) 
                return str;
            else 
                length = std::stoi(str.substr(0, n));
           
            str = str.substr(n + 1);

            for (int i = 0; i < length; ++i)
            {
                Node* child = new Node("", this);
                str = child->setFromString(str);
                this->children.push_back(child);
            }

            n = str.find('}');
            if (n == std::string::npos)
                return str;

            str = str.substr(n + 2);

            return str;
        }


        /*!*****************************************************************************
        \brief
        Serialization function that turn a tree in memory into a stream
        of bytes so it can be stored on disk or sent over the network.

        \return
        Returns a string representation of the node and its children.
        *******************************************************************************/
        std::string getAsString();

        //Helper function

        /*!*****************************************************************************
        \brief
        Checks whether a given string represents a number.

        \param str
        The string to be checked.

        \return
        Returns true if the string represents a number; otherwise, returns false.
        *******************************************************************************/
        static bool isnumber(const std::string& str);
    };

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
    Node<T>* BFS(Node<T>& node, const T& lookingfor);

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
    Node<T>* DFS(Node<T>& node, const T& lookingfor);


} // end namespace

#endif