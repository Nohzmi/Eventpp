/**
* @file Callback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>

template<typename FuncT>
class Callback;

/**
* @ingroup Tools
*
* Store a callback on a function
*/
template<typename Return, typename ...Args>
class Callback<Return(*)(Args...)>
{
	using FuncT = Return(*)(Args...);

public:

	/**
	* Constructor
	*/
	Callback() = default;

	/**
	* Destructor
	*/
	virtual ~Callback() = default;

	/**
	* Copy constructor
	*/
	Callback(const Callback&) = delete;

	/**
	* Move constructor
	*/
	Callback(Callback&&) noexcept = delete;

	/**
	* Copy assignement operator
	*/
	Callback& operator=(const Callback&) = delete;

	/**
	* Move assignement operator
	*/
	Callback& operator=(Callback&&) noexcept = delete;

	/**
	* Returns whether or not two callbacks are equals
	* @param rhs
	*/
	virtual bool operator==(const Callback<Return(*)(Args...)>& rhs) const noexcept = 0;

	/**
	* Caller of the callback
	* @param args
	*/
	virtual Return operator()(Args... args) const noexcept = 0;

	/**
	* Returns a copy of the current callback
	*/
	virtual std::unique_ptr<Callback<FuncT>> Clone() const noexcept = 0;
};
