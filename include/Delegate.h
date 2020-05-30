/**
* @file Delegate.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include "Details/Factory.h"
#include "Details/FreeCallback.h"
#include "Details/FreeBindCallback.h"
#include "Details/LambdaCallback.h"
#include "Details/LambdaBindCallback.h"
#include "Details/MemberCallback.h"
#include "Details/MemberBindCallback.h"

template<typename FuncT>
class Delegate;

/**
* @ingroup Tools
*
* Store a function
*/
template<typename Return, typename ...Args>
class Delegate<Return(*)(Args...)> final
{
	using FuncT = Return(*)(Args...);
	using CallbackT = std::unique_ptr<Callback<FuncT>>;

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
	Delegate(const Delegate&);

	/**
	* Move constructor
	*/
	Delegate(Delegate&&) noexcept = default;

	/**
	* Copy assignement operator
	*/
	Delegate& operator=(const Delegate&);

	/**
	* Move assignement operator
	*/
	Delegate& operator=(Delegate&&) noexcept = default;

	/**
	* Replace stored function \n
	* See make_callback() and make_lambda()
	* @param callback
	*/
	Delegate& operator=(CallbackT callback) noexcept;

	/**
	* Call stored function
	* @param args
	*/
	Return operator()(Args... args) const noexcept;

	/**
	* Returns whether or not it store a function
	*/
	operator bool() const noexcept;

	/**
	* Clear stored functions
	*/
	void Clear() noexcept;

	/**
	* Returns whether or not the delegate stores a function
	*/
	bool IsEmpty() noexcept;

private:

	CallbackT m_Callback;
};

#include "Details/Delegate.inl"

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Example: (free callback)
//    Delegate<void(*)(myType)> myDelegate;
//    myDelegate = make_callback(&myFreeFunction);
//    myDelegate(myArg);
//
//
// Example: (free callback with binded arguments)
//
//    Delegate<void(*)()> myDelegate;
//    myDelegate = make_callback(&myFreeFunction, myArg, ...);
//    myDelegate();
//
//
// Example: (member callback)
//
//    Delegate<void(*)(myType)> myDelegate;
//    myDelegate = make_callback(&myClass::myMemberFunction, &myClassinstance);
//    myDelegate(myArg);
//
//
// Example: (member callback with binded arguments)
//
//    Delegate<void(*)()> myDelegate;
//    myDelegate = make_callback(&myClass::myMemberFunction, &myClassinstance, myArg, ...);
//    myDelegate();
//
//
// Example: (lambda callback)
//
//    Delegate<void(*)()> myDelegate;
//    myDelegate = make_lambda([]() {...});
//    myDelegate = make_lambda(myFunctor());
//
//
// Example: (lambda callback with binded arguments)
//
//    Delegate<void(*)()> myDelegate;
//    myDelegate = make_lambda([]() {...}, arg, ...);
//    myDelegate = make_lambda(myFunctor(), arg, ...);
//    myDelegate();
//
//
// Example: (reassignment)
//
//    Delegate<void(*)()> myDelegate;
//    myDelegate = make_callback(&myFreeFunction); // not called
//    myDelegate = make_callback(&myOtherFreeFunction); // called
//    myDelegate();
//
//
// Example: (bool conversion)
//
//    Delegate<void(*)()> myDelegate;
//    Delegate<void(*)()> myOtherDelegate;
//    myDelegate = make_callback(&myFreeFunction);
//    if (myDelegate) // true
//    if (myOtherDelegate) // false
//
/////////////////////////////////////////////////////////////////////////////////////////////
