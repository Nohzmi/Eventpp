// Copyright (c) 2020, Nohzmi. All rights reserved.

/**
* @file Callback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>
#include <tuple>

/**
* @addtogroup Eventpp
* @{
*/

/**
* Contains all internal classes of Eventpp \n
* Mainly callback inheritance hidden from user
*/
namespace Details
{
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

	template<typename FuncT>
	class Callback;

	/**
	* Store a callback on a function
	*/
	template<typename Return, typename ...Args>
	class Callback<Return(*)(Args...)>
	{
		using FuncT = Return(*)(Args...);

	public:

		/**
		* Constructor
		*/
		Callback() = default;

		/**
		* Destructor
		*/
		virtual ~Callback() = default;

		/**
		* Copy constructor
		*/
		Callback(const Callback&) = delete;

		/**
		* Move constructor
		*/
		Callback(Callback&&) noexcept = delete;

		/**
		* Copy assignement operator
		*/
		Callback& operator=(const Callback&) = delete;

		/**
		* Move assignement operator
		*/
		Callback& operator=(Callback&&) noexcept = delete;

		/**
		* Returns whether or not two callbacks are equals
		* @param rhs
		*/
		virtual bool operator==(const Callback<Return(*)(Args...)>& rhs) const noexcept = 0;

		/**
		* Caller of the callback
		* @param args
		*/
		virtual Return operator()(Args... args) const noexcept = 0;

		/**
		* Returns a copy of the current callback
		*/
		virtual std::unique_ptr<Callback<FuncT>> Clone() const noexcept = 0;
	};

	template<typename FuncT>
	class FreeCallback;

	/**
	* Store a callback on a free function
	*/
	template<typename Return, typename ...Args>
	class FreeCallback<Return(*)(Args...)> final : public Callback<Return(*)(Args...)>
	{
		using FuncT = Return(*)(Args...);

	public:

		/**
		* Constructor
		*/
		FreeCallback() = delete;

		/**
		* Destructor
		*/
		~FreeCallback() final = default;

		/**
		* Copy constructor
		*/
		FreeCallback(const FreeCallback&) = delete;

		/**
		* Move constructor
		*/
		FreeCallback(FreeCallback&&) noexcept = delete;

		/**
		* Copy assignement operator
		*/
		FreeCallback& operator=(const FreeCallback&) = delete;

		/**
		* Move assignement operator
		*/
		FreeCallback& operator=(FreeCallback&&) noexcept = delete;

		/**
		* Construct a free callback
		* @param func
		*/
		FreeCallback(FuncT func) noexcept :
			m_Func{ func }
		{
		}

		/**
		* Returns whether or not two callbacks are equals
		* @param rhs
		*/
		bool operator==(const Callback<FuncT>& rhs) const noexcept final
		{
			const auto pc{ const_cast<Callback<FuncT>*>(&rhs) };
			const auto fc{ static_cast<FreeCallback*>(pc) };
			const bool isFunc{ fc ? fc->m_Func == m_Func : false };

			return isFunc;
		}

		/**
		* Caller of the callback
		* @param args
		*/
		Return operator()(Args... args) const noexcept final
		{
			return m_Func(args...);
		}

		/**
		* Returns a copy of the current callback
		*/
		std::unique_ptr<Callback<FuncT>> Clone() const noexcept final
		{
			return std::make_unique<FreeCallback>(m_Func);
		}

	private:

		FuncT m_Func;
	};

	template<typename ClassT, typename FuncT>
	class MemberCallback;

	/**
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
		MemberCallback(FuncClassT func, ClassT* callee) noexcept :
			m_Func{ func },
			m_Callee{ callee }
		{
		}

		/**
		* Returns whether or not two callbacks are equals
		* @param rhs
		*/
		bool operator==(const Callback<CallerT>& rhs) const noexcept final
		{
			const auto pc{ const_cast<Callback<CallerT>*>(&rhs) };
			const auto mc{ static_cast<MemberCallback*>(pc) };
			const bool isFunc{ mc ? mc->m_Func == m_Func : false };
			const bool isCallee{ mc ? mc->m_Callee == m_Callee : false };

			return isFunc && isCallee;
		}

		/**
		* Caller of the callback
		* @param args
		*/
		Return operator()(Args... args) const noexcept final
		{
			return (m_Callee->*m_Func)(args...);
		}

		/**
		* Returns a copy of the current callback
		*/
		std::unique_ptr<Callback<CallerT>> Clone() const noexcept final
		{
			return std::make_unique<MemberCallback>(m_Func, m_Callee);
		}

	private:

		FuncClassT m_Func;
		ClassT* m_Callee;
	};

	template<typename LambdaT, typename FuncT = LambdaT>
	class LambdaCallback;

	/**
	* Store a callback on a lambda function
	*/
	template<typename LambdaT, typename Return, typename ...Args>
	class LambdaCallback<LambdaT, Return(*)(Args...)> final : public Callback<Return(*)(Args...)>
	{
		using FuncT = Return(*)(Args...);

	public:

		/**
		* Constructor
		*/
		LambdaCallback() = delete;

		/**
		* Destructor
		*/
		~LambdaCallback() final = default;

		/**
		* Copy constructor
		*/
		LambdaCallback(const LambdaCallback&) = delete;

		/**
		* Move constructor
		*/
		LambdaCallback(LambdaCallback&&) noexcept = delete;

		/**
		* Copy assignement operator
		*/
		LambdaCallback& operator=(const LambdaCallback&) = delete;

		/**
		* Move assignement operator
		*/
		LambdaCallback& operator=(LambdaCallback&&) noexcept = delete;

		/**
		* Construct a lambda callback
		* @param func
		* @param hash
		*/
		LambdaCallback(LambdaT func, size_t hash) noexcept :
			m_Func{ std::make_unique<LambdaT>(func) },
			m_Hash{ hash }
		{
		}

		/**
		* Returns whether or not two callbacks are equals
		* @param rhs
		*/
		bool operator==(const Callback<FuncT>& rhs) const noexcept final
		{
			const auto pc{ const_cast<Callback<FuncT>*>(&rhs) };
			const auto fc{ static_cast<LambdaCallback*>(pc) };
			const bool isFunc{ fc ? fc->m_Hash == m_Hash : false };

			return isFunc;
		}

		/**
		* Caller of the callback
		* @param args
		*/
		Return operator()(Args... args) const noexcept final
		{
			return (*m_Func)(args...);
		}

		/**
		* Returns a copy of the current callback
		*/
		std::unique_ptr<Callback<FuncT>> Clone() const noexcept final
		{
			return std::make_unique<LambdaCallback>(*m_Func, m_Hash);
		}

	private:

		std::unique_ptr<LambdaT> m_Func;
		size_t m_Hash;
	};

	template<typename FuncT>
	class FreeBindCallback;

	/**
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
		FreeBindCallback(FuncT func, ArgsT args) noexcept :
			m_Func{ func },
			m_Args{ args }
		{
		}

		/**
		* Returns whether or not two callbacks are equals
		* @param rhs
		*/
		bool operator==(const Callback<CallerT>& rhs) const noexcept final
		{
			const auto pc{ const_cast<Callback<CallerT>*>(&rhs) };
			const auto fc{ static_cast<FreeBindCallback*>(pc) };
			const bool isFunc{ fc ? fc->m_Func == m_Func : false };
			const bool isArgs{ fc ? fc->m_Args == m_Args : false };

			return isFunc && isArgs;
		}

		/**
		* Caller of the callback
		*/
		Return operator()() const noexcept final
		{
			return Apply(m_Func, m_Args);
		}

		/**
		* Returns a copy of the current callback
		*/
		std::unique_ptr<Callback<CallerT>> Clone() const noexcept final
		{
			return std::make_unique<FreeBindCallback>(m_Func, m_Args);
		}

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

	template<typename ClassT, typename FuncT>
	class MemberBindCallback;

	/**
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
		MemberBindCallback(FuncClassT func, ClassT* callee, ArgsT args) noexcept :
			m_Func{ func },
			m_Callee{ callee },
			m_Args{ args }
		{
		}

		/**
		* Returns whether or not two callbacks are equals
		* @param rhs
		*/
		bool operator==(const Callback<CallerT>& rhs) const noexcept final
		{
			const auto pc{ const_cast<Callback<CallerT>*>(&rhs) };
			const auto mc{ static_cast<MemberBindCallback*>(pc) };
			const bool isFunc{ mc ? mc->m_Func == m_Func : false };
			const bool isCallee{ mc ? mc->m_Callee == m_Callee : false };
			const bool isArgs{ mc ? mc->m_Args == m_Args : false };

			return isFunc && isCallee && isArgs;
		}

		/**
		* Caller of the callback
		*/
		Return operator()() const noexcept final
		{
			return Apply(m_Func, m_Callee, m_Args);
		}

		/**
		* Returns a copy of the current callback
		*/
		std::unique_ptr<Callback<CallerT>> Clone() const noexcept final
		{
			return std::make_unique<MemberBindCallback>(m_Func, m_Callee, m_Args);
		}

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

	template<typename LambdaT, typename FuncT = LambdaT>
	class LambdaBindCallback;

	/**
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
		LambdaBindCallback(LambdaT func, size_t hash, ArgsT args) noexcept :
			m_Func{ std::make_unique<LambdaT>(func) },
			m_Hash{ hash },
			m_Args{ args }
		{
		}

		/**
		* Returns whether or not two callbacks are equals
		* @param rhs
		*/
		bool operator==(const Callback<FuncT>& rhs) const noexcept final
		{
			const auto pc{ const_cast<Callback<FuncT>*>(&rhs) };
			const auto fc{ static_cast<LambdaBindCallback*>(pc) };
			const bool isFunc{ fc ? fc->m_Hash == m_Hash : false };
			const bool isArgs{ fc ? fc->m_Args == m_Args : false };

			return isFunc && isArgs;
		}

		/**
		* Caller of the callback
		*/
		Return operator()() const noexcept final
		{
			return Apply(*m_Func, m_Args);
		}

		/**
		* Returns a copy of the current callback
		*/
		std::unique_ptr<Callback<FuncT>> Clone() const noexcept final
		{
			return std::make_unique<LambdaBindCallback>(*m_Func, m_Hash, m_Args);
		}

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
}

/**
* Returns a callback on a free function
* @param func
*/
template<typename FuncT>
std::unique_ptr<Details::FreeCallback<FuncT>> make_callback(FuncT&& func) noexcept
{
	return std::make_unique<Details::FreeCallback<FuncT>>(func);
}

/**
* Returns a callback on a free function \n
* Allow to bind arguments with the function
* @param func
* @param args
*/
template<typename FuncT, typename ...Args>
std::unique_ptr<Details::FreeBindCallback<FuncT>> make_callback(FuncT&& func, Args&&... args) noexcept
{
	return std::make_unique<Details::FreeBindCallback<FuncT>>(func, std::make_tuple(args...));
}

/**
* Returns a callback on a member function
* @param func
* @param obj
*/
template<typename ClassT, typename FuncT>
std::unique_ptr<Details::MemberCallback<ClassT, FuncT>> make_callback(FuncT&& func, ClassT*&& obj) noexcept
{
	return std::make_unique<Details::MemberCallback<ClassT, FuncT>>(func, obj);
}

/**
* Returns a callback on a member function \n
* Allow to bind arguments with the function
* @param func
* @param obj
* @param args
*/
template<typename ClassT, typename FuncT, typename ...Args>
std::unique_ptr<Details::MemberBindCallback<ClassT, FuncT>> make_callback(FuncT&& func, ClassT*&& obj, Args&&... args) noexcept
{
	return std::make_unique<Details::MemberBindCallback<ClassT, FuncT>>(func, obj, std::make_tuple(args...));
}

/**
* Returns a callback on a lambda function
* @param func
*/
template<typename LambdaT, typename FuncT = typename Details::FunctionTraits<LambdaT>::Type>
std::unique_ptr<Details::LambdaCallback<LambdaT, FuncT>> make_lambda(LambdaT func) noexcept
{
	return std::make_unique<Details::LambdaCallback<LambdaT, FuncT>>(func, typeid(func).hash_code());
}

/**
* Returns a callback on a lambda function \n
* Allow to bind arguments with the function
* @param func
* @param args
*/
template<typename LambdaT, typename FuncT = typename Details::FunctionTraits<LambdaT>::Type, typename ...Args>
std::unique_ptr<Details::LambdaBindCallback<LambdaT, FuncT>> make_lambda(LambdaT func, Args&&... args) noexcept
{
	return std::make_unique<Details::LambdaBindCallback<LambdaT, FuncT>>(func, typeid(func).hash_code(), std::make_tuple(args...));
}

/**
* @}
*/
