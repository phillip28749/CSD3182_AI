/*!*****************************************************************************
\file functions.cpp
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 10
\date 07-09-2023
\brief

    No update for this file

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#include "functions.h"

namespace AI
{
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
	Task& CheckState::operator()(Log* log, std::string level) {

		if (log) *log << level << "CheckState" << '(' << checktask.getId() << ',' << STATES[checkstate] << ')' << std::endl;

		state = checktask.getState() == checkstate ? State::Success : State::Failure;

		if (log) *log << level << "L " << STATES[state] << std::endl;

		return *this;
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
	Task& Selector::operator()(Log* log, std::string level) {

		if (log) *log << level << "Selector()" << std::endl;

		state = State::Failure;

		for (auto task : tasks) {

			state = task->operator()(log, level + "| ").getState();
			if (state != State::Failure)
				break;
		}

		if (log) *log << level << "L " << STATES[state] << std::endl;

		return *this;
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
	Task& Sequence::operator()(Log* log, std::string level) {

		if (log) *log << level << "Sequence()" << std::endl;

		state = State::Success;

		for (auto task : tasks) {

			state = task->operator()(log, level + "| ").getState();
			if (state != State::Success)
				break;
		}

		if (log) *log << level << "L " << STATES[state] << std::endl;

		return *this;
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
	Task& RandomSelector::operator()(Log* log, std::string level) {

		if (log) *log << level << "RandomSelector()" << std::endl;

		state = State::Failure;

		if (!tasks.empty()) {

			size_t i = std::rand() % tasks.size();

			auto it = tasks.begin();
			std::advance(it, i);

			state = (*it)->operator()(log, level + "| ").getState();
		}

		if (log) *log << level << "L " << STATES[state] << std::endl;

		return *this;
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
	Task& Inverter::operator()(Log* log, std::string level) {

		if (log) *log << level << "Inverter()" << std::endl;

		task->operator()(log, level + "| ");

		state = (task->getState() == State::Success) ? State::Failure : State::Success;

		if (log) *log << level << "L " << STATES[state] << std::endl;

		return *this;
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
	Task& Succeeder::operator()(Log* log, std::string level) {

		if (log) *log << level << "Succeeder()" << std::endl;

		state = State::Success;

		task->operator()(log, level + "| ");

		if (log) *log << level << "L " << STATES[state] << std::endl;

		return *this;
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
	Task& Repeater::operator()(Log* log, std::string level) {

		if (log) *log << level << "Repeater" << '(' << counter << ')' << std::endl;

		state = State::Success;

		while (task && (counter--) > 0)
			task->operator()(log, level + "| ");

		if (log) *log << level << "L " << STATES[state] << std::endl;

		return *this;
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
	Task& Repeat_until_fail::operator()(Log* log, std::string level) {

		if (log) *log << level << "Repeat_until_fail()" << std::endl;
		state = State::Success;

		while (task && task->operator()(log, level + "| ").getState() == State::Success);

		if (log) *log << level << "L " << STATES[state] << std::endl;

		return *this;
	}
} // end namespace