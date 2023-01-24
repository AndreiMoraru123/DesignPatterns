#include <iostream>
#include "../SmartPointer.h"

/*
 * The Subject interface declares common operations for both RealSubject and the
 * Proxy. As long as the client works with RealSubject using this interface, you'll
 * be able to pass it a proxy instead of a real subject.
 */

class Subject {
public:
    virtual void Request() const = 0;
    virtual ~Subject() = default;
};

/*
 * The RealSubject contains some core business logic. Usually, RealSubjects are
 * capable of doing some useful work which may also be very slow or sensitive -
 * e.g. correcting input data. A Proxy can solve these issues without any changes
 * to the RealSubject's code.
 */

class RealSubject : public Subject {
public:
    void Request() const override {
        std::cout << "RealSubject: Handling request.\n";
    }
};

/*
 * The Proxy has an interface identical to the RealSubject.
 */

class Proxy : public Subject {
private:
    SmartPtr<RealSubject> real_subject_;
    static bool CheckAccess() {
        // Some real checks should go here.
        std::cout << "Proxy: Checking access prior to firing a real request.\n";
        return true;
    }
    static void LogAccess() {
        std::cout << "Proxy: Logging the time of request.\n";
    }

    /*
 * The Proxy maintains a reference to an object of the RealSubject class. It can
 * be either lazy-loaded or passed to the Proxy by the client.
 */

public:
    explicit Proxy(SmartPtr<RealSubject> real_subject) : real_subject_(std::move(real_subject)) {}

    void Request() const override {
        if (Proxy::CheckAccess()) {
            this->real_subject_->Request();
            Proxy::LogAccess();
        }
    }
};

/*
 * The client code is supposed to work with all objects (both subjects and
 * proxies) via the Subject interface in order to support both real subjects and
 * proxies. In real life, however, clients mostly work with their real subjects
 * directly. In this case, to implement the pattern more easily, you can extend
 * your proxy from the real subject's class.
 */

void ClientCode(const SmartPtr<Subject>& subject) {
    // ...

    subject->Request();

    // ...
}

int main() {
    std::cout << "Client: Executing the client code with a real subject:\n";
    SmartPtr<Subject> real_subject = SmartPtr<Subject>(new RealSubject);
    ClientCode(real_subject);
    std::cout << std::endl;
    std::cout << "Client: Executing the same client code with a proxy:\n";
    SmartPtr<Subject> proxy = SmartPtr<Subject>(new Proxy(SmartPtr<RealSubject>(new RealSubject)));
    ClientCode(proxy);
}