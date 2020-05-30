template<typename Return, typename ...Args>
inline FreeCallback<Return(*)(Args...)>::FreeCallback(FuncT func) noexcept :
	m_Func{ func }
{
}

template<typename Return, typename ...Args>
inline bool FreeCallback<Return(*)(Args...)>::operator==(const Callback<FuncT>& rhs) const noexcept
{
	const auto pc{ const_cast<Callback<FuncT>*>(&rhs) };
	const auto fc{ static_cast<FreeCallback*>(pc) };
	const bool isFunc{ fc ? fc->m_Func == m_Func : false };
	return isFunc;
}

template<typename Return, typename ...Args>
inline Return FreeCallback<Return(*)(Args...)>::operator()(Args ...args) const noexcept
{
	return m_Func(args...);
}

template<typename Return, typename ...Args>
inline std::unique_ptr<Callback<Return(*)(Args...)>> FreeCallback<Return(*)(Args...)>::Clone() const noexcept
{
	return std::make_unique<FreeCallback>(m_Func);
}
