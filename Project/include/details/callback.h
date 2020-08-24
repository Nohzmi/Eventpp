// Copyright (c) 2020, Nohzmi. All rights reserved.

/**
* @file callback.h
* @author Nohzmi
* @version 1.0
*/

#pragma once
#include <memory>
#include <tuple>

namespace eventpp
{
	namespace details
	{
		template<typename FuncT>
		class callback;

		template<typename Return, typename ...Args>
		class callback<Return(*)(Args...)>
		{
			using FuncT = Return(*)(Args...);

		public:

			callback() = default;
			virtual ~callback() = default;
			callback(const callback&) = delete;
			callback(callback&&) noexcept = delete;
			callback& operator=(const callback&) = delete;
			callback& operator=(callback&&) noexcept = delete;

			virtual bool operator==(const callback<Return(*)(Args...)>& rhs) const noexcept = 0;
			virtual Return operator()(Args... args) const noexcept = 0;
			virtual std::unique_ptr<callback<FuncT>> clone() const noexcept = 0;
		};

		template<typename FuncT>
		class free_callback;

		template<typename Return, typename ...Args>
		class free_callback<Return(*)(Args...)> final : public callback<Return(*)(Args...)>
		{
			using FuncT = Return(*)(Args...);

		public:

			free_callback() = delete;
			~free_callback() final = default;
			free_callback(const free_callback&) = delete;
			free_callback(free_callback&&) noexcept = delete;
			free_callback& operator=(const free_callback&) = delete;
			free_callback& operator=(free_callback&&) noexcept = delete;

			free_callback(FuncT func) noexcept :
				m_func{ func }
			{
			}

			bool operator==(const callback<FuncT>& rhs) const noexcept final
			{
				const auto pc{ const_cast<callback<FuncT>*>(&rhs) };
				const auto fc{ static_cast<free_callback*>(pc) };
				const bool is_func{ fc ? fc->m_func == m_func : false };

				return is_func;
			}

			Return operator()(Args... args) const noexcept final
			{
				return m_func(args...);
			}

			std::unique_ptr<callback<FuncT>> clone() const noexcept final
			{
				return std::make_unique<free_callback>(m_func);
			}

		private:

			FuncT m_func;
		};

		template<typename ClassT, typename FuncT>
		class member_callback;

		template<typename ClassT, typename Return, typename ...Args>
		class member_callback<ClassT, Return(ClassT::*)(Args...)> final : public callback<Return(*)(Args...)>
		{
			using FuncClassT = Return(ClassT::*)(Args...);
			using CallerT = Return(*)(Args...);

		public:

			member_callback() = delete;
			~member_callback() final = default;
			member_callback(const member_callback&) = delete;
			member_callback(member_callback&&) noexcept = delete;
			member_callback& operator=(const member_callback&) = delete;
			member_callback& operator=(member_callback&&) noexcept = delete;

			member_callback(FuncClassT func, ClassT* callee) noexcept :
				m_func{ func },
				m_callee{ callee }
			{
			}

			bool operator==(const callback<CallerT>& rhs) const noexcept final
			{
				const auto pc{ const_cast<callback<CallerT>*>(&rhs) };
				const auto mc{ static_cast<member_callback*>(pc) };
				const bool is_func{ mc ? mc->m_func == m_func : false };
				const bool is_callee{ mc ? mc->m_callee == m_callee : false };

				return is_func && is_callee;
			}

			Return operator()(Args... args) const noexcept final
			{
				return (m_callee->*m_func)(args...);
			}

			std::unique_ptr<callback<CallerT>> clone() const noexcept final
			{
				return std::make_unique<member_callback>(m_func, m_callee);
			}

		private:

			FuncClassT m_func;
			ClassT* m_callee;
		};

		template<typename LambdaT, typename FuncT = LambdaT>
		class lambda_callback;

		template<typename LambdaT, typename Return, typename ...Args>
		class lambda_callback<LambdaT, Return(*)(Args...)> final : public callback<Return(*)(Args...)>
		{
			using FuncT = Return(*)(Args...);

		public:

			lambda_callback() = delete;
			~lambda_callback() final = default;
			lambda_callback(const lambda_callback&) = delete;
			lambda_callback(lambda_callback&&) noexcept = delete;
			lambda_callback& operator=(const lambda_callback&) = delete;
			lambda_callback& operator=(lambda_callback&&) noexcept = delete;

			lambda_callback(LambdaT func, size_t hash) noexcept :
				m_func{ std::make_unique<LambdaT>(func) },
				m_hash{ hash }
			{
			}

			bool operator==(const callback<FuncT>& rhs) const noexcept final
			{
				const auto pc{ const_cast<callback<FuncT>*>(&rhs) };
				const auto fc{ static_cast<lambda_callback*>(pc) };
				const bool is_func{ fc ? fc->m_hash == m_hash : false };

				return is_func;
			}

			Return operator()(Args... args) const noexcept final
			{
				return (*m_func)(args...);
			}

			std::unique_ptr<callback<FuncT>> clone() const noexcept final
			{
				return std::make_unique<lambda_callback>(*m_func, m_hash);
			}

		private:

			std::unique_ptr<LambdaT> m_func;
			size_t m_hash;
		};

		template<typename FuncT>
		class free_bind_callback;

		template<typename Return, typename ...Args>
		class free_bind_callback<Return(*)(Args...)> final : public callback<Return(*)()>
		{
			using FuncT = Return(*)(Args...);
			using CallerT = Return(*)();
			using ArgsT = std::tuple<Args...>;

		public:

			free_bind_callback() = delete;
			~free_bind_callback() final = default;
			free_bind_callback(const free_bind_callback&) = delete;
			free_bind_callback(free_bind_callback&&) noexcept = delete;
			free_bind_callback& operator=(const free_bind_callback&) = delete;
			free_bind_callback& operator=(free_bind_callback&&) noexcept = delete;

			free_bind_callback(FuncT func, ArgsT args) noexcept :
				m_func{ func },
				m_args{ args }
			{
			}

			bool operator==(const callback<CallerT>& rhs) const noexcept final
			{
				const auto pc{ const_cast<callback<CallerT>*>(&rhs) };
				const auto fc{ static_cast<free_bind_callback*>(pc) };
				const bool is_func{ fc ? fc->m_func == m_func : false };
				const bool is_args{ fc ? fc->m_args == m_args : false };

				return is_func && is_args;
			}

			Return operator()() const noexcept final
			{
				return apply(m_func, m_args);
			}

			std::unique_ptr<callback<CallerT>> clone() const noexcept final
			{
				return std::make_unique<free_bind_callback>(m_func, m_args);
			}

		private:

			template<typename FuncT, typename ArgsT>
			Return apply(FuncT func, ArgsT args) const noexcept
			{
				static constexpr auto size = std::tuple_size_v<ArgsT>;
				return apply_impl(func, args, std::make_index_sequence<size>{});
			}

			template<typename FuncT, typename ArgsT, int... idx>
			Return apply_impl(FuncT func, ArgsT args, std::index_sequence<idx...>) const noexcept
			{
				return (func)(std::get<idx>(args)...);
			}

			FuncT m_func;
			ArgsT m_args;
		};

		template<typename ClassT, typename FuncT>
		class member_bind_callback;

		template<typename ClassT, typename Return, typename ...Args>
		class member_bind_callback<ClassT, Return(ClassT::*)(Args...)> final : public callback<Return(*)()>
		{
			using FuncClassT = Return(ClassT::*)(Args...);
			using CallerT = Return(*)();
			using ArgsT = std::tuple<Args...>;

		public:

			member_bind_callback() = delete;
			~member_bind_callback() final = default;
			member_bind_callback(const member_bind_callback&) = delete;
			member_bind_callback(member_bind_callback&&) noexcept = delete;
			member_bind_callback& operator=(const member_bind_callback&) = delete;
			member_bind_callback& operator=(member_bind_callback&&) noexcept = delete;

			member_bind_callback(FuncClassT func, ClassT* callee, ArgsT args) noexcept :
				m_func{ func },
				m_callee{ callee },
				m_args{ args }
			{
			}

			bool operator==(const callback<CallerT>& rhs) const noexcept final
			{
				const auto pc{ const_cast<callback<CallerT>*>(&rhs) };
				const auto mc{ static_cast<member_bind_callback*>(pc) };
				const bool is_func{ mc ? mc->m_func == m_func : false };
				const bool is_callee{ mc ? mc->m_callee == m_callee : false };
				const bool is_args{ mc ? mc->m_args == m_args : false };

				return is_func && is_callee && is_args;
			}

			Return operator()() const noexcept final
			{
				return apply(m_func, m_callee, m_args);
			}

			std::unique_ptr<callback<CallerT>> clone() const noexcept final
			{
				return std::make_unique<member_bind_callback>(m_func, m_callee, m_args);
			}

		private:

			template<typename FuncClassT, typename ClassT, typename ArgsT>
			Return apply(FuncClassT func, ClassT* callee, ArgsT args) const noexcept
			{
				static constexpr auto size = std::tuple_size_v<ArgsT>;
				return apply_impl(func, callee, args, std::make_index_sequence<size>{});
			}

			template<typename FuncClassT, typename ClassT, typename ArgsT, size_t... idx>
			Return apply_impl(FuncClassT func, ClassT* callee, ArgsT args, std::index_sequence<idx...>) const noexcept
			{
				return (callee->*func)(std::get<idx>(args)...);
			}

			FuncClassT m_func;
			ClassT* m_callee;
			ArgsT m_args;
		};

		template<typename LambdaT, typename FuncT = LambdaT>
		class lambda_bind_callback;

		template<typename LambdaT, typename Return, typename ...Args>
		class lambda_bind_callback<LambdaT, Return(*)(Args...)> final : public callback<Return(*)()>
		{
			using FuncT = Return(*)();
			using ArgsT = std::tuple<Args...>;

		public:

			lambda_bind_callback() = delete;
			~lambda_bind_callback() final = default;
			lambda_bind_callback(const lambda_bind_callback&) = delete;
			lambda_bind_callback(lambda_bind_callback&&) noexcept = delete;
			lambda_bind_callback& operator=(const lambda_bind_callback&) = delete;
			lambda_bind_callback& operator=(lambda_bind_callback&&) noexcept = delete;

			lambda_bind_callback(LambdaT func, size_t hash, ArgsT args) noexcept :
				m_func{ std::make_unique<LambdaT>(func) },
				m_hash{ hash },
				m_args{ args }
			{
			}

			bool operator==(const callback<FuncT>& rhs) const noexcept final
			{
				const auto pc{ const_cast<callback<FuncT>*>(&rhs) };
				const auto fc{ static_cast<lambda_bind_callback*>(pc) };
				const bool is_func{ fc ? fc->m_hash == m_hash : false };
				const bool is_args{ fc ? fc->m_args == m_args : false };

				return is_func && is_args;
			}

			Return operator()() const noexcept final
			{
				return apply(*m_func, m_args);
			}

			std::unique_ptr<callback<FuncT>> clone() const noexcept final
			{
				return std::make_unique<lambda_bind_callback>(*m_func, m_hash, m_args);
			}

		private:

			template<typename FuncT, typename ArgsT>
			Return apply(FuncT func, ArgsT args) const noexcept
			{
				static constexpr auto size = std::tuple_size_v<ArgsT>;
				return apply_impl(func, args, std::make_index_sequence<size>{});
			}

			template<typename FuncT, typename ArgsT, size_t... idx>
			Return apply_impl(FuncT func, ArgsT args, std::index_sequence<idx...>) const noexcept
			{
				return (func)(std::get<idx>(args)...);
			}

			std::unique_ptr<LambdaT> m_func;
			size_t m_hash;
			ArgsT m_args;
		};
	}
}
