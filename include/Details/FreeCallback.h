/**
* @file FreeCallback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>
#include "Callback.h"

template<typename FuncT>
class FreeCallback;

/**
* @ingroup Tools
*
* Store a callback on a free function
*/
template<typename Return, typename ...Args>
class FreeCallback<Return(*)(Args...)> final : public Callback<Return(*)(Args...)>
{
	using FuncT = Return(*)(Args...);

public:

	/**
	* Constructor
	*/
	FreeCallback() = delete;

	/**
	* Destructor
	*/
	~FreeCallback() final = default;

	/**
	* Copy constructor
	*/
	FreeCallback(const FreeCallback&) = delete;

	/**
	* Move constructor
	*/
	FreeCallback(FreeCallback&&) noexcept = delete;

	/**
	* Copy assignement operator
	*/
	FreeCallback& operator=(const FreeCallback&) = delete;

	/**
	* Move assignement operator
	*/
	FreeCallback& operator=(FreeCallback&&) noexcept = delete;

	/**
	* Construct a free callback
	* @param func
	*/
	FreeCallback(FuncT func) noexcept;

	/**
	* Returns whether or not two callbacks are equals
	* @param rhs
	*/
	bool operator==(const Callback<FuncT>& rhs) const noexcept final;

	/**
	* Caller of the callback
	* @param args
	*/
	Return operator()(Args... args) const noexcept final;

	/**
	* Returns a copy of the current callback
	*/
	std::unique_ptr<Callback<FuncT>> Clone() const noexcept final;

private:

	FuncT m_Func;
};

#include "FreeCallback.inl"
