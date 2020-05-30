/**
* @file MemberCallback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>
#include "Callback.h"

template<typename ClassT, typename FuncT>
class MemberCallback;

/**
* @ingroup Tools
*
* Store a callback on a member function
*/
template<typename ClassT, typename Return, typename ...Args>
class MemberCallback<ClassT, Return(ClassT::*)(Args...)> final : public Callback<Return(*)(Args...)>
{
	using FuncClassT = Return(ClassT::*)(Args...);
	using CallerT = Return(*)(Args...);

public:

	/**
	* Constructor
	*/
	MemberCallback() = delete;

	/**
	* Destructor
	*/
	~MemberCallback() final = default;

	/**
	* Copy constructor
	*/
	MemberCallback(const MemberCallback&) = delete;

	/**
	* Move constructor
	*/
	MemberCallback(MemberCallback&&) noexcept = delete;

	/**
	* Copy assignement operator
	*/
	MemberCallback& operator=(const MemberCallback&) = delete;

	/**
	* Move assignement operator
	*/
	MemberCallback& operator=(MemberCallback&&) noexcept = delete;

	/**
	* Construct a member callback
	* @param func
	* @param callee
	*/
	MemberCallback(FuncClassT func, ClassT* callee) noexcept;

	/**
	* Returns whether or not two callbacks are equals
	* @param rhs
	*/
	bool operator==(const Callback<CallerT>& rhs) const noexcept final;

	/**
	* Caller of the callback
	* @param args
	*/
	Return operator()(Args... args) const noexcept final;

	/**
	* Returns a copy of the current callback
	*/
	std::unique_ptr<Callback<CallerT>> Clone() const noexcept final;

private:

	FuncClassT m_Func;
	ClassT* m_Callee;
};

#include "MemberCallback.inl"
