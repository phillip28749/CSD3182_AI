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
		CheckState(Task checktask = {}, State checkstate = State::Success)
			: Task{ "CheckState" }, checktask{ checktask }, checkstate{ checkstate }
		{
		}

		Task& operator()(Log* log = nullptr, std::string level = "") {

			if (log) *log << level << "CheckState" << '(' << checktask.getId() << ',' << STATES[checkstate] << ')' << std::endl;

			state = checktask.getState() == checkstate ? State::Success : State::Failure;

			if (log) *log << level << "L " << STATES[state] << std::endl;

			return *this;
		}

	};

	// Selector composite
	//     Returns immediately with a success status code 
	//     when one of its children runs successfully.
	class Selector : public Task
	{
		std::list<SMART> tasks;

	public:
		Selector(std::initializer_list<SMART> tasks = {})
			: Task{ "Selector" }, tasks{ tasks }
		{
		}

		Task& operator()(Log* log = nullptr, std::string level = "") {

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
		
	};

	// Sequence composite
	//     Returns immediately with a failure status code 
	//     when one of its children fails.
	class Sequence : public Task
	{
		std::list<SMART> tasks;

	public:
		Sequence(std::initializer_list<SMART> tasks = {})
			: Task{ "Sequence" }, tasks{ tasks }
		{
		}

		Task& operator()(Log* log = nullptr, std::string level = "") {

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
		
	};

	// Random selector composite
	//     Tries a single child at random.
	class RandomSelector : public Task
	{
		std::list<SMART> tasks;

	public:
		RandomSelector(std::initializer_list<SMART> tasks = {})
			: Task{ "RandomSelector" }, tasks{ tasks }
		{
		}

		Task& operator()(Log* log = nullptr, std::string level = "") {

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
		
	};


	// Decorators 


	// Inverter
	//     Invert the value returned by a task
	class Inverter : public Task
	{
		SMART task;

	public:
		Inverter(SMART task = {})
			: Task{ "Inverter" }, task{ task }
		{
		}

		Task& operator()(Log* log = nullptr, std::string level = "") {

			if (log) *log << level << "Inverter()" << std::endl;

			task->operator()(log, level + "| ");

			state = (task->getState() == State::Success) ? State::Failure : State::Success;

			if (log) *log << level << "L " << STATES[state] << std::endl;

			return *this;
		}
		
		
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
		Succeeder(SMART task = {})
			: Task{ "Succeeder" }, task{ task }
		{
		}

		Task& operator()(Log* log = nullptr, std::string level = "") {

			if (log) *log << level << "Succeeder()" << std::endl;

			state = State::Success;

			task->operator()(log, level + "| ");

			if (log) *log << level << "L " << STATES[state] << std::endl;

			return *this;
		}

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
		Repeater(SMART task = {}, int counter = 0)
			: Task{ "Repeater" }, task{ task }, counter{ counter }
		{
		}

		Task& operator()(Log* log = nullptr, std::string level = "") {

			if (log) *log << level << "Repeater" << '(' << counter << ')' << std::endl;

			state = State::Success;

			while(task && (counter--) > 0)
				task->operator()(log, level + "| ");

			if (log) *log << level << "L " << STATES[state] << std::endl;

			return *this;
		}
		
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
		Repeat_until_fail(SMART task = {})
			: Task{ "Repeat_until_fail" }, task{ task }
		{
		}

		Task& operator()(Log* log = nullptr, std::string level = "") {

			if (log) *log << level << "Repeat_until_fail()" << std::endl;
			state = State::Success;

			while (task && task->operator()(log, level + "| ").getState() == State::Success);

			if (log) *log << level << "L " << STATES[state] << std::endl;

			return *this;
		}

	};

} // end namespace

#endif