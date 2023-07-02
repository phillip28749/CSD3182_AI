/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 9
\date 07-02-2023
\brief

	The file includes necessary function declaration and definition for Adversarial Search


Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <cstring> // memcpy
#include <limits.h>

#include "data.h"

#define UNUSED(x) (void)x;

// Class that defines the game specific data and code
class Grid
{
	static const int width = 3;
	static const int height = 3;

	char squares[width * height];

public:
	// The game marks/pieces
	static const char x = 'x';
	static const char o = 'o';
	static const char _ = ' ';

	/*!******************************************************************************/
	/*
		@brief
			Parameter Constructor for class Grid

		@param squares
			character pointer
	*/
	/********************************************************************************/
	Grid(char* squares = nullptr)
		: squares{ _, _, _, _, _, _, _, _, _ }
	{
		if (squares)
			for (int i = 0; i < height * width; ++i)
				this->squares[i] = squares[i];
	}

	/*!******************************************************************************/
	/*
		@brief
			Constructor for class Grid

		@param rhs
			copied grid from the rhs
	*/
	/********************************************************************************/
	Grid(const Grid& rhs)
	{
		this->operator=(rhs);
	}

	/*!******************************************************************************/
	/*
		@brief
			Copy Constructor for class Grid

		@param rhs
			copied grid from the rhs
	*/
	/********************************************************************************/
	void operator=(const Grid& rhs)
	{
		std::memcpy(squares, rhs.squares, height * width * sizeof(char));
	}

	/*!******************************************************************************/
	/*
		@brief
			set value into the square

		@param i
			square index value

		@param c
			square char value
	*/
	/********************************************************************************/
	void set(int i, char c)
	{
		squares[i] = c;
	}

	/*!******************************************************************************/
	/*
		@brief
			clear grid

		@param i
			square index value
	*/
	/********************************************************************************/
	void clear(int i)
	{
		squares[i] = _;
	}

	// Returns a list of indices of all empty squares in the grid.
	// For example for grid [' ',' ','o',' ','x',' ',' ',' ',' ']
	// the function returns [0,1,3,5,6,7,8]
	/*!******************************************************************************/
	/*
		@brief
			return indices of all empty squares in the grid

		@return
			return std::vector<int> of all the indices
	*/
	/********************************************************************************/
	std::vector<int> emptyIndices() const
	{
		std::vector<int> array = {};

		for (int i = 0; i < width * height; ++i) {

			if (squares[i] == _) {
				array.emplace_back(i);
			}
		}

		return array;
	}

	// Returns true if the grid has a winning configuration for the player
	/*!******************************************************************************/
	/*
		@brief
			winning bool condition

		@param player
			player that placed the squares in the grid

		@return
			bool of winning condition
	*/
	/********************************************************************************/
	bool winning(char player)
	{

		return (squares[0] == player && squares[1] == player && squares[2] == player) ||
			(squares[3] == player && squares[4] == player && squares[5] == player) ||
			(squares[6] == player && squares[7] == player && squares[8] == player) ||
			(squares[0] == player && squares[3] == player && squares[6] == player) ||
			(squares[1] == player && squares[4] == player && squares[7] == player) ||
			(squares[2] == player && squares[5] == player && squares[8] == player) ||
			(squares[0] == player && squares[4] == player && squares[8] == player) ||
			(squares[2] == player && squares[4] == player && squares[6] == player);
	}

	/*!******************************************************************************/
	/*
		@brief
			output stream

		@param os
			output os stream

		@param rhs
			value read from grid

		@return 
			output stream
	*/
	/********************************************************************************/
	friend std::ostream& operator<<(std::ostream& os, const Grid& rhs)
	{
		//UNUSED(rhs);

		// Your code ...
		for (int i = 0; i < rhs.height; ++i) {

			if (i == 0) {
				os << "[";
			}
			else {
				os << " ";
			}

			for (int j = 0; j < rhs.width; ++j) {

				os << rhs.squares[i * rhs.width + j];

				if (i == 2 && j == 2) {
					os << "]";
				}
				else
				{
					os << ",";
				}
			}

			if (i != rhs.height - 1) {
				os << "\n";
			}
		}

		return os;
	}
};

namespace AI
{
	// A node of the game tree
	template<typename T>
	class Move 
	{
		T grid;			// Result of a move: new state of the game grid			
		int score;		// Score of the move			
		std::vector<Move*>* next;  // All possible next moves
		int bestMove;   // Index of the first move in member next that has the best score 
		int spotIndex;  // Index of the move's spot (used for a visualization)

	public:

		/*!******************************************************************************/
		/*
			@brief
				Parameter Constructor for class Grid

			@param grid
				game grid

			@param score
				score value

			@param next
				next possible move

			@param bestMove
				index of the first move
		*/
		/********************************************************************************/
		Move(T grid = {}, int score = 0, std::vector<Move*>* next = new std::vector<Move*>{}, int bestMove = -1)
			: grid{ grid }, score{ score }, next{ next }, bestMove{ bestMove }, spotIndex{ -1 }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				Destructure for Move class
		*/
		/********************************************************************************/
		~Move() 
		{
			for (auto x : *next) {
				delete x;
			}

			delete next;
		}

		/*!******************************************************************************/
		/*
			@brief
				return the address of the vector next

			@param i
				next vector index

			@return
				the address
		*/
		/********************************************************************************/
		Move& at(int i)
		{
			return *(*next)[i];
		}

		/*!******************************************************************************/
		/*
			@brief
				return the score for the game

			@return
				the score int
		*/
		/********************************************************************************/
		int getScore() const
		{
			return score;
		}

		/*!******************************************************************************/
		/*
			@brief
				set spot index

			@param i
				index of vector
		*/
		/********************************************************************************/
		void setSpotIndex(int i)
		{
			spotIndex = i;
		}

		/*!******************************************************************************/
		/*
			@brief
				write output

			@param os
				output stream

			@param rhs
				read the value

			@return
				output stream
		*/
		/********************************************************************************/
		friend std::ostream& operator<<(std::ostream& os, const Move& rhs)
		{
			os << rhs.grid << std::endl;
			os << rhs.score << std::endl;
			os << rhs.next->size() << std::endl;
			os << rhs.bestMove << std::endl;
			return os;
		}
	};


	// Find the best next move for maximizer.
	// For the initial call, set the player parameter as maximizer. 
	// Warning! Found solution (sequence of moves) is not necessary shortest.
	/*!******************************************************************************/
	/*
		@brief
			template function to find the next move

		@param grid
			grid

		@param player
			output stream

		@param maximizer
			max move

		@param minimizer
			min move

		@return
			the best move
	*/
	/********************************************************************************/
	template<typename T>
	Move<T>* minimax(T grid, char player, char maximizer, char minimizer)
	{
		// Checks for the terminal states such as win and lose 
		// and returning a value accordingly
		if (grid.winning(minimizer)) {
			return new Move<T>(grid, -10);
		}
		else if (grid.winning(maximizer)) {
			return new Move<T>(grid, 10);
		}

		std::vector<int> availSpots = grid.emptyIndices();

		// Checks for the terminal states tie 
		if (availSpots.size() == 0) {
			return new Move<T>(grid, 0);
		}

		// An array of all next possible moves
		auto next = new std::vector<Move<T>*>;

		// Loop through available spots
		for (size_t i = 0; i < availSpots.size(); ++i)
		{
			// Set the empty spot as next move of the current player
			grid.set(availSpots[i], player);

			// Collect the score resulted from calling minimax 
			// on the opponent of the current player
			Move<T>* move = minimax(grid,
				(player == maximizer) ? minimizer : maximizer,
				maximizer,
				minimizer);

			// Store the index of that spot 
			move->setSpotIndex(availSpots[i]);

			// Push the move object to the array
			next->emplace_back(move);

			// Reset the spot to empty
			grid.clear(availSpots[i]);
		}

		int bestScore = 0;
		int bestMove = -1;

		// If it is the maximizer turn, loop over the moves and 
		// choose the move with the highest score
		if (player == maximizer)
		{
			bestScore = INT_MIN;
			for (unsigned int i = 0; i < next->size(); ++i)
			{
				if ((*next)[i]->getScore() > bestScore)
				{
					bestScore = (*next)[i]->getScore();
					bestMove = i;
				}
			}
		}
		else // Else, choose the move with the lowest score
		{
			bestScore = INT_MAX;
			for (unsigned int i = 0; i < next->size(); ++i)
			{
				if ((*next)[i]->getScore() < bestScore)
				{
					bestScore = (*next)[i]->getScore();
					bestMove = i;
				}
			}
		}

		// Return the move
		return new Move<T>(grid, bestScore, next, bestMove);
	}

} // end namespace

#endif