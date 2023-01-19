/*
 * The Subsystem can accept requests either from the facade or client directly.
 * In any case, to the Subsystem, the Facade is yet another client, and it's not
 * a part of the Subsystem.
 */

#include <iostream>
#include <string>
#include "../SmartPointer.h"

class Subsystem1 {
public:
    [[nodiscard]] static std::string Operation1() {
        return "Subsystem1: Ready!\n";
    }
    [[nodiscard]] static std::string OperationN() {
        return "Subsystem1: Go!\n";
    }
};

/*
 * Some facades can work with multiple subsystems at the same time.
 */

class Subsystem2 {
public:
    [[nodiscard]] static std::string Operation1() {
        return "Subsystem2: Get ready!\n";
    }
    [[nodiscard]] static std::string OperationZ() {
        return "Subsystem2: Fire!\n";
    }
};

/*
 * The Facade class provides a simple interface to the complex logic of one or
 * several subsystems. The Facade delegates the client requests to the
 * appropriate objects within the subsystem. The Facade is also responsible for
 * managing their lifecycle. All of these shields the client from the undesired
 * complexity of the subsystem.
 */

class Facade {
protected:
    SmartPtr<Subsystem1> subsystem1_;
    SmartPtr<Subsystem2> subsystem2_;
    /*
     * Depending on your application's needs, you can provide the Facade with
     * existing subsystem objects or force the Facade to create them on its own.
     */
public:
    explicit Facade(SmartPtr<Subsystem1> subsystem1 = SmartPtr<Subsystem1>(nullptr), SmartPtr<Subsystem2> subsystem2 = SmartPtr<Subsystem2>(nullptr)) {
//        if (!subsystem1) {
//            this->subsystem1_ = SmartPtr<Subsystem1>(new Subsystem1);
//        } else {
//            this->subsystem1_ = subsystem1;
//        }
//        if (!subsystem2) {
//            this->subsystem2_ = SmartPtr<Subsystem2>(new Subsystem2);
//        } else {
//            this->subsystem2_ = subsystem2;
//        }
        this->subsystem1_ = subsystem1.get() ? subsystem1 : SmartPtr<Subsystem1>(new Subsystem1);
        this->subsystem2_ = subsystem2.get() ? subsystem2 : SmartPtr<Subsystem2>(new Subsystem2);
    }

    ~Facade() = default;

    [[nodiscard]] std::string Operation() {
        std::string result = "Facade initializes subsystems:\n";
        result += this->subsystem1_->Operation1();
        result += this->subsystem2_->Operation1();
        result += "Facade orders subsystems to perform the action:\n";
        result += this->subsystem1_->OperationN();
        result += this->subsystem2_->OperationZ();
        return result;
    }
};

/*
 * The client code works with complex subsystems through a simple interface
 * provided by the Facade. When a facade manages the lifecycle of the subsystem,
 * the client might not even know about the existence of the subsystem. This
 * approach lets you keep the complexity under control.
 */

void ClientCode(const SmartPtr<Facade>& facade) {
    // ...
    std::cout << facade->Operation();
    // ...
}

int main() {
    SmartPtr<Subsystem1> subsystem1 = SmartPtr<Subsystem1>(new Subsystem1);
    SmartPtr<Subsystem2> subsystem2 = SmartPtr<Subsystem2>(new Subsystem2);
    SmartPtr<Facade> facade = SmartPtr<Facade>(new Facade(subsystem1, subsystem2));
    ClientCode(facade);
}