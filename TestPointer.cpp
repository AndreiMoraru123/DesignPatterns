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
};

int main() {
    Test::TestDefaultConstructor();
    Test::TestMoveConstructor();
    Test::TestCopyConstructor();
    return 0;
}