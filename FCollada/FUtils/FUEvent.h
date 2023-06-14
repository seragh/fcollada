/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/
/*
	This file was taken off the Protect project on 26-09-2005
*/

/**
	@file FUEvent.h
	This file contains templates to contain and trigger callback events.
*/


#ifndef _FU_EVENT_H_
#define _FU_EVENT_H_

#include <functional>
#include <vector>

/**
	An event with no argument.
	@ingroup FUtils
*/
template <typename... Args>
class FUEvent
{
private:
	std::vector<std::tuple<void*, std::function<void(Args...)>>> handlers;
public:

	/** Constructor. */

	/** Destructor. */

	/**
	 * Retrieves the number of callbacks registered for this event.
	 * @return The number of callbacks registered.
	 */
	size_t GetHandlerCount() { return handlers.size(); }

	/** Adds a new callback that handles the event.
		@param _function The static function to callback. */
	void InsertHandler(void* object, std::function<void(Args...)> func)
	{
		handlers.emplace_back(std::tuple(object,func));
	}

	/** Releases and unregisters a callback that handles the event.
		@param handle The handle of the function container.
			This pointer will be NULL for static functions.
		@param function The address of the function callback to unregister. */
	void ReleaseHandler(void* object)
	{
		handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
					[object](auto t){ return std::get<0>(t) == object; }));
	}

	/**
	 * Triggers the event.
	 * All the registered callbacks will be called, in reverse-order
	 * of their registration.
	 */
	void operator()(Args... args)
	{
		for (std::size_t i = handlers.size(); i > 0; --i)
			(std::get<1>(handlers[i - 1]))(args...);
	}
};

#endif // _FU_EVENT_H_
