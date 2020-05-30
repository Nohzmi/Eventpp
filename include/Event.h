/**
* @file Event.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <vector>
#include "Details/Factory.h"
#include "Details/FreeCallback.h"
#include "Details/FreeBindCallback.h"
#include "Details/LambdaCallback.h"
#include "Details/LambdaBindCallback.h"
#include "Details/MemberCallback.h"
#include "Details/MemberBindCallback.h"

template<typename FuncT>
class Event;

/**
* @ingroup Tools
*
* Store multiple functions
*/
template<typename Return, typename ...Args>
class Event<Return(*)(Args...)> final
{
	using FuncT = Return(*)(Args...);
	using CallbackT = std::unique_ptr<Callback<FuncT>>;

public:

	/**
	* Constructor
	*/
	Event() = default;

	/**
	* Destructor
	*/
	~Event() = default;

	/**
	* Copy constructor
	*/
	Event(const Event&);

	/**
	* Move constructor
	*/
	Event(Event&&) noexcept = default;

	/**
	* Copy assignement operator
	*/
	Event& operator=(const Event&);

	/**
	* Move assignement operator
	*/
	Event& operator=(Event&&) noexcept = default;

	/**
	* Subscribe a function \n
	* See make_callback() and make_lambda()
	* @param callback
	*/
	void operator+=(CallbackT callback) noexcept;

	/**
	* Unsubscribe a function \n
	* See make_callback() and make_lambda()
	* @param callback
	*/
	void operator-=(CallbackT callback) noexcept;

	/**
	* Clear all subscribed functions
	*/
	void Clear();

	/**
	* Call all subscribed functions
	* @param args
	*/
	void Invoke(Args... args) const noexcept;

	/**
	* Returns whether or not the event stores functions
	*/
	bool IsEmpty() noexcept;

private:

	std::vector<CallbackT> m_Callbacks;
};

#include "Details/Event.inl"

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Example: (free callback)
//
//    Event<void(*)(myType)> myEvent;
//    myEvent += make_callback(&myFreeFunction);
//    myEvent.invoke(myArg);
//
//
// Example: (free callback with binded arguments)
//
//    Event<void(*)()> myEvent;
//    myEvent += make_callback(&myFreeFunction, myArg, ...);
//    myEvent.invoke();
//
//
// Example: (lambda callback)
//
//    Event<void(*)()> myEvent;
//    myEvent += make_lambda([]() {...});
//    myEvent += make_lambda(myFunctor());
//    myEvent.invoke();
//
//
// Example: (lambda callback with binded arguments)
//
//    Event<void(*)()> myEvent;
//    myEvent += make_lambda([]() {...}, arg, ...);
//    myEvent += make_lambda(myFunctor(), arg, ...);
//    myEvent.invoke();
//
//
// Example: (member callback)
//
//    Event<void(*)(myType)> myEvent;
//    myEvent += make_callback(&myClass::myMemberFunction, &myClassinstance);
//    myEvent.invoke(myArg);
//
//
// Example: (member callback with binded arguments)
//
//    Event<void(*)()> myEvent;
//    myEvent += make_callback(&myClass::myMemberFunction, &myClassinstance, myArg, ...);
//    myEvent.invoke();
//
//
// Example: (Unsubscribe)
//
//    Event<void(*)(myType)> myEvent;
//    myEvent += make_callback(&myFreeFunction); // not called
//    myEvent += make_callback(&myOtherFreeFunction); // called
//    myEvent -= make_callback(&myFreeFunction);
//    myEvent.invoke(myArg);
//
//
// Example: (Unsubscribe with binded arguments)
//
//    Event<void(*)()> myEvent;
//    myEvent += make_callback(&myFreeFunction, myArg, ...); // not called
//    myEvent += make_callback(&myFreeFunction, myOtherArg, ...); // called
//    myEvent -= make_callback(&myFreeFunction, myArg, ...);
//    myEvent.invoke();
//
/////////////////////////////////////////////////////////////////////////////////////////////
