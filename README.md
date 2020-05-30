Eventpp
====
> C++ Event/Delegate Library

Eventpp implement a simple event/delegate system in C++

How to Use
----------
```cpp
Event<void(*)()> event;
event += make_callback(&FreeFunction);
event += make_callback(&FreeFunctionWithParams, 0);
event += make_callback(&SomeClass::MemberFunction, &obj);
event += make_callback(&SomeClass::MemberFunctionWithParams, &obj, 1);
event += make_lambda(Functor());
event += make_lambda(FunctorWithParams(), 2);
event += make_lambda([] { printf("Lambda\n"); });
event += make_lambda([](int param) { printf("LambdaWithParams %d\n", param); }, 3);
event.Invoke();

Delegate<void(*)()> delegate;
delegate = make_callback(&FreeFunction);
delegate();
```

Features
---------
- Callback on free functions
- Callback on member functions 
- Callback on lambda functions
- Callback on functors
- Callback on free functions with binded arguments
- Callback on member functions with binded arguments
- Callback on lambda functions with binded arguments
- Callback on functors with binded arguments
- Full documentation

Portability
-----------
Tested and compiled with:
- Microsoft Visual Studio 2019 (x86 or x64)
- Support c++11 or more

Installation
------------
Header-only library