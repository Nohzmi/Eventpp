/**
* @file LambdaCallback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>
#include "Callback.h"

template<typename LambdaT, typename FuncT = LambdaT>
class LambdaCallback;

/**
* @ingroup Tools
*
* Store a callback on a lambda function
*/
template<typename LambdaT, typename Return, typename ...Args>
class LambdaCallback<LambdaT, Return(*)(Args...)> final : public Callback<Return(*)(Args...)>
{
	using FuncT = Return(*)(Args...);

public:

	/**
	* Constructor
	*/
	LambdaCallback() = delete;

	/**
	* Destructor
	*/
	~LambdaCallback() final = default;

	/**
	* Copy constructor
	*/
	LambdaCallback(const LambdaCallback&) = delete;

	/**
	* Move constructor
	*/
	LambdaCallback(LambdaCallback&&) noexcept = delete;

	/**
	* Copy assignement operator
	*/
	LambdaCallback& operator=(const LambdaCallback&) = delete;

	/**
	* Move assignement operator
	*/
	LambdaCallback& operator=(LambdaCallback&&) noexcept = delete;

	/**
	* Construct a lambda callback
	* @param func
	* @param hash
	*/
	LambdaCallback(LambdaT func, size_t hash) noexcept;

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

	std::unique_ptr<LambdaT> m_Func;
	size_t m_Hash;
};

#include "LambdaCallback.inl"
