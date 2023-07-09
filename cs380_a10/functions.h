/*!*****************************************************************************
\file functions.h
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 10
\date 07-09-2023
\brief

	The file includes necessary function declaration and definition for Behavior Trees


Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sstream>
#include <string>
#include <list>

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI
{

	// Check the state of a task comparing it with given by parameter 
	class CheckState : public Task
	{
		Task checktask;
		State checkstate;

	public:

		/*!******************************************************************************/
		/*
			@brief
				CheckState Constructor

			@param checktask
				Create action task

			@param checkstate
				Status of BT node
		*/
		/********************************************************************************/
		CheckState(Task checktask = {}, State checkstate = State::Success)
			: Task{ "CheckState" }, checktask{ checktask }, checkstate{ checkstate }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				override operator() for CheckState

			@param log
				log osstream

			@param level
				string level

			@return
				action task
		*/
		/********************************************************************************/
		Task& operator()(Log* log = nullptr, std::string level = "");

	};

	// Selector composite
	//     Returns immediately with a success status code 
	//     when one of its children runs successfully.
	class Selector : public Task
	{
		std::list<SMART> tasks;

	public:

		/*!******************************************************************************/
		/*
			@brief
				Selector Constructor

			@param tasks
				BT nodes
		*/
		/********************************************************************************/
		Selector(std::initializer_list<SMART> tasks = {})
			: Task{ "Selector" }, tasks{ tasks }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				override operator() for Selector

			@param log
				log osstream

			@param level
				string level

			@return
				action task
		*/
		/********************************************************************************/
		Task& operator()(Log* log = nullptr, std::string level = "");
		
	};

	// Sequence composite
	//     Returns immediately with a failure status code 
	//     when one of its children fails.
	class Sequence : public Task
	{
		std::list<SMART> tasks;

	public:

		/*!******************************************************************************/
		/*
			@brief
				Sequencer Constructor

			@param tasks
				BT nodes
		*/
		/********************************************************************************/
		Sequence(std::initializer_list<SMART> tasks = {})
			: Task{ "Sequence" }, tasks{ tasks }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				override operator() for sequencer

			@param log
				log osstream

			@param level
				string level

			@return
				action task
		*/
		/********************************************************************************/
		Task& operator()(Log* log = nullptr, std::string level = "");	

	};

	// Random selector composite
	//     Tries a single child at random.
	class RandomSelector : public Task
	{
		std::list<SMART> tasks;

	public:

		/*!******************************************************************************/
		/*
			@brief
				RandomSelector Constructor

			@param tasks
				BT nodes
		*/
		/********************************************************************************/
		RandomSelector(std::initializer_list<SMART> tasks = {})
			: Task{ "RandomSelector" }, tasks{ tasks }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				override operator() for RandomSelector

			@param log
				log osstream

			@param level
				string level

			@return
				action task
		*/
		/********************************************************************************/
		Task& operator()(Log* log = nullptr, std::string level = "");
		
	};


	// Decorators 


	// Inverter
	//     Invert the value returned by a task
	class Inverter : public Task
	{
		SMART task;

	public:

		/*!******************************************************************************/
		/*
			@brief
				Inverter Constructor

			@param tasks
				BT nodes
		*/
		/********************************************************************************/
		Inverter(SMART task = {})
			: Task{ "Inverter" }, task{ task }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				override operator() for Inverter

			@param log
				log osstream

			@param level
				string level

			@return
				action task
		*/
		/********************************************************************************/
		Task& operator()(Log* log = nullptr, std::string level = "");
		
	};

	// Succeeder
	//     Always return success, irrespective of what the child node actually returned.
	//     These are useful in cases where you want to process a branch of a tree 
	//     where a failure is expected or anticipated, but you don’t want to 
	//     abandon processing of a sequence that branch sits on.
	class Succeeder : public Task
	{
		SMART task;

	public:

		/*!******************************************************************************/
		/*
			@brief
				Succeeder Constructor

			@param tasks
				BT nodes
		*/
		/********************************************************************************/
		Succeeder(SMART task = {})
			: Task{ "Succeeder" }, task{ task }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				override operator() for Succeeder

			@param log
				log osstream

			@param level
				string level

			@return
				action task
		*/
		/********************************************************************************/
		Task& operator()(Log* log = nullptr, std::string level = "");

	};

	// Repeater
	//     A repeater will reprocess its child node each time its 
	//     child returns a result. These are often used at the very 
	//     base of the tree, to make the tree to run continuously. 
	//     Repeaters may optionally run their children a set number of 
	//     times before returning to their parent.
	class Repeater : public Task
	{
		SMART task;
		int counter;

	public:

		/*!******************************************************************************/
		/*
			@brief
				Repeater Constructor

			@param tasks
				BT nodes
		*/
		/********************************************************************************/
		Repeater(SMART task = {}, int counter = 0)
			: Task{ "Repeater" }, task{ task }, counter{ counter }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				override operator() for Repeater

			@param log
				log osstream

			@param level
				string level

			@return
				action task
		*/
		/********************************************************************************/
		Task& operator()(Log* log = nullptr, std::string level = "");

	};

	// Repeat_until_fail
	//      Like a repeater, these decorators will continue to 
	//      reprocess their child. That is until the child finally 
	//      returns a failure, at which point the repeater will 
	//      return success to its parent.
	class Repeat_until_fail : public Task
	{
		SMART task;

	public:

		/*!******************************************************************************/
		/*
			@brief
				Repeat_until_fail Constructor

			@param tasks
				BT nodes
		*/
		/********************************************************************************/
		Repeat_until_fail(SMART task = {})
			: Task{ "Repeat_until_fail" }, task{ task }
		{
		}

		/*!******************************************************************************/
		/*
			@brief
				override operator() for Repeat_until_fail

			@param log
				log osstream

			@param level
				string level

			@return
				action task
		*/
		/********************************************************************************/
		Task& operator()(Log* log = nullptr, std::string level = "");

	};

} // end namespace

#endif