# Design Patterns

Design Patterns implemented in C++, using a custom defined Smart Pointer.

### How to use the Smart Pointer

#### Allocating an integer to the heap

```cpp
SmartPtr<int> ptr = SmartPtr<int>(new int(10));
```

#### Its use count will be one:

```cpp
std::cout << "Use count: " << ptr.use_count(); // Use count: 1
```

#### Get the raw pointer:

```cpp
int* rawPtr = ptr.get();
std::cout << "Raw pointer: " << rawPtr; // 0x1df5c9315c0
```

#### And then get the value back:

```cpp
std::cout << "Value: " << *rawPtr << std::endl;
```

#### When you create a new SmartPtr that point to the same object, then use count will increase:

```cpp
SmartPtr<int> ptr2 = ptr;
std::cout << "Use count: " << ptr.use_count(); // Use count: 2
```

#### And when you move the ownership, the use count of the original object will be reset:

```cpp
SmartPtr<int> ptr3 = std::move(ptr);
std::cout << "Use count for ptr: " << ptr.use_count() << std::endl; // Use count: 0
std::cout << "Use count for ptr2: " << ptr2.use_count() << std::endl; // Use count: 2
std::cout << "Use count for ptr3: " << ptr3.use_count() << std::endl; // Use count: 2
```

> **Warning**
> These will only work with C++ 17 onwards.

> **Note**
> May have ridiculous overhead and unnecesarry complexity as they are just for experimentation.

Adapted from [refactoring.guru](https://refactoring.guru/).
