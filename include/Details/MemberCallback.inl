template<typename ClassT, typename Return, typename ...Args>
inline MemberCallback<ClassT, Return(ClassT::*)(Args...)>::MemberCallback(FuncClassT func, ClassT* callee) noexcept :
	m_Func{ func },
	m_Callee{ callee }
{
}

template<typename ClassT, typename Return, typename ...Args>
inline bool MemberCallback<ClassT, Return(ClassT::*)(Args...)>::operator==(const Callback<CallerT>& rhs) const noexcept
{
	const auto pc{ const_cast<Callback<CallerT>*>(&rhs) };
	const auto mc{ static_cast<MemberCallback*>(pc) };
	const bool isFunc{ mc ? mc->m_Func == m_Func : false };
	const bool isCallee{ mc ? mc->m_Callee == m_Callee : false };
	return isFunc && isCallee;
}

template<typename ClassT, typename Return, typename ...Args>
inline Return MemberCallback<ClassT, Return(ClassT::*)(Args...)>::operator()(Args ...args) const noexcept
{
	return (m_Callee->*m_Func)(args...);
}

template<typename ClassT, typename Return, typename ...Args>
inline std::unique_ptr<Callback<Return(*)(Args...)>> MemberCallback<ClassT, Return(ClassT::*)(Args...)>::Clone() const noexcept
{
	return std::make_unique<MemberCallback>(m_Func, m_Callee);
}
