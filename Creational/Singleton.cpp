/*
 * The Singleton pattern is a creational design pattern that lets you ensure
 * that a class has only one instance, while providing a global access point
 * to this instance.
*/

#include "../SmartPointer.h"
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

/*
 * It defines the following operations:
 * - getInstance(), which returns the Singleton's instance.
 * - someBusinessLogic(), which provides the Singleton with its business logic.
*/

class Singleton {
protected:
    explicit Singleton(std::string  value) : value_(std::move(value)) {}
    static SmartPtr<Singleton> singleton_;
    static std::mutex mutex_;
    std::string value_;

public:
    // Singleton should not be cloneable.
    Singleton(Singleton &other) = delete;
    // Singleton should not be assignable.
    void operator =(const Singleton &) = delete;

    /*
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static SmartPtr<Singleton> GetInstance(const std::string& value);

    /*
     * Finally, any singleton should define some business logic, which can be
     * executed on its instance.
     */

    void SomeBusinessLogic() {
        // ...
    }

    [[nodiscard]] std::string value() const {
        return value_;
    }
};

SmartPtr<Singleton> Singleton::singleton_ = SmartPtr<Singleton>(nullptr);
std::mutex Singleton::mutex_;

/*
 * Static methods should be defined outside the class.
 */

SmartPtr<Singleton> Singleton::GetInstance(const std::string& value) {

    std::unique_lock<std::mutex> lock(mutex_);
    if(!singleton_) {
        singleton_ = SmartPtr<Singleton>(new Singleton(value));
    }
    return std::move(singleton_);
}

void ThreadFoo() {
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    SmartPtr<Singleton> singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->value() << "\n";
}

void ThreadBar() {
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    SmartPtr<Singleton> singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->value() << "\n";
}

int main() {
    std::cout << "If you see the same value, then singleton was reused (yay!\n"
              << "If you see different values, then 2 singletons were created (booo!!)\n\n"
              << "RESULT:\n";
    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);
    t1.join();
    t2.join();
    return 0;
}
