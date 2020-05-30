template<typename ClassT, typename Return, typename ...Args>
inline MemberBindCallback<ClassT, Return(ClassT::*)(Args...)>::MemberBindCallback(FuncClassT func, ClassT* callee, ArgsT args) noexcept :
	m_Func{ func },
	m_Callee{ callee },
	m_Args{ args }
{
}

template<typename ClassT, typename Return, typename ...Args>
inline bool MemberBindCallback<ClassT, Return(ClassT::*)(Args...)>::operator==(const Callback<CallerT>& rhs) const noexcept
{
	const auto pc{ const_cast<Callback<CallerT>*>(&rhs) };
	const auto mc{ static_cast<MemberBindCallback*>(pc) };
	const bool isFunc{ mc ? mc->m_Func == m_Func : false };
	const bool isCallee{ mc ? mc->m_Callee == m_Callee : false };
	const bool isArgs{ mc ? mc->m_Args == m_Args : false };
	return isFunc && isCallee && isArgs;
}

template<typename ClassT, typename Return, typename ...Args>
inline Return MemberBindCallback<ClassT, Return(ClassT::*)(Args...)>::operator()() const noexcept
{
	return Apply(m_Func, m_Callee, m_Args);
}

template<typename ClassT, typename Return, typename ...Args>
inline std::unique_ptr<Callback<Return(*)()>> MemberBindCallback<ClassT, Return(ClassT::*)(Args...)>::Clone() const noexcept
{
	return std::make_unique<MemberBindCallback>(m_Func, m_Callee, m_Args);
}
