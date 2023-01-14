//
// Created by Andrei on 13-Jan-23.
//

#ifndef DESIGNPATTERNS_SMARTPOINTER_H
#define DESIGNPATTERNS_SMARTPOINTER_H

#include <typeinfo>
#include <utility>

template <class T>
class SmartPtr {
    T *ptr;
public:
    // Constructors
    // The default constructor initializes the pointer with nullptr
    // Example: SmartPtr<int> ptr;
    explicit SmartPtr(T *p = nullptr) { ptr = p; }
    // The move constructor takes the ownership of the pointer from the rvalue reference
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    explicit SmartPtr(T&& other) : ptr(new T(std::move(other))) {}

    // Destructor
    // This is the bread and butter of the smart pointer
    ~SmartPtr() { delete(ptr); }

    // Overloading de-referencing operator
    T &operator*() const { return *ptr; }

    // Overloading arrow operator so that members of T can be accessed
    // like a pointer (useful if T represents a class or struct or union type)
    T *operator->() const { return ptr; }

    // Copy constructor
    // Used when a new SmartPtr is created from an existing SmartPtr
    // Example: SmartPtr<int> ptr1 = new int(10);
    // SmartPtr<int> ptr2(ptr1);
    SmartPtr(const SmartPtr<T> &other) {
        ptr = new T(*other.ptr);
    }

    // Copy assignment operator
    // Used when an existing SmartPtr is assigned the value of another existing SmartPtr
    // Example: SmartPtr<int> ptr1 = new int(10);
    // SmartPtr<int> ptr2 = new int(20);
    // ptr2 = ptr1;
    SmartPtr<T> &operator = (const SmartPtr<T> &other) {
        if (this != &other) {
            delete ptr;
            ptr = new T(*other.ptr);
        }
        return *this;
    }

    // Move operator
    // Needed when you want to transfer ownership of a pointer from one SmartPtr to another
    // Example: SmartPtr<int> ptr1 = new int(10);
    // SmartPtr<int> ptr2 = new int(20);
    // ptr2 = std::move(ptr1);
    SmartPtr<T> &operator = (SmartPtr<T>&& other)  noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Dynamic cast
    // Used to cast a SmartPtr to a different type
    // Example: SmartPtr<int> ptr1 = new int(10);
    // SmartPtr<double> ptr2 = ptr1.dynamic_cast<double>();
    template <typename U>
    SmartPtr<U> dynamicCast() {
        U* newPtr = dynamic_cast<U*>(ptr);
        if (newPtr != nullptr) {
            return SmartPtr<U>(newPtr);
        }
        throw std::bad_cast();
    }

    // Static cast
    // Used to cast a SmartPtr to a different type
    // Example: SmartPtr<int> ptr1 = new int(10);
    // SmartPtr<double> ptr2 = ptr1.static_cast<double>();
    template <typename U>
    SmartPtr<U> staticCast() {
        U* newPtr = static_cast<U*>(ptr);
        if (newPtr != nullptr) {
            return SmartPtr<U>(newPtr);
        }
        return SmartPtr<U>();
    }

    // Get pointer
    // Used to get the raw pointer
    // Example: SmartPtr<int> ptr1 = new int(10);
    // int* ptr2 = ptr1.get();
    T* get() const {
        return ptr;
    }
};

#endif//DESIGNPATTERNS_SMARTPOINTER_H
