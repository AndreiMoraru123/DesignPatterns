/*
 * The Bridge pattern is used when we need to decouple an
 * abstraction from its implementation so that the two can
 * vary independently. This type of design pattern comes under
 * structural pattern as this pattern decouples implementation
 * class and abstract class by providing a bridge structure
 * between them.
 */

#include <iostream>
#include <string>
#include "../SmartPointer.h"


/*
 *  The Implementation defines the interface for implementation classes.
 *  It doesn't have to match the Abstraction's interface. In fact, the two
 *  interfaces can be entirely different. Typically, the Implementation
 *  interface provides only primitive operations, while the Abstraction
 *  defines higher-level operations based on those primitives.
 */

class Implementation {
public:
    virtual ~Implementation() = default;
    [[nodiscard]] virtual std::string OperationImplementation() const = 0;
};

/*
 * Each Concrete Implementation corresponds to a specific platform and
 * implements the Implementation interface using that platform's API.
 */

class ConcreteImplementationA : public Implementation {
public:
    [[nodiscard]] std::string OperationImplementation() const override {
        return "ConcreteImplementationA: Here's the result on the platform A.";
    }
};

class ConcreteImplementationB : public Implementation {
public:
    [[nodiscard]] std::string OperationImplementation() const override {
        return "ConcreteImplementationB: Here's the result on the platform B.";
    }
};

/*
 * The Abstraction defines the interface for the "control" part of the two
 * class hierarchies. It maintains a reference to an object of the
 * Implementation hierarchy and delegates all the real work to this object.
 */

class Abstraction {
    /**
     * @var Implementation
     */
protected:
    SmartPtr<Implementation> implementation;
public:
    explicit Abstraction(SmartPtr<Implementation>& implementation) : implementation(implementation) {}
    virtual ~Abstraction() = default;

    [[nodiscard]] virtual std::string Operation() const {
        return "Abstraction: Base operation with:\n" +
               this->implementation->OperationImplementation();
    }
};

/*
 * You can extend the Abstraction without changing the Implementation classes.
 */

class ExtendedAbstraction : public Abstraction {
public:
    explicit ExtendedAbstraction(SmartPtr<Implementation>& implementation) : Abstraction(implementation) {}
    [[nodiscard]] std::string Operation() const override {
        return "ExtendedAbstraction: Extended operation with:\n" +
               this->implementation->OperationImplementation();
    }
};

/*
 * Except for the initialization phase, where an Abstraction object gets linked
 * with a specific Implementation object, the client code should only depend on
 * the Abstraction class. This way the client code can support any abstraction-
 * implementation combination.
 */

void ClientCode(const SmartPtr<Abstraction>& abstraction) {
    // ...
    std::cout << abstraction->Operation();
    // ...
}

/*
 * The client code should be able to work with any pre-configured abstraction-
 * implementation combination.
 */

int main() {
    SmartPtr<Implementation> implementation = SmartPtr<Implementation>(new ConcreteImplementationA);
    SmartPtr<Abstraction> abstraction = SmartPtr<Abstraction>(new Abstraction(implementation));
    ClientCode(abstraction);

    std::cout << std::endl;

    implementation = SmartPtr<Implementation>(new ConcreteImplementationB);
    abstraction = SmartPtr<Abstraction>(new ExtendedAbstraction(implementation));
    ClientCode(abstraction);

    return 0;
}