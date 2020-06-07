// Copyright (c) 2020, Nohzmi. All rights reserved.

/**
* @file Delegate.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include "Callback.h"

/**
* @addtogroup Eventpp
* @{
*/

template<typename FuncT>
class Delegate;

/**
* Store a function
*/
template<typename Return, typename ...Args>
class Delegate<Return(*)(Args...)> final
{
	using FuncT = Return(*)(Args...);
	using CallbackT = std::unique_ptr<Eventpp::Callback<FuncT>>;

public:

	/**
	* Constructor
	*/
	Delegate() = default;

	/**
	* Destructor
	*/
	~Delegate() = default;

	/**
	* Copy constructor
	*/
	Delegate(const Delegate& copy)
	{
		if (copy.m_Callback)
			m_Callback = copy.m_Callback->Clone();
	}

	/**
	* Move constructor
	*/
	Delegate(Delegate&&) noexcept = default;

	/**
	* Copy assignement operator
	*/
	Delegate& operator=(const Delegate& copy)
	{
		if (copy.m_Callback)
			m_Callback = copy.m_Callback->copy();

		return this;
	}

	/**
	* Move assignement operator
	*/
	Delegate& operator=(Delegate&&) noexcept = default;

	/**
	* Replace stored function \n
	* See make_callback() and make_lambda()
	* @param callback
	*/
	Delegate& operator=(CallbackT callback) noexcept
	{
		m_Callback = std::forward<CallbackT>(callback);
		return *this;
	}

	/**
	* Call stored function
	* @param args
	*/
	Return operator()(Args... args) const noexcept
	{
		return m_Callback ? (*m_Callback)(args...) : Return();
	}

	/**
	* Returns whether or not it store a function
	*/
	operator bool() const noexcept
	{
		return static_cast<bool>(m_Callback);
	}

	/**
	* Clear stored functions
	*/
	void Clear() noexcept
	{
		m_Callback.reset(nullptr);
	}

private:

	CallbackT m_Callback;
};

/**
* @}
*/
