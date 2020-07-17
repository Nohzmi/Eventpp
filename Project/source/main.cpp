// Copyright (c) 2020, Nohzmi. All rights reserved.

#include <cstdio>
#include "delegate.h"
#include "event.h"

void FreeFunction()
{
	printf("FreeFunction\n");
}

void FreeFunctionWithParams(int param)
{
	printf("FreeFunctionWithParams %d\n", param);
}

struct SomeClass
{
	void MemberFunction()
	{
		printf("MemberFunction\n");
	}

	void MemberFunctionWithParams(int param)
	{
		printf("MemberFunctionWithParams %d\n", param);
	}
};

struct Functor
{
	void operator()()
	{
		printf("Functor\n");
	}
};

struct FunctorWithParams
{
	void operator()(int param)
	{
		printf("FunctorWithParams %d\n", param);
	}
};

using namespace eventpp;

int main()
{
	SomeClass obj;

	event<void(*)()> event;
	event += make_callback(&FreeFunction);
	event += make_callback(&FreeFunctionWithParams, 0);
	event += make_callback(&SomeClass::MemberFunction, &obj);
	event += make_callback(&SomeClass::MemberFunctionWithParams, &obj, 1);
	event += make_lambda(Functor());
	event += make_lambda(FunctorWithParams(), 2);
	event += make_lambda([] { printf("Lambda\n"); });
	event += make_lambda([](int param) { printf("LambdaWithParams %d\n", param); }, 3);
	event.invoke();

	delegate<void(*)()> delegate;
	delegate = make_callback(&FreeFunction);
	delegate();

	return 0;
}
