/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 3
\date 05-21-2023
\brief

Conbinaion of assignment 1 and 2, declard Node, GetAdjacents, 
GetTreeAdjacents, Flood_Fill_Recursive class and template class
Flood_Fill_Iterative template class functions and struct interface.
Function definition for stack and queue

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <algorithm>

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI 
{

    // A simple graph node definition with serialization functions
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
            //os << rhs.value; // just for test purpose
            //// ...

            //return os;
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
            //is >> rhs.value; // just for test purpose
            //// ...

            //return is;
            std::string input;
            std::getline(is, input);
            rhs.setFromString(input);
            return is;
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
        Checks whether a given string represents a number.

        \param str
        The string to be checked.

        \return
        Returns true if the string represents a number; otherwise, returns false.
        *******************************************************************************/
        static bool isnumber(const std::string& str);

        // Returns values from root to this node as an array
        std::vector<T> getPath() const
        {
            //std::vector<T> r;
            //// ...

            //return r;
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
        Serialization function that turn a tree in memory into a stream
        of bytes so it can be stored on disk or sent over the network.

        \return
        Returns a string representation of the node and its children.
        *******************************************************************************/
        std::string getAsString();
    };


    // The actual node type for this assignment
    using TreeNode = Node<std::string>;


    // Abstract base class for domain specific functors that return adjacent nodes
    class GetAdjacents
    {

    public:

        virtual ~GetAdjacents()
        {
        }

        virtual std::vector<TreeNode*> operator()(TreeNode* pNode) = 0;
    };


    // Domain specific functor that returns adjacent nodes
    class GetTreeAdjacents : public GetAdjacents
    {
 
    public:
        /*!*****************************************************************************
        \brief
        Default Constructor
        *******************************************************************************/
        GetTreeAdjacents()
            : GetAdjacents()
        {
        }

        /*!*****************************************************************************
         * \brief
            Functor operator that returns the adjacent nodes of a given key.
         *
         * \param pNode
            The TreeNode for which adjacent nodes are to be retrieved.
         *
         * \return
            A vector of pointers to AI::Node objects representing the adjacent nodes.
        *******************************************************************************/
        std::vector<TreeNode*> operator()(TreeNode* pNode);
    };

    // Domain specific functor that returns shuffled adjacent nodes
    class GetTreeStochasticAdjacents : public GetTreeAdjacents
    {
    public:
        /*!*****************************************************************************
        \brief
        Default Constructor
        *******************************************************************************/
        GetTreeStochasticAdjacents()
            : GetTreeAdjacents{}
        {
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
        std::vector<TreeNode*> operator()(TreeNode* pNode);
    };

    /*!*****************************************************************************
    // Wrappers that provide same interface for queue and stack
    *******************************************************************************/
    struct Interface
    {
        std::list<TreeNode*> p_Node;

        virtual void clear() = 0;

        virtual void push(TreeNode* pNode) = 0;

        virtual TreeNode* pop() = 0;

        bool isEmpty() { return p_Node.size() == 0; }
    };

    /*!*****************************************************************************
    // Declare and define function for for struct Queue
    *******************************************************************************/
    struct Queue : Interface //...
    {
        void clear()
        {
            p_Node.clear();
        }

        void push(TreeNode* pNode)
        {
            p_Node.emplace_back(pNode);
        }

        TreeNode* pop()
        {
            TreeNode* pNode = nullptr;
            pNode = p_Node.front();
            p_Node.pop_front();

            return pNode;
        }
    };

    /*!*****************************************************************************
    // Declare and define function for for struct Stack
    *******************************************************************************/
    struct Stack : Interface //...
    {
        void clear()
        {
            p_Node.clear();
        }

        void push(TreeNode* pNode)
        {
            p_Node.emplace_back(pNode);
        }

        TreeNode* pop()
        {
            TreeNode* pNode = nullptr;
            pNode = p_Node.back();
            p_Node.pop_back();
           
            return pNode;
        }
    };

    /*!*****************************************************************************
     * \brief Class for performing flood fill using a recursive algorithm.
    *******************************************************************************/
    class Flood_Fill_Recursive
    {
        GetTreeAdjacents* pGetAdjacents;

    public:
        /*!*****************************************************************************
         * \brief
            Constructs a `Flood_Fill_Recursive` object.
         *
         * \param pGetAdjacents
            Pointer to a GetAdjacents object used for retrieving adjacent nodes.
        *******************************************************************************/
        Flood_Fill_Recursive(GetTreeAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }
        {
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
        void run(TreeNode* pNode, std::string value);
    };

    // Iterative Flood Fill
    // Type T defines is it depth- or breadth-first
    template<typename T>
    class Flood_Fill_Iterative
    {
        GetTreeAdjacents* pGetAdjacents;
        T openlist;

    public:
        /*!*****************************************************************************
         * \brief
            Constructs a `Flood_Fill_Recursive` object.
         *
         * \param pGetAdjacents
            Pointer to a GetAdjacents object used for retrieving adjacent nodes.
        *******************************************************************************/
        Flood_Fill_Iterative(GetTreeAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }, openlist{}
        {
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
        void run(TreeNode* pNode, std::string value);
    };

} // end namespace

#endif