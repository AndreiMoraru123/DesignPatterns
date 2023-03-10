//
// Created by Andrei on 13-Jan-23.
//

#ifndef DESIGNPATTERNS_SMARTPOINTER_H
#define DESIGNPATTERNS_SMARTPOINTER_H

#include <typeinfo>
#include <utility>
#include <cstddef>
#include <memory>
#include <atomic>

template <class T>
class SmartPtr {
    T *ptr;
public:
    int *ref_count{};
    // Constructors
    // The default constructor initializes the pointer with nullptr
    // Example: SmartPtr<int> ptr;
    explicit SmartPtr(T *p = nullptr) : ptr(p), ref_count(new int(1)) {}

    // The move constructor takes the ownership of the pointer from the rvalue reference
    // It is used to move the value of a temporary object of type T into a SmartPtr
    explicit SmartPtr(T&& other) : ptr(new T(std::move(other))), ref_count(new int(1)) {}

    // This second move constructor is a move constructor for the smart pointer class itself
    // It is used to move the ownership of the pointer from one smart pointer to another
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    //          SmartPtr<int> ptr2 = std::move(ptr1);
    SmartPtr(SmartPtr<T>&& other) noexcept : ptr(other.ptr), ref_count(other.ref_count) {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    // Destructor
    // This is the bread and butter of the smart pointer
    ~SmartPtr() {
        if (ptr != nullptr) {
            --(*ref_count);
            if(*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
        }
    }

    // ----------------- Shared Pointer Methods -----------------
    template<typename U>
    explicit SmartPtr(const std::shared_ptr<U> &other) : ptr(new T(*other.get())), ref_count(new int) {
        *ref_count = other.use_count();
    }


    explicit SmartPtr(std::shared_ptr<T> &&other) : ptr(other.get()), ref_count(new int(1)) {
        ref_count = other.use_count();
        other.reset();
    }
    // ----------------- Shared Pointer Methods -----------------


    // Overloading == operator
    bool operator==(const SmartPtr<T>& other) const {
        return ptr == other.ptr && ref_count == other.ref_count;
    }

    // Overloading != operator
    bool operator!=(const SmartPtr<T>& other) const
    {
        return !(*this == other);
    }

    // Overloading new operator
    void* operator new(std::size_t size) {
        return ::new SmartPtr<T>(static_cast<T*>(::operator new(size)));
    }

    // Overloading delete operator
    // Not really necessary, but it's good practice
    void operator delete(void* ptr) {
        ::operator delete(ptr);
    }

    // Overloading de-referencing operator
    T &operator*() const { return *ptr; }

    // Overloading arrow operator so that members of T can be accessed
    // like a pointer (useful if T represents a class or struct or union type)
    T *operator->() const { return ptr; }

    // Shallow Copy constructor
    // Used when a new SmartPtr is created from an existing SmartPtr
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    // SmartPtr<int> ptr2(ptr1);
    // SmartPtr(const SmartPtr<T> &other) : ptr(new T(*other.ptr)), ref_count(other.ref_count) {
    //  ++(*ref_count);
    // }

    // Deep Copy constructor
    // Used when a new SmartPtr is created from an existing SmartPtr
    SmartPtr(const SmartPtr<T> &other) : ptr(other.ptr), ref_count(other.ref_count) {
        ++(*ref_count);
    }

    // Copy assignment operator
    // Used when an existing SmartPtr is assigned the value of another existing SmartPtr
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    // SmartPtr<int> ptr2 = new int(20);
    // ptr2 = ptr1;
    SmartPtr<T> &operator = (const SmartPtr<T> &other) {
        if (this != &other) {
            --(*ref_count);
            if (*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            ++(*ref_count);
        }
        return *this;
    }

    // Move operator
    // Needed when you want to transfer ownership of a pointer from one SmartPtr to another
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    // SmartPtr<int> ptr2 = new int(20);
    // ptr2 = std::move(ptr1);
    SmartPtr<T> &operator = (SmartPtr<T>&& other)  noexcept {
        if (this != &other) {
            --(*ref_count);
            if (*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = new int(0);
        }
        return *this;
    }

    // Dynamic cast
    // Used to cast a SmartPtr to a different type
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    // SmartPtr<double> ptr2 = ptr1.dynamic_cast<double>();
    template <typename U>
    SmartPtr<U> dynamicCast() {
        U* newPtr = dynamic_cast<U*>(ptr);
        if (newPtr != nullptr) {
            SmartPtr<U> newSmartPtr(newPtr);
            newSmartPtr.ref_count = ref_count; // share the ref_count with the original SmartPtr
            return newSmartPtr;
        }
        throw std::bad_cast();
    }

    // Static cast
    // Used to cast a SmartPtr to a different type
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    // SmartPtr<double> ptr2 = ptr1.static_cast<double>();
    template <typename U>
    SmartPtr<U> staticCast() {
        U* newPtr = static_cast<U*>(ptr);
        if (newPtr != nullptr) {
            SmartPtr<U> newSmartPtr(newPtr);
            newSmartPtr.ref_count = ref_count; // share the ref_count with the original SmartPtr
            return newSmartPtr;
        }
        throw std::bad_cast();
    }

    // Get pointer
    // Used to get the raw pointer
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    // int* ptr2 = ptr1.get();
    T* get() const {
        return ptr;
    }

    // Use count
    // Used to get the number of SmartPtrs that point to the same object
    // Example: SmartPtr<int> ptr1 = SmartPtr<int>(new int(5));
    // SmartPtr<int> ptr2 = ptr1;
    // int count = ptr1.use_count();
    [[nodiscard]] int use_count() const {
        return ptr == nullptr ? 0 : *ref_count;
    }

    // Null pointer check
    bool operator!() const {
        return ptr == nullptr;
    }
};

#endif//DESIGNPATTERNS_SMARTPOINTER_H