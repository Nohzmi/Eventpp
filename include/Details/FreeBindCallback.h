/**
* @file FreeBindCallback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>
#include <tuple>
#include "Callback.h"

template<typename FuncT>
class FreeBindCallback;

/**
* @ingroup Tools
*
* Store a callback on a free function
* Store binded arguments
*/
template<typename Return, typename ...Args>
class FreeBindCallback<Return(*)(Args...)> final : public Callback<Return(*)()>
{
	using FuncT = Return(*)(Args...);
	using CallerT = Return(*)();
	using ArgsT = std::tuple<Args...>;

public:

	/**
	* Constructor
	*/
	FreeBindCallback() = delete;

	/**
	* Destructor
	*/
	~FreeBindCallback() final = default;

	/**
	* Copy constructor
	*/
	FreeBindCallback(const FreeBindCallback&) = delete;

	/**
	* Move constructor
	*/
	FreeBindCallback(FreeBindCallback&&) noexcept = delete;

	/**
	* Copy assignement operator
	*/
	FreeBindCallback& operator=(const FreeBindCallback&) = delete;

	/**
	* Move assignement operator
	*/
	FreeBindCallback& operator=(FreeBindCallback&&) noexcept = delete;

	/**
	* Construct a free bind callback
	* @param func
	* @param args
	*/
	FreeBindCallback(FuncT func, ArgsT args) noexcept;

	/**
	* Returns whether or not two callbacks are equals
	* @param rhs
	*/
	bool operator==(const Callback<CallerT>& rhs) const noexcept final;

	/**
	* Caller of the callback
	*/
	Return operator()() const noexcept final;

	/**
	* Returns a copy of the current callback
	*/
	std::unique_ptr<Callback<CallerT>> Clone() const noexcept final;

private:

	template<typename FuncT, typename ArgsT>
	Return Apply(FuncT func, ArgsT args) const noexcept
	{
		static constexpr auto size = std::tuple_size_v<ArgsT>;
		return ApplyImpl(func, args, std::make_index_sequence<size>{});
	}

	template<typename FuncT, typename ArgsT, int... idx>
	Return ApplyImpl(FuncT func, ArgsT args, std::index_sequence<idx...>) const noexcept
	{
		return (func)(std::get<idx>(args)...);
	}

	FuncT m_Func;
	ArgsT m_Args;
};

#include "FreeBindCallback.inl"
