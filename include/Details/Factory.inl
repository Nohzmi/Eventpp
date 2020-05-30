template<typename FuncT>
inline std::unique_ptr<FreeCallback<FuncT>> make_callback(FuncT&& func) noexcept
{
	return std::make_unique<FreeCallback<FuncT>>(func);
}

template<typename FuncT, typename ...Args>
inline std::unique_ptr<FreeBindCallback<FuncT>> make_callback(FuncT&& func, Args&& ...args) noexcept
{
	return std::make_unique<FreeBindCallback<FuncT>>(func, std::make_tuple(args...));
}

template<typename ClassT, typename FuncT>
inline std::unique_ptr<MemberCallback<ClassT, FuncT>> make_callback(FuncT&& func, ClassT*&& obj) noexcept
{
	return std::make_unique<MemberCallback<ClassT, FuncT>>(func, obj);
}

template<typename ClassT, typename FuncT, typename ...Args>
inline std::unique_ptr<MemberBindCallback<ClassT, FuncT>> make_callback(FuncT&& func, ClassT*&& obj, Args&& ...args) noexcept
{
	return std::make_unique<MemberBindCallback<ClassT, FuncT>>(func, obj, std::make_tuple(args...));
}

template<typename LambdaT, typename FuncT>
inline std::unique_ptr<LambdaCallback<LambdaT, FuncT>> make_lambda(LambdaT func) noexcept
{
	return std::make_unique<LambdaCallback<LambdaT, FuncT>>(func, typeid(func).hash_code());
}

template<typename LambdaT, typename FuncT, typename ...Args>
inline std::unique_ptr<LambdaBindCallback<LambdaT, FuncT>> make_lambda(LambdaT func, Args&& ...args) noexcept
{
	return std::make_unique<LambdaBindCallback<LambdaT, FuncT>>(func, typeid(func).hash_code(), std::make_tuple(args...));
}
