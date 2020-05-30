/**
* @file MemberBindCallback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>
#include <tuple>
#include "Callback.h"

template<typename ClassT, typename FuncT>
class MemberBindCallback;

/**
* @ingroup Tools
*
* Store a callback on a member function
* Store binded arguments
*/
template<typename ClassT, typename Return, typename ...Args>
class MemberBindCallback<ClassT, Return(ClassT::*)(Args...)> final : public Callback<Return(*)()>
{
	using FuncClassT = Return(ClassT::*)(Args...);
	using CallerT = Return(*)();
	using ArgsT = std::tuple<Args...>;

public:

	/**
	* Constructor
	*/
	MemberBindCallback() = delete;

	/**
	* Destructor
	*/
	~MemberBindCallback() final = default;

	/**
	* Copy constructor
	*/
	MemberBindCallback(const MemberBindCallback&) = delete;

	/**
	* Move constructor
	*/
	MemberBindCallback(MemberBindCallback&&) noexcept = delete;

	/**
	* Copy assignement operator
	*/
	MemberBindCallback& operator=(const MemberBindCallback&) = delete;

	/**
	* Move assignement operator
	*/
	MemberBindCallback& operator=(MemberBindCallback&&) noexcept = delete;

	/**
	* Construct a member bind callback
	* @param func
	* @param callee
	* @param args
	*/
	MemberBindCallback(FuncClassT func, ClassT* callee, ArgsT args) noexcept;

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

	template<typename FuncClassT, typename ClassT, typename ArgsT>
	Return Apply(FuncClassT func, ClassT* callee, ArgsT args) const noexcept
	{
		static constexpr auto size = std::tuple_size_v<ArgsT>;
		return ApplyImpl(func, callee, args, std::make_index_sequence<size>{});
	}

	template<typename FuncClassT, typename ClassT, typename ArgsT, size_t... idx>
	Return ApplyImpl(FuncClassT func, ClassT* callee, ArgsT args, std::index_sequence<idx...>) const noexcept
	{
		return (callee->*func)(std::get<idx>(args)...);
	}

	FuncClassT m_Func;
	ClassT* m_Callee;
	ArgsT m_Args;
};

#include "MemberBindCallback.inl"
