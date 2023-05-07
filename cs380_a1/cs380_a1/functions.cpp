/************************************************************************************//*!
\file          functions.cpp
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
#include "functions.h"

namespace AI 
{
    template<typename T>
    bool Node<T>::isnumber(const std::string& str) {
        for (auto x : str) {
            if (!std::isdigit(x))
                return false;
        }
        return true;
    }

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

