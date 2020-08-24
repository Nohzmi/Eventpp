// Copyright (c) 2020, Nohzmi. All rights reserved.

/**
* @file delegate.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include "factory.h"

/**
* @addtogroup Eventpp
* @{
*/

namespace eventpp
{
	template<typename FuncT>
	class delegate;

	/**
	* Store a function
	*/
	template<typename Return, typename ...Args>
	class delegate<Return(*)(Args...)> final
	{
		using FuncT = Return(*)(Args...);
		using CallbackT = std::unique_ptr<details::callback<FuncT>>;

	public:

		delegate() = default;
		~delegate() = default;
		delegate(delegate&&) noexcept = default;
		delegate& operator=(delegate&&) noexcept = default;

		delegate(const delegate& copy)
		{
			if (copy.m_callback)
				m_callback = copy.m_callback->clone();
		}

		delegate& operator=(const delegate& copy)
		{
			if (copy.m_callback)
				m_callback = copy.m_callback->copy();

			return this;
		}

		/**
		* Replace stored function \n
		* See make_callback() and make_lambda()
		* @param callback
		*/
		delegate& operator=(CallbackT callback) noexcept
		{
			m_callback = std::forward<CallbackT>(callback);
			return *this;
		}

		/**
		* Call stored function
		* @param args
		*/
		Return operator()(Args... args) const noexcept
		{
			return m_callback ? (*m_callback)(args...) : Return();
		}

		/**
		* Returns whether or not it store a function
		*/
		operator bool() const noexcept
		{
			return static_cast<bool>(m_callback);
		}

		/**
		* Clear stored functions
		*/
		void clear() noexcept
		{
			m_callback.reset(nullptr);
		}

	private:

		CallbackT m_callback;
	};
}

/**
* @}
*/
