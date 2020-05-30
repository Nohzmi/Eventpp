template<typename Return, typename ...Args>
inline Delegate<Return(*)(Args...)>::Delegate(const Delegate& delegate)
{
	if (delegate.m_Callback)
		m_Callback = delegate.m_Callback->Clone();
}

template<typename Return, typename ...Args>
inline Delegate<Return(*)(Args...)>& Delegate<Return(*)(Args...)>::operator=(const Delegate& delegate)
{
	if (delegate.m_Callback)
		m_Callback = delegate.m_Callback->copy();
	return this;
}

template<typename Return, typename ...Args>
inline Delegate<Return(*)(Args...)>& Delegate<Return(*)(Args...)>::operator=(CallbackT callback) noexcept
{
	m_Callback = std::forward<CallbackT>(callback);
	return *this;
}

template<typename Return, typename ...Args>
inline Return Delegate<Return(*)(Args...)>::operator()(Args... args) const noexcept
{
	return m_Callback ? (*m_Callback)(args...) : Return();
}

template<typename Return, typename ...Args>
inline Delegate<Return(*)(Args...)>::operator bool() const noexcept
{
	return IsEmpty();
}

template<typename Return, typename ...Args>
inline void Delegate<Return(*)(Args...)>::Clear() noexcept
{
	m_Callback.reset(nullptr);
}

template<typename Return, typename ...Args>
inline bool Delegate<Return(*)(Args...)>::IsEmpty() noexcept
{
	return static_cast<bool>(m_Callback);
}
