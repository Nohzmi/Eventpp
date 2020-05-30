template<typename LambdaT, typename Return, typename ...Args>
inline LambdaBindCallback<LambdaT, Return(*)(Args...)>::LambdaBindCallback(LambdaT func, size_t hash, ArgsT args) noexcept :
	m_Func{ std::make_unique<LambdaT>(func) },
	m_Hash{ hash },
	m_Args{ args }
{
}

template<typename LambdaT, typename Return, typename ...Args>
inline bool LambdaBindCallback<LambdaT, Return(*)(Args...)>::operator==(const Callback<FuncT>& rhs) const noexcept
{
	const auto pc{ const_cast<Callback<FuncT>*>(&rhs) };
	const auto fc{ static_cast<LambdaBindCallback*>(pc) };
	const bool isFunc{ fc ? fc->m_Hash == m_Hash : false };
	const bool isArgs{ fc ? fc->m_Args == m_Args : false };
	return isFunc && isArgs;
}

template<typename LambdaT, typename Return, typename ...Args>
inline Return LambdaBindCallback<LambdaT, Return(*)(Args...)>::operator()() const noexcept
{
	return Apply(*m_Func, m_Args);
}

template<typename LambdaT, typename Return, typename ...Args>
inline std::unique_ptr<Callback<Return(*)()>> LambdaBindCallback<LambdaT, Return(*)(Args...)>::Clone() const noexcept
{
	return std::make_unique<LambdaBindCallback>(*m_Func, m_Hash, m_Args);
}
