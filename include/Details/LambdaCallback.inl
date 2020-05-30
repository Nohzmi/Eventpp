template<typename LambdaT, typename Return, typename ...Args>
inline LambdaCallback<LambdaT, Return(*)(Args...)>::LambdaCallback(LambdaT func, size_t hash) noexcept :
	m_Func{ std::make_unique<LambdaT>(func) },
	m_Hash{ hash }
{
}

template<typename LambdaT, typename Return, typename ...Args>
inline bool LambdaCallback<LambdaT, Return(*)(Args...)>::operator==(const Callback<FuncT>& rhs) const noexcept
{
	const auto pc{ const_cast<Callback<FuncT>*>(&rhs) };
	const auto fc{ static_cast<LambdaCallback*>(pc) };
	const bool isFunc{ fc ? fc->m_Hash == m_Hash : false };
	return isFunc;
}

template<typename LambdaT, typename Return, typename ...Args>
inline Return LambdaCallback<LambdaT, Return(*)(Args...)>::operator()(Args ...args) const noexcept
{
	return (*m_Func)(args...);
}

template<typename LambdaT, typename Return, typename ...Args>
inline std::unique_ptr<Callback<Return(*)(Args...)>> LambdaCallback<LambdaT, Return(*)(Args...)>::Clone() const noexcept
{
	return std::make_unique<LambdaCallback>(*m_Func, m_Hash);
}
