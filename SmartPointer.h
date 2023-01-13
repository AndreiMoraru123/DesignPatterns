//
// Created by Andrei on 13-Jan-23.
//

#ifndef DESIGNPATTERNS_SMARTPOINTER_H
#define DESIGNPATTERNS_SMARTPOINTER_H

#include <utility>

template <class T>
class SmartPtr {
    T *ptr;
public:
    // Constructors
    explicit SmartPtr(T *p = nullptr) { ptr = p; }
    explicit SmartPtr(T&& other) : ptr(new T(std::move(other))) {}

    // Destructor
    ~SmartPtr() { delete(ptr); }

    // Overloading de-referencing operator
    T &operator*() const { return *ptr; }

    // Overloading arrow operator so that members of T can be accessed
    // like a pointer (useful if T represents a class or struct or
    // union type)
    T *operator->() const { return ptr; }

    // Copy constructor
    SmartPtr(const SmartPtr<T> &other) {
        ptr = new T(*other.ptr);
    }

    // Assignment operator
    SmartPtr<T> &operator = (const SmartPtr<T> &other) {
        if (this != &other) {
            delete ptr;
            ptr = new T(*other.ptr);
        }
        return *this;
    }

    // Move operator
    SmartPtr<T> &operator = (SmartPtr<T>&& other)  noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
};

#endif//DESIGNPATTERNS_SMARTPOINTER_H
