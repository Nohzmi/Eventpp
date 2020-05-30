/**
* @file Factory.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include "FunctionTraits.h"
#include "FreeCallback.h"
#include "FreeBindCallback.h"
#include "LambdaCallback.h"
#include "LambdaBindCallback.h"
#include "MemberCallback.h"
#include "MemberBindCallback.h"

/**
* @ingroup Tools
*
* Returns a callback on a free function
* @param func
*/
template<typename FuncT>
std::unique_ptr<FreeCallback<FuncT>> make_callback(FuncT&& func) noexcept;

/**
* @ingroup Tools
*
* Returns a callback on a free function \n
* Allow to bind arguments with the function
* @param func
* @param args
*/
template<typename FuncT, typename ...Args>
std::unique_ptr<FreeBindCallback<FuncT>> make_callback(FuncT&& func, Args&&... args) noexcept;

/**
* @ingroup Tools
*
* Create a callback on a member function
* @param func
* @param obj
*/
template<typename ClassT, typename FuncT>
std::unique_ptr<MemberCallback<ClassT, FuncT>> make_callback(FuncT&& func, ClassT*&& obj) noexcept;

/**
* @ingroup Tools
*
* Returns a callback on a member function \n
* Allow to bind arguments with the function
* @param func
* @param obj
* @param args
*/
template<typename ClassT, typename FuncT, typename ...Args>
std::unique_ptr<MemberBindCallback<ClassT, FuncT>> make_callback(FuncT&& func, ClassT*&& obj, Args&&... args) noexcept;

/**
* @ingroup Tools
*
* Returns a callback on a lambda function
* @param func
*/
template<typename LambdaT, typename FuncT = typename FunctionTraits<LambdaT>::Type>
std::unique_ptr<LambdaCallback<LambdaT, FuncT>> make_lambda(LambdaT func) noexcept;

/**
* @ingroup Tools
*
* Returns a callback on a lambda function \n
* Allow to bind arguments with the function
* @param func
* @param args
*/
template<typename LambdaT, typename FuncT = typename FunctionTraits<LambdaT>::Type, typename ...Args>
std::unique_ptr<LambdaBindCallback<LambdaT, FuncT>> make_lambda(LambdaT func, Args&&... args) noexcept;

#include "factory.inl"
