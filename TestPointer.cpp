//
// Created by Andrei on 14-Jan-23.
//

#include "SmartPointer.h"
#include <cassert>

class Test {
public:
    static void TestDefaultConstructor() {
        SmartPtr<int> ptr;
        assert(ptr.get() == nullptr);
        assert(ptr.use_count() == 0);
    }

    static void TestMoveConstructor() {
        SmartPtr<int> ptr1 = SmartPtr<int>(new int(10));
        SmartPtr<int> ptr2 = std::move(ptr1);
        assert(ptr1.get() == nullptr);
        assert(ptr1.use_count() == 0);
        assert(ptr2.get() != nullptr);
        assert(ptr2.use_count() == 1);
        assert(*ptr2.get() == 10);
    }

    static void TestCopyConstructor() {
        SmartPtr<int> ptr1 = SmartPtr<int>(new int(10));
        SmartPtr<int> ptr2 = ptr1;
        assert(ptr1.get() != nullptr);
        assert(ptr2.get() != nullptr);
        assert(ptr1.use_count() == 2);
        assert(ptr2.use_count() == 2);
    }

    static void TestCopyAssignment() {
        SmartPtr<int> ptr1 = SmartPtr<int>(new int(10));
        SmartPtr<int> ptr2 = SmartPtr<int>(new int(20));
        ptr2 = ptr1;
        assert(ptr1.get() != nullptr);
        assert(ptr2.get() != nullptr);
        assert(ptr1.use_count() == 2);
        assert(ptr2.use_count() == 2);
    }

    static void TestMoveOperator() {
        SmartPtr<int> ptr1 = SmartPtr<int>(new int(10));
        SmartPtr<int> ptr2 = SmartPtr<int>(new int(20));
        ptr2 = std::move(ptr1);
        assert(ptr1.get() == nullptr);
        assert(ptr2.get() != nullptr);
        assert(ptr1.use_count() == 0);
        assert(ptr2.use_count() == 1);
        assert(*ptr2.get() == 10);
    }

    class Base {
    public:
        virtual ~Base() = default;
    };

    class Derived : public Base {};

    static void TestDynamicCast() {
        SmartPtr<Base> basePtr = SmartPtr<Base>(new Derived());
        SmartPtr<Derived> derivedPtr = basePtr.dynamicCast<Derived>();
        assert(basePtr.get() != nullptr);
        assert(derivedPtr.get() != nullptr);
        assert(basePtr.use_count() == 1);
        assert(derivedPtr.use_count() == 1);
    }

    static void TestStaticCast() {
        SmartPtr<Base> basePtr = SmartPtr<Base>(new Derived());
        SmartPtr<Derived> derivedPtr = basePtr.staticCast<Derived>();
        assert(basePtr.get() != nullptr);
        assert(derivedPtr.get() != nullptr);
        assert(basePtr.use_count() == 1);
        assert(derivedPtr.use_count() == 1);
    }

    static void TestGet() {
        SmartPtr<int> ptr = SmartPtr<int>(new int(10));
        assert(ptr.get() != nullptr);
        assert(*ptr.get() == 10);
    }

    static void TestUseCount() {
        SmartPtr<int> ptr1 = SmartPtr<int>(new int(10));
        SmartPtr<int> ptr2 = ptr1;
        assert(ptr1.use_count() == 2);
        assert(ptr2.use_count() == 2);
    }

    static void TestNullptr() {
        SmartPtr<int> ptr = SmartPtr<int>(nullptr);
        assert(ptr.get() == nullptr);
        assert(!ptr);
        assert(ptr.use_count() == 0);
    }

    static void TestEquality() {
        SmartPtr<int> ptr1 = SmartPtr<int>(new int(10));
        SmartPtr<int> ptr2 = ptr1;
        SmartPtr<int> ptr3 = SmartPtr<int>(new int(10));
        assert(ptr1 == ptr2);
        assert(ptr1 != ptr3);
    }

    static void TestInequality() {
        SmartPtr<int> ptr1 = SmartPtr<int>(new int(10));
        SmartPtr<int> ptr2 = ptr1;
        SmartPtr<int> ptr3 = SmartPtr<int>(new int(10));
        assert(ptr1 != ptr3);
        assert(ptr1 == ptr2);
    }

    static void TestDereference() {
        SmartPtr<int> ptr = SmartPtr<int>(new int(10));
        assert(*ptr == 10);
    }

    static void TestArrow() {
        class TestClass {
        public:
            int value = 10;
        };
        SmartPtr<TestClass> ptr = SmartPtr<TestClass>(new TestClass());
        assert(ptr->value == 10);
    }
};

int main() {
    Test::TestDefaultConstructor();
    Test::TestMoveConstructor();
    Test::TestCopyConstructor();
    Test::TestCopyAssignment();
    Test::TestMoveOperator();
    Test::TestDynamicCast();
    Test::TestStaticCast();
    Test::TestGet();
    Test::TestUseCount();
    Test::TestNullptr();
    Test::TestEquality();
    Test::TestInequality();
    Test::TestDereference();
    Test::TestArrow();
}