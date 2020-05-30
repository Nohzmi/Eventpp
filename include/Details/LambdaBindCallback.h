/**
* @file LambdaBindCallback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>
#include <tuple>
#include "Callback.h"

template<typename LambdaT, typename FuncT = LambdaT>
class LambdaBindCallback;

/**
* @ingroup Tools
*
* Store a callback on a lambda function
* Store binded arguments
*/
template<typename LambdaT, typename Return, typename ...Args>
class LambdaBindCallback<LambdaT, Return(*)(Args...)> final : public Callback<Return(*)()>
{
	using FuncT = Return(*)();
	using ArgsT = std::tuple<Args...>;

public:

	/**
	* Constructor
	*/
	LambdaBindCallback() = delete;

	/**
	* Destructor
	*/
	~LambdaBindCallback() final = default;

	/**
	* Copy constructor
	*/
	LambdaBindCallback(const LambdaBindCallback&) = delete;

	/**
	* Move constructor
	*/
	LambdaBindCallback(LambdaBindCallback&&) noexcept = delete;

	/**
	* Copy assignement operator
	*/
	LambdaBindCallback& operator=(const LambdaBindCallback&) = delete;

	/**
	* Move assignement operator
	*/
	LambdaBindCallback& operator=(LambdaBindCallback&&) noexcept = delete;

	/**
	* Construct a member bind callback
	* @param func
	* @param hash
	* @param args
	*/
	LambdaBindCallback(LambdaT func, size_t hash, ArgsT args) noexcept;

	/**
	* Returns whether or not two callbacks are equals
	* @param rhs
	*/
	bool operator==(const Callback<FuncT>& rhs) const noexcept final;

	/**
	* Caller of the callback
	*/
	Return operator()() const noexcept final;

	/**
	* Returns a copy of the current callback
	*/
	std::unique_ptr<Callback<FuncT>> Clone() const noexcept final;

private:

	template<typename FuncT, typename ArgsT>
	Return Apply(FuncT func, ArgsT args) const noexcept
	{
		static constexpr auto size = std::tuple_size_v<ArgsT>;
		return ApplyImpl(func, args, std::make_index_sequence<size>{});
	}

	template<typename FuncT, typename ArgsT, size_t... idx>
	Return ApplyImpl(FuncT func, ArgsT args, std::index_sequence<idx...>) const noexcept
	{
		return (func)(std::get<idx>(args)...);
	}

	std::unique_ptr<LambdaT> m_Func;
	size_t m_Hash;
	ArgsT m_Args;
};

#include "LambdaBindCallback.inl"
