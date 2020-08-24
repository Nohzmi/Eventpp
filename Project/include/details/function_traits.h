// Copyright (c) 2020, Nohzmi. All rights reserved.

/**
* @file fonction_traits.h
* @author Nohzmi
* @version 1.0
*/

#pragma once

namespace eventpp
{
	namespace details
	{
		template<typename FuncT>
		struct function_traits
		{
			using type = typename function_traits<decltype(&FuncT::operator())>::type;
		};

		template<typename Return, typename ...Args>
		struct function_traits<Return(Args...)>
		{
			using type = Return(*)(Args...);
		};

		template<typename ClassT, typename Return, typename ...Args>
		struct function_traits<Return(ClassT::*)(Args...)> final : public function_traits<Return(Args...)> {};

		template<typename ClassT, typename Return, typename ...Args>
		struct function_traits<Return(ClassT::*)(Args...) const> final : public function_traits<Return(Args...)> {};
	}
}
