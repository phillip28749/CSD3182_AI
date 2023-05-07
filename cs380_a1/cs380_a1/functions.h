/************************************************************************************//*!
\file          functions.h
\project       A1
\author        Chen Yen Hsun, 20027671
\par           email: c.yenhsun\@digipen.edu
\date          May 7, 2023
\brief
               



Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
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

    // A simple graph node definition with serialization functions
    template<typename T>
    struct Node
    {
        // Member data

        T value;
        Node* parent;
        std::list<Node*> children;

        Node(T value = {}, Node* parent = nullptr, 
                    const std::list<Node*>& children = {})
            : value{ value }, parent{ parent }, children{ children }
        {

        }

        ~Node()
        {
            for (auto child : children)
                delete child;
        }

        // Serialization
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

        // Deserialization
        friend std::istream& operator>>(std::istream& is, Node& rhs)
        {
            std::string input;
            std::getline(is, input);
            rhs.setFromString(input);
            return is;
        }

        // Returns values from root to this node as an array
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

        // Deserialization function that turns a stream of bytes 
        // into an object in memory.
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

        // Serialization function that turn a tree in memory into a stream  
        // of bytes so it can be stored on disk or sent over the network.
        std::string getAsString();

        //Helper function
        static bool isnumber(const std::string& str);
    };

    // Implementation of the Breadth-First Search algorithm
    template<typename T>
    Node<T>* BFS(Node<T>& node, const T& lookingfor);


    // Implementation of the Depth-First Search algorithm
    template<typename T>
    Node<T>* DFS(Node<T>& node, const T& lookingfor);


} // end namespace

#endif