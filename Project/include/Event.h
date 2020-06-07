// Copyright (c) 2020, Nohzmi. All rights reserved.

/**
* @file Event.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <vector>
#include "Callback.h"

/**
* @addtogroup Eventpp
* @{
*/

template<typename FuncT>
class Event;

/**
* Store multiple functions
*/
template<typename Return, typename ...Args>
class Event<Return(*)(Args...)> final
{
	using FuncT = Return(*)(Args...);
	using CallbackT = std::unique_ptr<Eventpp::Callback<FuncT>>;

public:

	/**
	* Constructor
	*/
	Event() = default;

	/**
	* Destructor
	*/
	~Event() = default;

	/**
	* Copy constructor
	*/
	Event(const Event& copy)
	{
		m_Callbacks.clear();

		for (const CallbackT& callback : copy.m_Callbacks)
			m_Callbacks.emplace_back(callback->Clone());
	}

	/**
	* Move constructor
	*/
	Event(Event&&) noexcept = default;

	/**
	* Copy assignement operator
	*/
	Event& operator=(const Event& copy)
	{
		m_Callbacks.clear();

		for (const CallbackT& callback : copy.m_Callbacks)
			m_Callbacks.emplace_back(callback->Clone());

		return this;
	}

	/**
	* Move assignement operator
	*/
	Event& operator=(Event&&) noexcept = default;

	/**
	* Subscribe a function \n
	* See make_callback() and make_lambda()
	* @param callback
	*/
	void operator+=(CallbackT callback) noexcept
	{
		for (auto i{ 0u }; i < m_Callbacks.size(); ++i)
			if (*callback == *m_Callbacks[i])
				return;

		m_Callbacks.emplace_back(std::forward<CallbackT>(callback));
	}

	/**
	* Unsubscribe a function \n
	* See make_callback() and make_lambda()
	* @param callback
	*/
	void operator-=(CallbackT callback) noexcept
	{
		for (auto i{ 0u }; i < m_Callbacks.size(); ++i)
		{
			if (*callback == *m_Callbacks[i])
			{
				m_Callbacks.erase(m_Callbacks.begin() + i);
				return;
			}
		}
	}

	/**
	* Clear all subscribed functions
	*/
	void Clear()
	{
		m_Callbacks.clear();
	}

	/**
	* Call all subscribed functions
	* @param args
	*/
	void Invoke(Args... args) const noexcept
	{
		for (unsigned int i{ 0 }; i < m_Callbacks.size(); ++i)
			(*m_Callbacks[i])(std::forward<Args>(args)...);
	}

	/**
	* Returns whether or not the event stores functions
	*/
	bool IsEmpty() noexcept
	{
		return m_Callbacks.size() == 0;
	}

private:

	std::vector<CallbackT> m_Callbacks;
};

/**
* @}
*/
