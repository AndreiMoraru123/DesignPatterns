/*
 * The Adapter is a class that allows objects with incompatible interfaces to
 * collaborate. The Adapter wraps one of the objects to hide the complexity of
 * its interface and exposes a standard interface that is common to all classes
 * that collaborate with it.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include "../SmartPointer.h"

/*
 * In C++, the advantage of multiple inheritance is that it allows you to
 * implement the Adapter pattern in a more straightforward way. In other
 * languages, you might need to use a wrapper class to achieve the same
 * result.
 */

/*
 * The Target defines the domain-specific interface used by the client code.
 */

class Target {
public:
    virtual ~Target() = default;
    [[nodiscard]] virtual std::string Request() const {
        return "Target: The default target's behavior.";
    }
};

/*
 * The Adaptee contains some useful behavior, but its interface is incompatible
 * with the existing client code. The Adaptee needs some adaptation before the
 * client code can use it.
 */

class Adaptee {
public:
    [[nodiscard]] static std::string SpecificRequest() {
        return ".eetpadA eht fo roivaheb laicepS";
    }
};

/*
 * The Adapter makes the Adaptee's interface compatible with the Target's
 * interface.
 */

class Adapter : public Target, private Adaptee {
    public:
    [[nodiscard]] std::string Request() const override {
        std::string to_reverse = Adaptee::SpecificRequest();
        std::reverse(to_reverse.begin(), to_reverse.end());
        return "Adapter: (TRANSLATED) " + to_reverse;
    }
};

/*
 * The client code supports all classes that follow the Target interface.
 */

template <typename T>
void ClientCode(const SmartPtr<T>& target) {
    std::cout << target->Request();
}

int main(){
    std::cout<<"Client: I can work just fine with the Target objects:" << std::endl;
    SmartPtr<Target> target = SmartPtr<Target>(new Target);
    ClientCode(target);
    std::cout << std::endl;
    SmartPtr<Adaptee> adaptee = SmartPtr<Adaptee>(new Adaptee);
    std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:" << std::endl;
    std::cout << "Adaptee: " << adaptee->SpecificRequest();
    std::cout << std::endl;
    std::cout << "Client: But I can work with it via the Adapter:" << std::endl;
    SmartPtr<Adapter> adapter = SmartPtr<Adapter>(new Adapter);
    ClientCode(adapter);
}