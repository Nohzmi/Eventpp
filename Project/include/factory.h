// Copyright (c) 2020, Nohzmi. All rights reserved.

/**
* @file factory.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include "details/callback.h"
#include "details/function_traits.h"

namespace eventpp
{
	/**
	* Returns a callback on a free function
	* @param func
	*/
	template<typename FuncT>
	std::unique_ptr<details::free_callback<FuncT>> make_callback(FuncT&& func) noexcept
	{
		return std::make_unique<details::free_callback<FuncT>>(func);
	}

	/**
	* Returns a callback on a free function \n
	* Allow to bind arguments with the function
	* @param func
	* @param args
	*/
	template<typename FuncT, typename ...Args>
	std::unique_ptr<details::free_bind_callback<FuncT>> make_callback(FuncT&& func, Args&&... args) noexcept
	{
		return std::make_unique<details::free_bind_callback<FuncT>>(func, std::make_tuple(args...));
	}

	/**
	* Returns a callback on a member function
	* @param func
	* @param obj
	*/
	template<typename ClassT, typename FuncT>
	std::unique_ptr<details::member_callback<ClassT, FuncT>> make_callback(FuncT&& func, ClassT*&& obj) noexcept
	{
		return std::make_unique<details::member_callback<ClassT, FuncT>>(func, obj);
	}

	/**
	* Returns a callback on a member function \n
	* Allow to bind arguments with the function
	* @param func
	* @param obj
	* @param args
	*/
	template<typename ClassT, typename FuncT, typename ...Args>
	std::unique_ptr<details::member_bind_callback<ClassT, FuncT>> make_callback(FuncT&& func, ClassT*&& obj, Args&&... args) noexcept
	{
		return std::make_unique<details::member_bind_callback<ClassT, FuncT>>(func, obj, std::make_tuple(args...));
	}

	/**
	* Returns a callback on a lambda function
	* @param func
	*/
	template<typename LambdaT, typename FuncT = typename details::function_traits<LambdaT>::type>
	std::unique_ptr<details::lambda_callback<LambdaT, FuncT>> make_lambda(LambdaT func) noexcept
	{
		return std::make_unique<details::lambda_callback<LambdaT, FuncT>>(func, typeid(func).hash_code());
	}

	/**
	* Returns a callback on a lambda function \n
	* Allow to bind arguments with the function
	* @param func
	* @param args
	*/
	template<typename LambdaT, typename FuncT = typename details::function_traits<LambdaT>::type, typename ...Args>
	std::unique_ptr<details::lambda_bind_callback<LambdaT, FuncT>> make_lambda(LambdaT func, Args&&... args) noexcept
	{
		return std::make_unique<details::lambda_bind_callback<LambdaT, FuncT>>(func, typeid(func).hash_code(), std::make_tuple(args...));
	}
}
