template<typename Return, typename ...Args>
inline Event<Return(*)(Args...)>::Event(const Event& event)
{
	for (const CallbackT& callback : event.m_Callbacks)
		m_Callbacks.emplace_back(callback->Clone());
}

template<typename Return, typename ...Args>
inline Event<Return(*)(Args...)>& Event<Return(*)(Args...)>::operator=(const Event& event)
{
	for (const CallbackT& callback : event.m_Callbacks)
		m_Callbacks.emplace_back(callback->Clone());
	return this;
}

template<typename Return, typename ...Args>
inline void Event<Return(*)(Args...)>::operator+=(CallbackT callback) noexcept
{
	for (auto i{ 0u }; i < m_Callbacks.size(); ++i)
		if (*callback == *m_Callbacks[i])
			return;
	m_Callbacks.emplace_back(std::forward<CallbackT>(callback));
}

template<typename Return, typename ...Args>
inline void Event<Return(*)(Args...)>::operator-=(CallbackT callback) noexcept
{
	for (auto i{ 0u }; i < m_Callbacks.size(); ++i)
	{
		if (*callback == *m_Callbacks[i])
		{
			m_Callbacks.erase(m_Callbacks.begin() + i);
			return;
		}
	}
}

template<typename Return, typename ...Args>
inline void Event<Return(*)(Args...)>::Clear()
{
	m_Callbacks.clear();
}

template<typename Return, typename ...Args>
inline void Event<Return(*)(Args...)>::Invoke(Args ...args) const noexcept
{
	for (unsigned int i{ 0 }; i < m_Callbacks.size(); ++i)
		(*m_Callbacks[i])(std::forward<Args>(args)...);
}

template<typename Return, typename ...Args>
inline bool Event<Return(*)(Args...)>::IsEmpty() noexcept
{
	return m_Callbacks.size() == 0;
}
