template<typename Return, typename ...Args>
inline FreeBindCallback<Return(*)(Args...)>::FreeBindCallback(FuncT func, ArgsT args) noexcept :
	m_Func{ func },
	m_Args{ args }
{
}

template<typename Return, typename ...Args>
inline bool FreeBindCallback<Return(*)(Args...)>::operator==(const Callback<CallerT>& rhs) const noexcept
{
	const auto pc{ const_cast<Callback<CallerT>*>(&rhs) };
	const auto fc{ static_cast<FreeBindCallback*>(pc) };
	const bool isFunc{ fc ? fc->m_Func == m_Func : false };
	const bool isArgs{ fc ? fc->m_Args == m_Args : false };
	return isFunc && isArgs;
}

template<typename Return, typename ...Args>
inline Return FreeBindCallback<Return(*)(Args...)>::operator()() const noexcept
{
	return Apply(m_Func, m_Args);
}

template<typename Return, typename ...Args>
inline std::unique_ptr<Callback<Return(*)()>> FreeBindCallback<Return(*)(Args...)>::Clone() const noexcept
{
	return std::make_unique<FreeBindCallback>(m_Func, m_Args);
}
