/**
* @file FunctionTraits.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <tuple>

template<typename FuncT>
class FunctionTraits
{
	using CallType = FunctionTraits<decltype(&FuncT::operator())>;

public:

	using Type = typename CallType::Type;

	struct Return
	{
		using Type = typename CallType::Return::Type;
	};

	template <size_t idx>
	struct Argument
	{
		using type = typename CallType::template Argument<idx + 1>::Type;
	};
};

template<typename Return, typename ...Args>
struct FunctionTraits<Return(Args...)>
{
	using Type = Return(*)(Args...);

	struct Return
	{
		using Type = Return;
	};

	template <size_t idx>
	struct Argument
	{
		using Type = typename std::tuple_element<idx, std::tuple<Args...>>::type;
	};
};

template<typename FuncT>
struct FunctionTraits<FuncT*> final : public FunctionTraits<FuncT> {};

template<typename FuncT>
struct FunctionTraits<FuncT**> final : public FunctionTraits<FuncT> {};

template<typename FuncT>
struct FunctionTraits<FuncT&> final : public FunctionTraits<FuncT> {};

template<typename FuncT>
struct FunctionTraits<FuncT&&> final : public FunctionTraits<FuncT> {};

template<typename Return, typename ...Args>
struct FunctionTraits<Return(*)(Args...)> final : public FunctionTraits<Return(Args...)> {};

template<typename ClassT, typename Return, typename ...Args>
struct FunctionTraits<Return(ClassT::*)(Args...)> final : public FunctionTraits<Return(Args...)> {};

template<typename ClassT, typename Return, typename ...Args>
struct FunctionTraits<Return(ClassT::*)(Args...) const> final : public FunctionTraits<Return(Args...)> {};

template<typename ClassT, typename Return>
struct FunctionTraits<Return(ClassT::*)> final : public FunctionTraits<Return()> {};
