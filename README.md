# Design Patterns

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white) ![CLion](https://img.shields.io/badge/CLion-black?style=for-the-badge&logo=clion&logoColor=white)

Design Patterns using a custom Smart Pointer

Adapted from [refactoring.guru](https://refactoring.guru/)

## How to use the pointer

#### Allocating an integer to the heap

```cpp
#include "SmartPointer.h"

SmartPtr<int> ptr = SmartPtr<int>(new int(10));
```

#### Its use count will be one:

```cpp
std::cout << "Use count: " << ptr.use_count();  // Use count: 1
```

#### Get the raw pointer:

```cpp
int* rawPtr = ptr.get();
std::cout << "Raw pointer: " << rawPtr;  // Raw pointer: 0x1df5c9315c0
```

#### And then get the value back:

```cpp
std::cout << "Value: " << *rawPtr;  // Value: 10
```

#### When you create a new SmartPtr that points to the same object, then its use count will increase:

```cpp
SmartPtr<int> ptr2 = ptr;
std::cout << "Use count: " << ptr.use_count();  // Use count: 2
```

#### And when you move the ownership, the use count of the original object will be reset:

```cpp
SmartPtr<int> ptr3 = std::move(ptr);
std::cout << "Use count for ptr: " << ptr.use_count();  // Use count: 0
std::cout << "Use count for ptr2: " << ptr2.use_count();  // Use count: 2
std::cout << "Use count for ptr3: " << ptr3.use_count();  // Use count: 2
```

#### (Somewhat) Supports casting:

```cpp
class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {};
```

#### Static:

```cpp
SmartPtr<Base> basePtr = SmartPtr<Base>(new Derived());
SmartPtr<Derived> derivedPtr = basePtr.staticCast<Derived>();
std::cout << "Use count for basePtr: " << basePtr.use_count();  // Use count for basePtr: 1
std::cout << "Use count for derivedPtr: " << derivedPtr.use_count();  // Use count for derivedPtr: 1
```

#### Dynamic: 

```cpp
  SmartPtr<Derived> derivedPtr2 = basePtr.dynamicCast<Derived>();
  std::cout << "Use count for basePtr: " << basePtr.use_count();  // Use count for basePtr: 1
  std::cout << "Use count for derivedPtr2: " << derivedPtr2.use_count();  // Use count for derivedPtr2: 1
```

#### And, most importantly, you do not have to call ```delete```.

> **Note**
> These will only work with C++ 11 onwards.

> **Warning**
> May have ridiculous overhead and unnecesarry complexity as they are just for experimentation.
