// Copyright (c) 2020, Nohzmi. All rights reserved.

/**
* @file event.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <vector>
#include "factory.h"

/**
* @addtogroup Eventpp
* @{
*/

namespace eventpp
{
	template<typename FuncT>
	class event;

	/**
	* Store multiple functions
	*/
	template<typename Return, typename ...Args>
	class event<Return(*)(Args...)> final
	{
		using FuncT = Return(*)(Args...);
		using CallbackT = std::unique_ptr<details::callback<FuncT>>;

	public:

		event() = default;
		~event() = default;
		event(event&&) noexcept = default;
		event& operator=(event&&) noexcept = default;

		event(const event& copy)
		{
			m_callbacks.clear();

			for (const CallbackT& callback : copy.m_callbacks)
				m_callbacks.emplace_back(callback->clone());
		}

		event& operator=(const event& copy)
		{
			m_callbacks.clear();

			for (const CallbackT& callback : copy.m_callbacks)
				m_callbacks.emplace_back(callback->clone());

			return this;
		}

		/**
		* Subscribe a function \n
		* See make_callback() and make_lambda()
		* @param callback
		*/
		void operator+=(CallbackT callback) noexcept
		{
			for (auto i{ 0u }; i < m_callbacks.size(); ++i)
				if (*callback == *m_callbacks[i])
					return;

			m_callbacks.emplace_back(std::forward<CallbackT>(callback));
		}

		/**
		* Unsubscribe a function \n
		* See make_callback() and make_lambda()
		* @param callback
		*/
		void operator-=(CallbackT callback) noexcept
		{
			for (auto i{ 0u }; i < m_callbacks.size(); ++i)
			{
				if (*callback == *m_callbacks[i])
				{
					m_callbacks.erase(m_callbacks.begin() + i);
					return;
				}
			}
		}

		/**
		* Clear all subscribed functions
		*/
		void clear()
		{
			m_callbacks.clear();
		}

		/**
		* Call all subscribed functions
		* @param args
		*/
		void invoke(Args... args) const noexcept
		{
			for (auto i{ 0u }; i < m_callbacks.size(); ++i)
				(*m_callbacks[i])(std::forward<Args>(args)...);
		}

		/**
		* Returns whether or not the event stores functions
		*/
		bool empty() noexcept
		{
			return m_callbacks.size() == 0;
		}

	private:

		std::vector<CallbackT> m_callbacks;
	};
}

/**
* @}
*/
